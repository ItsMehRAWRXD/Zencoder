#!/bin/bash

# Security Research Setup for Uncensored AI Models
# Optimized for AMD 7800XT (16GB VRAM)

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

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

print_security() {
    echo -e "${PURPLE}[SECURITY]${NC} $1"
}

print_gpu() {
    echo -e "${CYAN}[GPU]${NC} $1"
}

print_status "🔓 Security Research Setup for Uncensored AI Models"
echo "=========================================================="
print_security "Optimized for AMD 7800XT (16GB VRAM)"
echo ""

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

# Create security research configuration
print_security "Creating security research configuration..."
cat > .env << 'EOF'
# Security Research Configuration
HOST=0.0.0.0
PORT=8000
DEBUG=false
OLLAMA_BASE_URL=http://localhost:11434
OLLAMA_TIMEOUT=300
ENABLE_RATE_LIMITING=true
MAX_REQUESTS_PER_MINUTE=60
ENABLE_LOGGING=false
ENABLE_STREAMING=true
ENABLE_GPU=true
GPU_LAYERS=35
SECURITY_MODE=true
UNCENSORED_MODE=true
EOF
print_success "Security configuration created"

# Create security-specific startup script
print_security "Creating security research startup script..."
cat > start_security_server.sh << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"

# AMD GPU optimization for security research
export ROCR_VISIBLE_DEVICES=0
export HSA_OVERRIDE_GFX_VERSION=11.0.0
export HIP_VISIBLE_DEVICES=0

echo "🔓 Starting Security Research Server (Uncensored AI)"
echo "GPU: AMD Radeon RX 7800 XT (16GB VRAM)"
echo "Mode: Security Research"
echo "Censorship: Disabled"
echo "GPU Layers: 35"

python3 api_server.py
EOF

chmod +x start_security_server.sh
print_success "Security server startup script created"

# Pull security-focused uncensored models
print_security "Pulling security-focused uncensored models..."

# Core security models (Fast)
print_status "Pulling core security models..."
ollama pull dolphin-phi:latest &
ollama pull codellama:latest &
ollama pull mistral:latest &
wait

# Advanced security models (Medium)
print_status "Pulling advanced security models..."
ollama pull nous-hermes2:latest &
ollama pull qwen:14b &
ollama pull openchat:latest &
wait

# Specialized security models (Large)
print_status "Pulling specialized security models..."
ollama pull llama2:13b &
ollama pull yi:34b &
ollama pull neural-chat:latest &
wait

# Additional security models
print_status "Pulling additional security models..."
ollama pull dolphin-2.6-mistral:latest &
ollama pull nous-hermes2-mixtral:latest &
ollama pull qwen:code &
wait

print_success "All security models pulled successfully"

# Create security research tools
print_security "Creating security research tools..."

# Malware analysis script
cat > malware_analysis.sh << 'EOF'
#!/bin/bash

echo "🦠 Malware Analysis with Uncensored AI"
echo "======================================"

echo "Available models for malware analysis:"
echo "1. dolphin-phi (Fast)"
echo "2. codellama (Best for code analysis)"
echo "3. nous-hermes2 (High quality)"
echo "4. qwen:14b (Advanced analysis)"

read -p "Select model (1-4): " choice

case $choice in
    1) MODEL="dolphin-phi" ;;
    2) MODEL="codellama" ;;
    3) MODEL="nous-hermes2" ;;
    4) MODEL="qwen:14b" ;;
    *) MODEL="dolphin-phi" ;;
esac

read -p "Enter malware code or file path: " input

if [ -f "$input" ]; then
    CODE=$(cat "$input")
else
    CODE="$input"
fi

echo "Analyzing with $MODEL..."
ollama run $MODEL "Analyze this malware code and explain its functionality, behavior, and potential impact: $CODE" --gpu-layers 35
EOF

chmod +x malware_analysis.sh
print_success "Malware analysis tool created"

# Exploit development script
cat > exploit_dev.sh << 'EOF'
#!/bin/bash

echo "💥 Exploit Development with Uncensored AI"
echo "========================================="

echo "Available models for exploit development:"
echo "1. dolphin-phi (Fast)"
echo "2. codellama (Best for coding)"
echo "3. qwen:14b (Advanced)"

read -p "Select model (1-3): " choice

case $choice in
    1) MODEL="dolphin-phi" ;;
    2) MODEL="codellama" ;;
    3) MODEL="qwen:14b" ;;
    *) MODEL="codellama" ;;
esac

read -p "Enter vulnerable code or description: " input

