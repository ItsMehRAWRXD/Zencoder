#!/bin/bash

# GPU-Optimized Ollama Setup for AMD RX 7800 XT (16GB VRAM)
# This script sets up Ollama with ROCm support for AMD GPUs and downloads
# the best uncensored models that will fit in 16GB VRAM

set -e

echo "🚀 AMD RX 7800 XT (16GB) Optimized Setup"
echo "========================================"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_gpu() { echo -e "${PURPLE}[GPU]${NC} $1"; }

# Check for AMD GPU
check_amd_gpu() {
    print_status "Checking for AMD GPU..."
    
    if lspci | grep -i amd | grep -i vga >/dev/null 2>&1; then
        gpu_info=$(lspci | grep -i amd | grep -i vga | head -1)
        print_success "AMD GPU detected: $gpu_info"
        
        if echo "$gpu_info" | grep -i "7800" >/dev/null 2>&1; then
            print_gpu "RX 7800 XT confirmed! Optimizing for 16GB VRAM..."
        else
            print_warning "Different AMD GPU detected. This script is optimized for RX 7800 XT."
        fi
    else
        print_warning "AMD GPU not detected. This script is optimized for AMD RX 7800 XT."
    fi
}

# Install ROCm for AMD GPU support
install_rocm() {
    print_status "Installing ROCm for AMD GPU acceleration..."
    
    # Check if ROCm is already installed
    if command -v rocm-smi >/dev/null 2>&1; then
        print_success "ROCm is already installed"
        rocm-smi
        return
    fi
    
    # Install ROCm based on distribution
    if [ -f /etc/debian_version ]; then
        # Ubuntu/Debian
        print_status "Installing ROCm on Ubuntu/Debian..."
        
        # Add ROCm repository
        wget -q -O - https://repo.radeon.com/rocm/rocm.gpg.key | sudo apt-key add -
        echo 'deb [arch=amd64] https://repo.radeon.com/rocm/apt/5.7 ubuntu main' | sudo tee /etc/apt/sources.list.d/rocm.list
        
        sudo apt update
        sudo apt install -y rocm-dev rocm-libs rocm-utils
        
        # Add user to render group
        sudo usermod -a -G render,video $USER
        
        print_warning "You may need to reboot for ROCm to work properly"
        
    elif [ -f /etc/redhat-release ]; then
        # RHEL/CentOS/Fedora
        print_status "Installing ROCm on RHEL/CentOS/Fedora..."
        
        sudo dnf install -y rocm-dev rocm-runtime
        sudo usermod -a -G render,video $USER
        
    else
        print_warning "Unsupported distribution for automatic ROCm installation"
        print_warning "Please install ROCm manually from: https://rocm.docs.amd.com/en/latest/deploy/linux/quick_start.html"
    fi
}

# Install Ollama with GPU support
install_ollama_gpu() {
    print_status "Installing Ollama with GPU support..."
    
    if command -v ollama >/dev/null 2>&1; then
        print_success "Ollama is already installed"
    else
        curl -fsSL https://ollama.com/install.sh | sh
    fi
    
    # Configure Ollama for AMD GPU
    print_status "Configuring Ollama for AMD GPU..."
    
    # Create systemd override for GPU support
    sudo mkdir -p /etc/systemd/system/ollama.service.d/
    
    cat << 'EOF' | sudo tee /etc/systemd/system/ollama.service.d/override.conf
[Service]
Environment="HSA_OVERRIDE_GFX_VERSION=11.0.0"
Environment="OLLAMA_HOST=0.0.0.0:11434"
Environment="OLLAMA_GPU_LAYERS=99"
Environment="HIP_VISIBLE_DEVICES=0"
Environment="ROCM_PATH=/opt/rocm"
EOF

    sudo systemctl daemon-reload
    sudo systemctl enable ollama
    sudo systemctl restart ollama
    
    print_success "Ollama configured for AMD GPU"
}

