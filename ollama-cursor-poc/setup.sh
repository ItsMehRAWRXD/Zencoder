#!/bin/bash

# Ollama + Cursor POC Setup Script
# Optimized for AMD RX 7800 XT (16GB VRAM)

echo "🚀 Ollama + Cursor POC Setup Script"
echo "===================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for Ollama
echo "🔍 Checking for Ollama..."
if command_exists ollama; then
    echo -e "${GREEN}✓ Ollama is installed${NC}"
else
    echo -e "${YELLOW}⚠ Ollama not found. Installing...${NC}"
    curl -fsSL https://ollama.com/install.sh | sh
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Ollama installed successfully${NC}"
    else
        echo -e "${RED}✗ Failed to install Ollama${NC}"
        exit 1
    fi
fi

# Check for Python
echo ""
echo "🔍 Checking for Python..."
if command_exists python3; then
    PYTHON_VERSION=$(python3 --version 2>&1 | awk '{print $2}')
    echo -e "${GREEN}✓ Python $PYTHON_VERSION is installed${NC}"
else
    echo -e "${RED}✗ Python 3 is required but not installed${NC}"
    echo "Please install Python 3.8 or higher"
    exit 1
fi

# Check for pip
echo ""
echo "🔍 Checking for pip..."
if command_exists pip3; then
    echo -e "${GREEN}✓ pip is installed${NC}"
else
    echo -e "${YELLOW}⚠ pip not found. Installing...${NC}"
    python3 -m ensurepip --upgrade
fi

# Install Python dependencies
echo ""
echo "📦 Installing Python dependencies..."
cd /workspace/ollama-cursor-poc/backend
pip3 install -r requirements.txt
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Dependencies installed successfully${NC}"
else
    echo -e "${RED}✗ Failed to install dependencies${NC}"
    exit 1
fi

# Start Ollama service
echo ""
echo "🔄 Starting Ollama service..."
if pgrep -x "ollama" > /dev/null; then
    echo -e "${GREEN}✓ Ollama is already running${NC}"
else
    echo "Starting Ollama in background..."
    nohup ollama serve > /tmp/ollama.log 2>&1 &
    sleep 3
    if pgrep -x "ollama" > /dev/null; then
        echo -e "${GREEN}✓ Ollama started successfully${NC}"
    else
        echo -e "${RED}✗ Failed to start Ollama${NC}"
        echo "Please start Ollama manually with: ollama serve"
    fi
fi

# List available models
echo ""
echo "📋 Available Ollama models:"
ollama list

# Suggest models for 16GB VRAM
echo ""
echo "🎮 Recommended models for AMD RX 7800 XT (16GB VRAM):"
echo ""
echo "Top uncensored models optimized for your GPU:"
echo "1. dolphin-llama3:8b-v2.9-q5_K_M (~5.5GB) - Fast & capable"
echo "2. dolphin-mistral:7b-v2.8-q6_K (~6GB) - High quality"
echo "3. wizard-vicuna-13b-uncensored:q4_K_M (~7.5GB) - Classic"
echo "4. yi-34b-chat:q3_K_M (~14GB) - Maximum capability"
echo "5. mixtral-8x7b-instruct:q3_K_M (~16GB) - State-of-the-art"
echo ""
echo -e "${YELLOW}Would you like to pull a model? (y/n)${NC}"
read -r response

if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    echo ""
    echo "Enter the model name (e.g., dolphin-llama3:8b-v2.9-q5_K_M):"
    read -r model_name
    echo "Pulling $model_name..."
    ollama pull "$model_name"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Model pulled successfully${NC}"
    else
        echo -e "${RED}✗ Failed to pull model${NC}"
    fi
fi

# Create start script
echo ""
echo "📝 Creating start script..."
cat > /workspace/ollama-cursor-poc/start.sh << 'EOF'
#!/bin/bash

echo "Starting Ollama Cursor POC..."

# Start Ollama if not running
if ! pgrep -x "ollama" > /dev/null; then
    echo "Starting Ollama service..."
    ollama serve &
    sleep 3
fi

# Start FastAPI server
echo "Starting FastAPI server..."
cd /workspace/ollama-cursor-poc/backend
python3 main.py &
SERVER_PID=$!

echo ""
echo "✅ Server is running!"
echo "🌐 Open http://localhost:8000/static/index.html in your browser"
echo ""
echo "Press Ctrl+C to stop the server"

# Wait for interrupt
trap "echo 'Stopping server...'; kill $SERVER_PID; exit" INT
wait $SERVER_PID
EOF

chmod +x /workspace/ollama-cursor-poc/start.sh

echo ""
echo -e "${GREEN}✅ Setup complete!${NC}"
echo ""
echo "To start the application, run:"
echo -e "${YELLOW}./start.sh${NC}"
echo ""
echo "Or start manually:"
echo "1. cd /workspace/ollama-cursor-poc/backend"
echo "2. python3 main.py"
echo "3. Open http://localhost:8000/static/index.html"
echo ""
echo "Enjoy your uncensored AI chat interface! 🤖"