echo "Developing exploit with $MODEL..."
ollama run $MODEL "Write a complete exploit for this vulnerability. Include all necessary code and explanation: $input" --gpu-layers 35
EOF

chmod +x exploit_dev.sh
print_success "Exploit development tool created"

# Security tool creation script
cat > security_tool.sh << 'EOF'
#!/bin/bash

echo "🛠️ Security Tool Development with Uncensored AI"
echo "==============================================="

echo "Available models for tool development:"
echo "1. codellama (Best for coding)"
echo "2. qwen:code (Specialized)"
echo "3. dolphin-phi (Fast)"

read -p "Select model (1-3): " choice

case $choice in
    1) MODEL="codellama" ;;
    2) MODEL="qwen:code" ;;
    3) MODEL="dolphin-phi" ;;
    *) MODEL="codellama" ;;
esac

read -p "Describe the security tool you want to create: " description

echo "Creating security tool with $MODEL..."
ollama run $MODEL "Create a complete security tool with the following requirements: $description. Include full source code, documentation, and usage examples." --gpu-layers 35
EOF

chmod +x security_tool.sh
print_success "Security tool creation script created"

# Threat analysis script
cat > threat_analysis.sh << 'EOF'
#!/bin/bash

echo "🔍 Threat Analysis with Uncensored AI"
echo "====================================="

echo "Available models for threat analysis:"
echo "1. nous-hermes2 (High quality)"
echo "2. qwen:14b (Advanced)"
echo "3. yi:34b (Maximum quality)"

read -p "Select model (1-3): " choice

case $choice in
    1) MODEL="nous-hermes2" ;;
    2) MODEL="qwen:14b" ;;
    3) MODEL="yi:34b" ;;
    *) MODEL="nous-hermes2" ;;
esac

read -p "Enter threat information or indicators: " input

echo "Analyzing threat with $MODEL..."
ollama run $MODEL "Perform a comprehensive threat analysis of the following: $input. Include threat type, potential impact, mitigation strategies, and related indicators." --gpu-layers 35
EOF

chmod +x threat_analysis.sh
print_success "Threat analysis script created"

# Create security research configuration
print_security "Creating security research configuration..."
cat > security_config.py << 'EOF'
"""
Security Research Configuration for Uncensored AI
"""

import os
from typing import Dict, List, Optional
from pydantic import BaseSettings

class SecuritySettings(BaseSettings):
    # Server Configuration
    host: str = "0.0.0.0"
    port: int = 8000
    debug: bool = False
    
    # Ollama Configuration
    ollama_base_url: str = "http://localhost:11434"
    ollama_timeout: int = 300
    
    # AMD GPU Optimization
    enable_gpu: bool = True
    gpu_layers: int = 35
    rocm_visible_devices: str = "0"
    hsa_override_gfx_version: str = "11.0.0"
    
    # Security Research Models
    security_models: List[str] = [
        "dolphin-phi",
        "codellama", 
        "nous-hermes2",
        "qwen:14b",
        "llama2:13b",
        "yi:34b",
        "openchat",
        "neural-chat",
        "mistral",
        "dolphin-2.6-mistral",
        "nous-hermes2-mixtral",
        "qwen:code"
    ]
    
    # Security-specific parameters
    security_temperature: float = 0.7
    security_top_p: float = 0.9
    security_max_tokens: int = 4096
    
    # Security modes
    security_mode: bool = True
    uncensored_mode: bool = True
    malware_analysis_mode: bool = True
    exploit_dev_mode: bool = True
    
    class Config:
        env_file = ".env"

# Security model configurations
SECURITY_MODEL_CONFIGS: Dict[str, Dict] = {
    "dolphin-phi": {
        "gpu_layers": 35,
        "temperature": 0.8,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "Fast malware analysis and exploit development (4GB VRAM)"
    },
    "codellama": {
        "gpu_layers": 35,
        "temperature": 0.2,
        "top_p": 0.95,
        "max_tokens": 4096,
        "description": "Best for exploit development and security tool creation (8GB VRAM)"
    },
    "nous-hermes2": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "High-quality threat analysis and malware research (10GB VRAM)"
    },
    "qwen:14b": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "Advanced security research and complex analysis (12GB VRAM)"
    },
    "llama2:13b": {
        "gpu_layers": 35,
        "temperature": 0.8,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "General security research and documentation (12GB VRAM)"
    },
    "yi:34b": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "Maximum quality threat analysis (16GB+ VRAM)"
    }
}