# Download uncensored models optimized for 16GB VRAM
download_uncensored_models() {
    print_gpu "Downloading uncensored models optimized for 16GB VRAM..."
    
    # Wait for Ollama to be ready
    sleep 10
    
    # Models that fit well in 16GB VRAM with excellent performance
    declare -A models=(
        # Fast and efficient models (4-8GB VRAM)
        ["dolphin-llama3:8b"]="~5GB - Best balance of speed/quality. Completely uncensored Llama3."
        ["nous-hermes2:10.7b"]="~6GB - Excellent for coding. Very capable and uncensored."
        ["openhermes:7b"]="~4GB - Fast responses. Good for quick tasks."
        ["dolphin-phi:2.7b"]="~2GB - Ultra-fast for simple tasks and autocomplete."
        
        # Medium models (8-12GB VRAM)  
        ["wizard-vicuna-uncensored:13b"]="~8GB - Very powerful uncensored model."
        ["dolphin-mixtral:8x7b"]="~26GB - Will use model sharding, slower but very capable."
        ["llama2-uncensored:13b"]="~7GB - Classic uncensored Llama2."
        ["vicuna:13b"]="~7GB - Great conversational model, less filtered."
        
        # Specialized models
        ["deepseek-coder:6.7b"]="~4GB - Specialized for coding, minimal filtering."
        ["codellama:13b"]="~7GB - Meta's code model, good for programming."
        ["starling-lm:7b"]="~4GB - High-quality responses, minimal censorship."
        ["neural-chat:7b"]="~4GB - Intel's uncensored chat model."
        
        # Experimental/Alternative models
        ["orca-mini:13b"]="~7GB - Microsoft's uncensored variant."
        ["zephyr:7b-beta"]="~4GB - Mistral-based, less restricted."
        ["openchat:7b"]="~4GB - Open-source uncensored chat model."
    )
    
    print_gpu "Available uncensored models for your 16GB setup:"
    echo "=================================================="
    
    for model in "${!models[@]}"; do
        echo -e "${PURPLE}$model${NC}: ${models[$model]}"
    done
    
    echo ""
    read -p "Download all recommended models? (y/N): " download_all
    
    if [[ $download_all =~ ^[Yy]$ ]]; then
        # Download all models
        for model in "${!models[@]}"; do
            download_model_with_gpu_check "$model"
        done
    else
        # Interactive selection
        echo ""
        echo "Select models to download (recommended for 16GB):"
        echo "1. Essential Pack (dolphin-llama3:8b, nous-hermes2:10.7b, openhermes:7b)"
        echo "2. Coding Pack (deepseek-coder:6.7b, codellama:13b, dolphin-llama3:8b)"
        echo "3. Power Pack (wizard-vicuna-uncensored:13b, dolphin-mixtral:8x7b)"
        echo "4. Speed Pack (dolphin-phi:2.7b, starling-lm:7b, neural-chat:7b)"
        echo "5. Custom selection"
        
        read -p "Choose pack (1-5): " pack_choice
        
        case $pack_choice in
            1)
                download_model_with_gpu_check "dolphin-llama3:8b"
                download_model_with_gpu_check "nous-hermes2:10.7b"
                download_model_with_gpu_check "openhermes:7b"
                ;;
            2)
                download_model_with_gpu_check "deepseek-coder:6.7b"
                download_model_with_gpu_check "codellama:13b"
                download_model_with_gpu_check "dolphin-llama3:8b"
                ;;
            3)
                download_model_with_gpu_check "wizard-vicuna-uncensored:13b"
                download_model_with_gpu_check "dolphin-mixtral:8x7b"
                ;;
            4)
                download_model_with_gpu_check "dolphin-phi:2.7b"
                download_model_with_gpu_check "starling-lm:7b"
                download_model_with_gpu_check "neural-chat:7b"
                ;;
            5)
                for model in "${!models[@]}"; do
                    read -p "Download $model? (y/N): " choice
                    if [[ $choice =~ ^[Yy]$ ]]; then
                        download_model_with_gpu_check "$model"
                    fi
                done
                ;;
        esac
    fi
}

download_model_with_gpu_check() {
    local model=$1
    print_gpu "Downloading $model..."
    
    if ollama list | grep -q "$model"; then
        print_success "$model already exists"
        return
    fi
    
    # Download with retries
    local max_attempts=3
    local attempt=1
    
    while [ $attempt -le $max_attempts ]; do
        if timeout 1800 ollama pull "$model"; then
            print_success "✅ $model downloaded successfully"
            
            # Test GPU usage
            print_gpu "Testing GPU acceleration for $model..."
            response=$(timeout 30 ollama run "$model" "Say 'GPU test successful' if you can see this." --verbose 2>&1 || echo "timeout")
            
            if echo "$response" | grep -q "GPU\|gpu\|success"; then
                print_success "🚀 $model is using GPU acceleration!"
            else
                print_warning "⚠️  $model may be running on CPU"
            fi
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
        print_error "❌ Failed to download $model"
    fi
}

