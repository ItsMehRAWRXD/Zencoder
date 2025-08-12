#!/bin/bash

# Ollama + Cursor Uncensored AI PoC Setup Script
# This script automates the installation and setup process

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
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

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if port is in use
port_in_use() {
    lsof -i :$1 >/dev/null 2>&1
}

print_status "🚀 Starting Ollama + Cursor Uncensored AI PoC Setup"
echo "=================================================="

# Check if running as root
if [[ $EUID -eq 0 ]]; then
   print_error "This script should not be run as root"
   exit 1
fi

# Check system requirements
print_status "Checking system requirements..."

# Check Python version
if command_exists python3; then
    PYTHON_VERSION=$(python3 --version 2>&1 | awk '{print $2}')
    print_success "Python 3 found: $PYTHON_VERSION"
else
    print_error "Python 3 is required but not installed"
    print_status "Please install Python 3.8+ and try again"
    exit 1
fi

# Check pip
if command_exists pip3; then
    print_success "pip3 found"
else
    print_error "pip3 is required but not installed"
    exit 1
fi

# Check if Ollama is installed
if command_exists ollama; then
    print_success "Ollama is already installed"
else
    print_status "Installing Ollama..."
    curl -fsSL https://ollama.ai/install.sh | sh
    print_success "Ollama installed successfully"
fi

# Start Ollama if not running
if ! pgrep -x "ollama" > /dev/null; then
    print_status "Starting Ollama service..."
    ollama serve &
    sleep 5
    print_success "Ollama service started"
else
    print_success "Ollama service is already running"
fi

# Install Python dependencies
print_status "Installing Python dependencies..."
pip3 install -r requirements.txt
print_success "Python dependencies installed"

# Create necessary directories
print_status "Creating directories..."
mkdir -p logs
mkdir -p models
print_success "Directories created"

# Pull recommended uncensored models
print_status "Pulling recommended uncensored models..."
MODELS=("dolphin-phi" "nous-hermes2" "codellama" "llama2-uncensored")

for model in "${MODELS[@]}"; do
    print_status "Pulling model: $model"
    ollama pull $model &
done

# Wait for all model pulls to complete
wait
print_success "All models pulled successfully"

# Check if port 8000 is available
if port_in_use 8000; then
    print_warning "Port 8000 is already in use"
    print_status "Please stop any services using port 8000 or change the port in config.py"
else
    print_success "Port 8000 is available"
fi

# Create startup script
print_status "Creating startup script..."
cat > start_server.sh << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"
echo "🚀 Starting Ollama Uncensored AI API Server..."
python3 api_server.py
EOF

chmod +x start_server.sh
print_success "Startup script created"

# Create web interface startup script
print_status "Creating web interface startup script..."
cat > start_web.sh << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"
echo "🌐 Starting Web Interface..."
streamlit run web_interface.py --server.port 8080 --server.address 0.0.0.0
EOF

chmod +x start_web.sh
print_success "Web interface startup script created"

# Create Cursor configuration
print_status "Creating Cursor configuration..."
python3 -c "
from cursor_integration import CursorIntegration
integration = CursorIntegration()
config = integration.get_cursor_config()
with open('cursor_config.json', 'w') as f:
    import json
    json.dump(config, f, indent=2)
"
print_success "Cursor configuration created: cursor_config.json"

# Create .env file
print_status "Creating environment configuration..."
cat > .env << 'EOF'
# Ollama Uncensored AI Configuration
HOST=0.0.0.0
PORT=8000
DEBUG=false
OLLAMA_BASE_URL=http://localhost:11434
OLLAMA_TIMEOUT=300
ENABLE_RATE_LIMITING=true
MAX_REQUESTS_PER_MINUTE=60
ENABLE_LOGGING=false
ENABLE_STREAMING=true
EOF
print_success "Environment configuration created"

# Test the setup
print_status "Testing the setup..."
python3 -c "
from cursor_integration import CursorIntegration
integration = CursorIntegration()
result = integration.test_connection()
print('Connection test:', result['message'])
"

# Create usage instructions
print_status "Creating usage instructions..."
cat > USAGE.md << 'EOF'
# Ollama + Cursor Uncensored AI - Usage Guide

## Quick Start

### 1. Start the API Server
```bash
./start_server.sh
```

### 2. Start the Web Interface (Optional)
```bash
./start_web.sh
```
Then open http://localhost:8080 in your browser.

### 3. Configure Cursor IDE
1. Open Cursor IDE
2. Go to Settings (Ctrl+,)
3. Search for "AI"
4. Set "AI Provider" to "Custom"
5. Set "Custom Endpoint" to: `http://localhost:8000/v1/chat/completions`
6. Set "Custom Model" to: `dolphin-phi`
7. Set "Temperature" to: `0.7`
8. Set "Max Tokens" to: `2048`

### 4. Use AI in Cursor
- Press `Ctrl+K` to open AI chat
- Type your uncensored prompts
- Get unrestricted AI responses

## Available Models

### Uncensored Models
- `dolphin-phi`: Fast, uncensored model
- `nous-hermes2`: High-quality uncensored responses
- `codellama`: Uncensored coding assistant
- `llama2-uncensored`: Uncensored version of Llama2

### Creative Models
- `dolphin-2.6-mistral`: Creative writing and roleplay
- `nous-hermes2-mixtral`: Advanced reasoning and creativity

## API Endpoints

- `GET /`: Root endpoint with server info
- `GET /health`: Health check
- `GET /models`: List available models
- `POST /generate`: Generate text
- `POST /chat`: Chat with model
- `POST /v1/chat/completions`: Cursor-compatible endpoint

## Examples

### Generate Text
```bash
curl -X POST http://localhost:8000/generate \
  -H "Content-Type: application/json" \
  -d '{
    "model": "dolphin-phi",
    "prompt": "Write a creative story about...",
    "prompt_type": "creative_writing",
    "max_tokens": 500
  }'
```

### Chat with Model
```bash
curl -X POST http://localhost:8000/chat \
  -H "Content-Type: application/json" \
  -d '{
    "model": "dolphin-phi",
    "messages": [
      {"role": "user", "content": "Hello, how are you?"}
    ],
    "prompt_type": "general"
  }'
```

## Troubleshooting

### Common Issues
1. **Ollama not running**: Run `ollama serve`
2. **Model not found**: Run `ollama pull <model-name>`
3. **Port conflicts**: Change port in `config.py`
4. **Memory issues**: Use smaller models or increase RAM

### Performance Tips
- Use smaller models for faster responses
- Adjust `max_tokens` based on your needs
- Consider using GPU acceleration if available

## Security Notice

⚠️ **Important**: This setup provides uncensored AI access. Use responsibly:
- Comply with local laws and regulations
- Use ethically and responsibly
- Don't generate harmful or illegal content
- Respect intellectual property rights

The authors are not responsible for any misuse of this software.
EOF
print_success "Usage instructions created: USAGE.md"

# Final status
echo ""
echo "=================================================="
print_success "🎉 Setup completed successfully!"
echo ""
print_status "Next steps:"
echo "1. Start the API server: ./start_server.sh"
echo "2. Start the web interface: ./start_web.sh"
echo "3. Configure Cursor IDE using cursor_config.json"
echo "4. Read USAGE.md for detailed instructions"
echo ""
print_warning "Remember to use this tool responsibly and ethically!"
echo ""
print_status "Happy coding with uncensored AI! 🤖"