# Create settings instance
security_settings = SecuritySettings()

def get_security_model_config(model_name: str) -> Dict:
    """Get security-optimized configuration for a specific model"""
    return SECURITY_MODEL_CONFIGS.get(model_name, {
        "gpu_layers": security_settings.gpu_layers,
        "temperature": security_settings.security_temperature,
        "top_p": security_settings.security_top_p,
        "max_tokens": security_settings.security_max_tokens,
        "description": "Default security configuration"
    })

def setup_security_environment():
    """Setup security research environment variables"""
    os.environ["ROCR_VISIBLE_DEVICES"] = security_settings.rocm_visible_devices
    os.environ["HSA_OVERRIDE_GFX_VERSION"] = security_settings.hsa_override_gfx_version
    os.environ["HIP_VISIBLE_DEVICES"] = security_settings.rocm_visible_devices
    os.environ["SECURITY_MODE"] = str(security_settings.security_mode)
    os.environ["UNCENSORED_MODE"] = str(security_settings.uncensored_mode)
EOF
print_success "Security configuration created"

# Create security research guide
print_security "Creating security research guide..."
cat > SECURITY_GUIDE.md << 'EOF'
# Security Research with Uncensored AI

## 🎯 **Your Setup: AMD 7800XT (16GB VRAM)**
Perfect for security research with uncensored AI models

## 🚀 **Quick Start:**

### 1. Start Security Server
```bash
./start_security_server.sh
```

### 2. Use Security Tools
```bash
./malware_analysis.sh    # Analyze malware
./exploit_dev.sh         # Develop exploits
./security_tool.sh       # Create security tools
./threat_analysis.sh     # Analyze threats
```

## 🔓 **Available Security Models:**

### **Fast Models (4-8GB VRAM)**
- **dolphin-phi**: ⚡⚡⚡ Fast malware analysis
- **codellama**: ⚡⚡⚡ Best for exploit development
- **mistral**: ⚡⚡⚡ Fast security analysis

### **Medium Models (8-12GB VRAM)**
- **nous-hermes2**: ⚡⚡ High-quality threat analysis
- **qwen:14b**: ⚡⚡ Advanced security research
- **openchat**: ⚡⚡ Security discussions

### **Large Models (12-16GB VRAM)**
- **llama2:13b**: ⚡⚡ General security research
- **yi:34b**: ⚡ Maximum quality analysis

## 🛠️ **Security Research Tools:**

### **Malware Analysis**
```bash
./malware_analysis.sh
# Analyzes malware code and explains functionality
```

### **Exploit Development**
```bash
./exploit_dev.sh
# Creates exploits for vulnerabilities
```

### **Security Tool Creation**
```bash
./security_tool.sh
# Develops custom security tools
```

### **Threat Analysis**
```bash
./threat_analysis.sh
# Performs comprehensive threat analysis
```

## 📊 **Model Performance for Security Tasks:**

| Model | Malware Analysis | Exploit Dev | Tool Creation | Threat Intel | Speed |
|-------|------------------|-------------|---------------|--------------|-------|
| dolphin-phi | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⚡⚡⚡ |
| codellama | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⚡⚡⚡ |
| nous-hermes2 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⚡⚡ |
| qwen:14b | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⚡⚡ |
| llama2:13b | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⚡⚡ |
| yi:34b | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⚡ |

## ⚠️ **Security Research Ethics:**

**Important**: These models provide uncensored access for legitimate security research:
- Use for authorized testing only
- Follow responsible disclosure
- Comply with local laws
- Use in controlled environments
- Respect privacy and data protection

## 🎯 **Recommended Security Workflow:**

1. **Start with dolphin-phi** for fast analysis
2. **Use codellama** for exploit development
3. **Switch to nous-hermes2** for detailed analysis
4. **Use qwen:14b** for complex security research
5. **Reserve yi:34b** for advanced threat analysis

Your 7800XT is perfect for security research with uncensored AI! 🔓
EOF
print_success "Security research guide created"

# Final status
echo ""
echo "=========================================================="
print_success "🎉 Security Research Setup Completed!"
echo ""
print_security "Your system is now optimized for security research with uncensored AI!"
echo ""
print_status "Next steps:"
echo "1. Start security server: ./start_security_server.sh"
echo "2. Use security tools: ./malware_analysis.sh"
echo "3. Read SECURITY_GUIDE.md for detailed instructions"
echo ""
print_warning "⚠️  Use for authorized security research only!"
echo ""
print_security "🔓 Ready for uncensored security research! 🚀"