# Configure Cursor for GPU-accelerated models
configure_cursor_gpu() {
    print_status "Configuring Cursor for GPU-accelerated models..."
    
    CURSOR_CONFIG_DIR="$HOME/.cursor/User"
    mkdir -p "$CURSOR_CONFIG_DIR"
    
    # Backup existing config
    if [ -f "$CURSOR_CONFIG_DIR/settings.json" ]; then
        cp "$CURSOR_CONFIG_DIR/settings.json" "$CURSOR_CONFIG_DIR/settings.json.backup.$(date +%s)"
    fi
    
    # Optimized settings for 16GB GPU
    cat > "$CURSOR_CONFIG_DIR/settings.json" << 'EOF'
{
    "cursor.chat.model": "ollama/dolphin-llama3:8b",
    "cursor.chat.ollamaBaseUrl": "http://localhost:11434",
    "cursor.autocomplete.model": "ollama/dolphin-phi:2.7b",
    "cursor.autocomplete.ollamaBaseUrl": "http://localhost:11434",
    "cursor.autocomplete.enabled": true,
    "cursor.autocomplete.delay": 100,
    "continue.telemetryEnabled": false,
    "cursor.general.enableCodeLens": true,
    "cursor.general.enableInlineChat": true
}
EOF
    
    # GPU-optimized Continue config
    CONTINUE_CONFIG_DIR="$HOME/.continue"
    mkdir -p "$CONTINUE_CONFIG_DIR"
    
    cat > "$CONTINUE_CONFIG_DIR/config.json" << 'EOF'
{
  "models": [
    {
      "title": "🚀 Dolphin Llama3 8B (GPU)",
      "provider": "ollama",
      "model": "dolphin-llama3:8b",
      "apiBase": "http://localhost:11434",
      "contextLength": 8192,
      "completionOptions": {
        "temperature": 0.7,
        "top_p": 0.9,
        "num_gpu": 99
      }
    },
    {
      "title": "🧙 Wizard Vicuna 13B (GPU)",
      "provider": "ollama",
      "model": "wizard-vicuna-uncensored:13b",
      "apiBase": "http://localhost:11434",
      "contextLength": 4096,
      "completionOptions": {
        "temperature": 0.8,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "💻 DeepSeek Coder 6.7B (GPU)",
      "provider": "ollama",
      "model": "deepseek-coder:6.7b",
      "apiBase": "http://localhost:11434",
      "contextLength": 16384,
      "completionOptions": {
        "temperature": 0.6,
        "top_p": 0.85,
        "num_gpu": 99
      }
    },
    {
      "title": "⚡ Dolphin Phi 2.7B (Fast GPU)",
      "provider": "ollama",
      "model": "dolphin-phi:2.7b",
      "apiBase": "http://localhost:11434",
      "contextLength": 2048,
      "completionOptions": {
        "temperature": 0.5,
        "top_p": 0.8,
        "num_gpu": 99
      }
    }
  ],
  "tabAutocompleteModel": {
    "title": "⚡ Fast Autocomplete",
    "provider": "ollama",
    "model": "dolphin-phi:2.7b",
    "apiBase": "http://localhost:11434",
    "completionOptions": {
      "temperature": 0.3,
      "max_tokens": 50,
      "num_gpu": 99
    }
  }
}
EOF
    
    print_success "Cursor configured for GPU acceleration"
}

# Performance monitoring and tips
show_performance_info() {
    echo ""
    print_gpu "🎯 Performance Optimization Tips for RX 7800 XT:"
    echo "=================================================="
    echo ""
    echo "🔥 VRAM Usage Optimization:"
    echo "  • Use dolphin-phi:2.7b for autocomplete (ultra-fast)"
    echo "  • Use dolphin-llama3:8b for general chat (balanced)"
    echo "  • Use wizard-vicuna-uncensored:13b for complex tasks"
    echo "  • Monitor VRAM with: watch -n 1 rocm-smi"
    echo ""
    echo "⚡ Speed Optimization:"
    echo "  • Set num_gpu=99 to use full GPU"
    echo "  • Lower temperature for faster responses"
    echo "  • Use smaller context lengths when possible"
    echo ""
    echo "🧠 Model Combinations:"
    echo "  • Autocomplete: dolphin-phi:2.7b (instant responses)"
    echo "  • Code chat: deepseek-coder:6.7b (coding expert)"
    echo "  • General chat: dolphin-llama3:8b (uncensored)"
    echo "  • Complex tasks: wizard-vicuna-uncensored:13b (powerful)"
    echo ""
    echo "🔧 Useful Commands:"
    echo "  • Monitor GPU: rocm-smi -d"
    echo "  • Check models: ollama list"
    echo "  • Test model: ollama run model-name 'test prompt'"
    echo "  • Restart Ollama: sudo systemctl restart ollama"
}

# Main execution
main() {
    check_amd_gpu
    install_rocm
    install_ollama_gpu
    
    # Wait for service to be ready
    print_status "Waiting for Ollama GPU service..."
    sleep 15
    
    download_uncensored_models
    configure_cursor_gpu
    show_performance_info
    
    echo ""
    print_success "🎉 AMD RX 7800 XT setup complete!"
    print_gpu "Your 16GB GPU is ready for uncensored AI acceleration!"
    echo ""
    echo "Next steps:"
    echo "1. Restart Cursor if it was open"
    echo "2. Install Continue extension in Cursor"
    echo "3. Test with Ctrl+K for chat and Tab for autocomplete"
    echo "4. Monitor GPU usage with: rocm-smi"
}

# Handle interruption
trap 'print_error "Setup interrupted"; exit 1' INT

main "$@"