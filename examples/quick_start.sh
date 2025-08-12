#!/bin/bash

# Quick Start Script for Ollama + Cursor + Uncensored AI
# This script automates the entire setup process

set -e

echo "🚀 Ollama + Cursor + Uncensored AI Quick Start"
echo "=============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check system requirements
check_requirements() {
    print_status "Checking system requirements..."
    
    # Check if we're on Linux
    if [[ "$OSTYPE" != "linux-gnu"* ]]; then
        print_error "This script is designed for Linux. Please adapt for your OS."
        exit 1
    fi
    
    # Check available memory (should have at least 8GB for larger models)
    total_mem=$(grep MemTotal /proc/meminfo | awk '{print $2}')
    total_mem_gb=$((total_mem / 1024 / 1024))
    
    if [ $total_mem_gb -lt 8 ]; then
        print_warning "You have ${total_mem_gb}GB RAM. Some larger models may not work well."
        print_warning "Consider using smaller models like dolphin-llama3:8b"
    else
        print_success "RAM: ${total_mem_gb}GB - Good for running larger models"
    fi
    
    # Check disk space (models can be large)
    available_space=$(df / | awk 'NR==2{print $4}')
    available_space_gb=$((available_space / 1024 / 1024))
    
    if [ $available_space_gb -lt 20 ]; then
        print_warning "Available disk space: ${available_space_gb}GB. You may need more space for multiple models."
    else
        print_success "Disk space: ${available_space_gb}GB available"
    fi
}

# Install Ollama
install_ollama() {
    print_status "Installing Ollama..."
    
    if command_exists ollama; then
        print_success "Ollama is already installed"
        return
    fi
    
    # Download and install Ollama
    curl -fsSL https://ollama.com/install.sh | sh
    
    if command_exists ollama; then
        print_success "Ollama installed successfully"
    else
        print_error "Failed to install Ollama"
        exit 1
    fi
}

# Start Ollama service
start_ollama() {
    print_status "Starting Ollama service..."
    
    # Enable and start systemd service
    if systemctl is-enabled ollama >/dev/null 2>&1; then
        print_status "Ollama service is already enabled"
    else
        sudo systemctl enable ollama
        print_success "Ollama service enabled"
    fi
    
    if systemctl is-active ollama >/dev/null 2>&1; then
        print_success "Ollama service is already running"
    else
        sudo systemctl start ollama
        print_success "Ollama service started"
    fi
    
    # Wait for service to be ready
    print_status "Waiting for Ollama to be ready..."
    sleep 5
    
    # Test if Ollama is responding
    for i in {1..10}; do
        if curl -s http://localhost:11434/api/tags >/dev/null 2>&1; then
            print_success "Ollama is ready!"
            return
        fi
        sleep 2
    done
    
    print_error "Ollama failed to start properly"
    exit 1
}

# Download models
download_models() {
    print_status "Downloading uncensored AI models..."
    
    # Define models to download (start with essential ones)
    declare -a models=(
        "dolphin-llama3:8b"
        "nous-hermes2:10.7b"
    )
    
    # Optional larger models (only download if user has enough RAM)
    total_mem=$(grep MemTotal /proc/meminfo | awk '{print $2}')
    total_mem_gb=$((total_mem / 1024 / 1024))
    
    if [ $total_mem_gb -ge 16 ]; then
        models+=("dolphin-mixtral:8x7b")
        models+=("wizard-vicuna-uncensored:13b")
    fi
    
    if [ $total_mem_gb -ge 8 ]; then
        models+=("openhermes:7b")
    fi
    
    for model in "${models[@]}"; do
        print_status "Downloading $model..."
        
        # Check if model already exists
        if ollama list | grep -q "$model"; then
            print_success "$model is already downloaded"
            continue
        fi
        
        # Download with retry logic
        max_attempts=3
        attempt=1
        
        while [ $attempt -le $max_attempts ]; do
            if ollama pull "$model"; then
                print_success "Successfully downloaded $model"
                break
            else
                print_warning "Attempt $attempt failed for $model"
                ((attempt++))
                if [ $attempt -le $max_attempts ]; then
                    print_status "Retrying in 10 seconds..."
                    sleep 10
                fi
            fi
        done
        
        if [ $attempt -gt $max_attempts ]; then
            print_error "Failed to download $model after $max_attempts attempts"
        fi
    done
}

# Setup Cursor configuration
setup_cursor() {
    print_status "Setting up Cursor configuration..."
    
    # Create Cursor config directory if it doesn't exist
    CURSOR_CONFIG_DIR="$HOME/.cursor/User"
    mkdir -p "$CURSOR_CONFIG_DIR"
    
    # Backup existing settings if they exist
    if [ -f "$CURSOR_CONFIG_DIR/settings.json" ]; then
        cp "$CURSOR_CONFIG_DIR/settings.json" "$CURSOR_CONFIG_DIR/settings.json.backup.$(date +%s)"
        print_status "Backed up existing Cursor settings"
    fi
    
    # Create basic Cursor settings for Ollama
    cat > "$CURSOR_CONFIG_DIR/settings.json" << 'EOF'
{
    "cursor.chat.model": "ollama/dolphin-llama3:8b",
    "cursor.chat.ollamaBaseUrl": "http://localhost:11434",
    "cursor.autocomplete.model": "ollama/dolphin-llama3:8b",
    "cursor.autocomplete.ollamaBaseUrl": "http://localhost:11434",
    "continue.telemetryEnabled": false
}
EOF
    
    print_success "Cursor settings configured"
    
    # Copy Continue configuration
    CONTINUE_CONFIG_DIR="$HOME/.continue"
    mkdir -p "$CONTINUE_CONFIG_DIR"
    
    if [ -f "examples/continue_config.json" ]; then
        cp "examples/continue_config.json" "$CONTINUE_CONFIG_DIR/config.json"
        print_success "Continue extension configured"
    else
        print_warning "Continue config file not found. You may need to configure it manually."
    fi
}

# Test the setup
test_setup() {
    print_status "Testing the setup..."
    
    # Test if Python is available for running tests
    if command_exists python3; then
        if [ -f "examples/test_ollama.py" ]; then
            print_status "Running setup tests..."
            python3 examples/test_ollama.py
        else
            print_warning "Test script not found. Skipping automated tests."
        fi
    else
        print_warning "Python3 not found. Skipping automated tests."
    fi
    
    # Manual test
    print_status "Testing Ollama API manually..."
    
    response=$(curl -s -X POST http://localhost:11434/api/generate \
        -H "Content-Type: application/json" \
        -d '{
            "model": "dolphin-llama3:8b",
            "prompt": "Say hello in Python code",
            "stream": false
        }' | grep -o '"response":"[^"]*"' | cut -d'"' -f4 || echo "")
    
    if [ -n "$response" ]; then
        print_success "Ollama API test successful!"
        echo "Response: $response"
    else
        print_error "Ollama API test failed"
    fi
}

# Print final instructions
print_instructions() {
    echo ""
    echo "🎉 Setup Complete!"
    echo "=================="
    echo ""
    echo "📋 What's been set up:"
    echo "  ✅ Ollama installed and running"
    echo "  ✅ Uncensored AI models downloaded"
    echo "  ✅ Cursor configured for local AI"
    echo "  ✅ Continue extension configured"
    echo ""
    echo "🔧 How to use:"
    echo "  1. Open Cursor IDE"
    echo "  2. Install the 'Continue' extension if not already installed"
    echo "  3. Use Ctrl+K (or Cmd+K) to chat with AI"
    echo "  4. Use Ctrl+I (or Cmd+I) for inline code editing"
    echo "  5. Tab completion should work automatically"
    echo ""
    echo "📋 Available models:"
    ollama list
    echo ""
    echo "🛠️  Useful commands:"
    echo "  • Check Ollama status: systemctl status ollama"
    echo "  • List models: ollama list"
    echo "  • Test setup: python3 examples/test_ollama.py"
    echo "  • View logs: journalctl -u ollama -f"
    echo ""
    echo "🔒 Privacy & Security:"
    echo "  • All AI processing happens locally"
    echo "  • No data sent to external servers"
    echo "  • Models are uncensored - use responsibly"
    echo ""
    echo "💡 Troubleshooting:"
    echo "  • If models are slow, try smaller ones like openhermes:7b"
    echo "  • If out of memory, restart Ollama: sudo systemctl restart ollama"
    echo "  • For help, check cursor_config.md"
    echo ""
    print_success "Enjoy your uncensored, private AI coding assistant!"
}

# Main execution
main() {
    check_requirements
    install_ollama
    start_ollama
    download_models
    setup_cursor
    test_setup
    print_instructions
}

# Handle script interruption
trap 'print_error "Script interrupted by user"; exit 1' INT

# Run main function
main "$@"