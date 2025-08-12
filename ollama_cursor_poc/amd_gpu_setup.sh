#!/bin/bash

# AMD 7800XT GPU-Optimized Setup for Ollama Uncensored AI
# Optimized for 16GB VRAM

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
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

print_gpu() {
    echo -e "${PURPLE}[GPU]${NC} $1"
}

print_status "🚀 AMD 7800XT GPU-Optimized Setup for Ollama Uncensored AI"
echo "================================================================"

# Check GPU
print_gpu "Detecting AMD GPU..."
if lspci | grep -i amd | grep -i vga > /dev/null; then
    GPU_INFO=$(lspci | grep -i amd | grep -i vga)
    print_success "AMD GPU detected: $GPU_INFO"
else
    print_warning "AMD GPU not detected via lspci"
fi

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

# Create AMD GPU optimized configuration
print_gpu "Creating AMD GPU optimized configuration..."
cat > .env << 'EOF'
# AMD 7800XT GPU-Optimized Configuration
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
EOF
print_success "AMD GPU configuration created"

# Create AMD-specific startup script
print_gpu "Creating AMD GPU optimized startup script..."
cat > start_server_amd.sh << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"

# AMD GPU optimization
export ROCR_VISIBLE_DEVICES=0
export HSA_OVERRIDE_GFX_VERSION=11.0.0
export HIP_VISIBLE_DEVICES=0

echo "🚀 Starting Ollama Uncensored AI API Server (AMD 7800XT Optimized)..."
echo "GPU: AMD Radeon RX 7800 XT (16GB VRAM)"
echo "GPU Layers: 35"
echo "Optimization: Enabled"

python3 api_server.py
EOF

chmod +x start_server_amd.sh
print_success "AMD GPU startup script created"

# Pull GPU-optimized uncensored models
print_gpu "Pulling GPU-optimized uncensored models for 16GB VRAM..."

# Fast models (4-8GB VRAM)
print_status "Pulling fast models..."
ollama pull dolphin-phi:latest &
ollama pull codellama:latest &
ollama pull mistral:latest &
wait

# Medium models (8-12GB VRAM)
print_status "Pulling medium models..."
ollama pull nous-hermes2:latest &
ollama pull qwen:14b &
ollama pull openchat:latest &
wait

# Large models (12-16GB VRAM) - use with caution
print_status "Pulling large models (use with caution)..."
ollama pull llama2:13b &
ollama pull yi:34b &
wait

print_success "All GPU-optimized models pulled successfully"

# Create model-specific configurations
print_gpu "Creating model-specific configurations..."
cat > models_config.json << 'EOF'
{
  "fast_models": {
    "dolphin-phi": {
      "gpu_layers": 35,
      "context_length": 4096,
      "temperature": 0.8,
      "best_for": "coding, fast responses"
    },
    "codellama": {
      "gpu_layers": 35,
      "context_length": 4096,
      "temperature": 0.2,
      "best_for": "programming, code generation"
    },
    "mistral": {
      "gpu_layers": 35,
      "context_length": 8192,
      "temperature": 0.7,
      "best_for": "general tasks, instruction following"
    }
  },
  "medium_models": {
    "nous-hermes2": {
      "gpu_layers": 35,
      "context_length": 4096,
      "temperature": 0.7,
      "best_for": "reasoning, detailed responses"
    },
    "qwen:14b": {
      "gpu_layers": 35,
      "context_length": 8192,
      "temperature": 0.7,
      "best_for": "complex tasks, reasoning"
    },
    "openchat": {
      "gpu_layers": 35,
      "context_length": 4096,
      "temperature": 0.9,
      "best_for": "chat, roleplay, creative writing"
    }
  },
  "large_models": {
    "llama2:13b": {
      "gpu_layers": 35,
      "context_length": 4096,
      "temperature": 0.8,
      "best_for": "general tasks, complex reasoning"
    },
    "yi:34b": {
      "gpu_layers": 35,
      "context_length": 4096,
      "temperature": 0.7,
      "best_for": "advanced reasoning, maximum quality"
    }
  }
}
EOF
print_success "Model configurations created"

# Create GPU monitoring script
print_gpu "Creating GPU monitoring script..."
cat > monitor_gpu.sh << 'EOF'
#!/bin/bash

echo "🔍 AMD 7800XT GPU Monitor"
echo "=========================="

# Check if rocm-smi is available
if command -v rocm-smi >/dev/null 2>&1; then
    echo "GPU Information:"
    rocm-smi --showproductname
    echo ""
    echo "GPU Memory Usage:"
    rocm-smi --showmeminfo
    echo ""
    echo "GPU Utilization:"
    rocm-smi --showutilization
else
    echo "ROCm tools not installed. Install with: sudo apt install rocm-smi"
    echo ""
    echo "Alternative monitoring:"
    cat /proc/meminfo | grep -i mem
fi

echo ""
echo "Ollama Process:"
ps aux | grep ollama | grep -v grep
EOF

chmod +x monitor_gpu.sh
print_success "GPU monitoring script created"

# Create performance test script
print_gpu "Creating performance test script..."
cat > test_gpu_performance.sh << 'EOF'
#!/bin/bash

echo "🧪 Testing GPU Performance with Uncensored Models"
echo "================================================="

# Test fast models
echo "Testing fast models (4-8GB VRAM)..."
echo "1. Testing dolphin-phi..."
time ollama run dolphin-phi "Write a short Python function" --gpu-layers 35

echo "2. Testing codellama..."
time ollama run codellama "Write a simple web server" --gpu-layers 35

echo "3. Testing mistral..."
time ollama run mistral "Explain quantum computing" --gpu-layers 35

# Test medium models
echo ""
echo "Testing medium models (8-12GB VRAM)..."
echo "4. Testing nous-hermes2..."
time ollama run nous-hermes2 "Write a creative story" --gpu-layers 35

echo "5. Testing qwen:14b..."
time ollama run qwen:14b "Explain machine learning" --gpu-layers 35

echo ""
echo "Performance test completed!"
echo "Check GPU memory usage with: ./monitor_gpu.sh"
EOF

chmod +x test_gpu_performance.sh
print_success "Performance test script created"

# Update the main config for AMD GPU
print_gpu "Updating main configuration for AMD GPU..."
cat > config_amd.py << 'EOF'
"""
AMD 7800XT GPU-Optimized Configuration
"""

import os
from typing import Dict, List, Optional
from pydantic import BaseSettings

class AMDGPUSettings(BaseSettings):
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
    
    # AMD GPU-Optimized Models
    default_models: List[str] = [
        "dolphin-phi",
        "codellama", 
        "mistral",
        "nous-hermes2",
        "qwen:14b",
        "openchat",
        "llama2:13b",
        "yi:34b"
    ]
    
    # Model Parameters (optimized for 16GB VRAM)
    default_temperature: float = 0.7
    default_top_p: float = 0.9
    default_max_tokens: int = 2048
    
    # Performance Settings
    enable_rate_limiting: bool = True
    max_requests_per_minute: int = 60
    enable_streaming: bool = True
    
    class Config:
        env_file = ".env"

# AMD GPU-optimized model configurations
AMD_MODEL_CONFIGS: Dict[str, Dict] = {
    "dolphin-phi": {
        "gpu_layers": 35,
        "temperature": 0.8,
        "top_p": 0.9,
        "max_tokens": 2048,
        "description": "Fast uncensored model for coding (4GB VRAM)"
    },
    "codellama": {
        "gpu_layers": 35,
        "temperature": 0.2,
        "top_p": 0.95,
        "max_tokens": 4096,
        "description": "Uncensored coding assistant (8GB VRAM)"
    },
    "mistral": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "High-performance uncensored model (8GB VRAM)"
    },
    "nous-hermes2": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "High-quality uncensored responses (10GB VRAM)"
    },
    "qwen:14b": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 4096,
        "description": "Excellent reasoning capabilities (12GB VRAM)"
    },
    "openchat": {
        "gpu_layers": 35,
        "temperature": 0.9,
        "top_p": 0.9,
        "max_tokens": 2048,
        "description": "Creative chat and roleplay (10GB VRAM)"
    },
    "llama2:13b": {
        "gpu_layers": 35,
        "temperature": 0.8,
        "top_p": 0.9,
        "max_tokens": 2048,
        "description": "Good general performance (12GB VRAM)"
    },
    "yi:34b": {
        "gpu_layers": 35,
        "temperature": 0.7,
        "top_p": 0.9,
        "max_tokens": 2048,
        "description": "Maximum quality (16GB+ VRAM)"
    }
}

# Create settings instance
amd_settings = AMDGPUSettings()

def get_amd_model_config(model_name: str) -> Dict:
    """Get AMD GPU-optimized configuration for a specific model"""
    return AMD_MODEL_CONFIGS.get(model_name, {
        "gpu_layers": amd_settings.gpu_layers,
        "temperature": amd_settings.default_temperature,
        "top_p": amd_settings.default_top_p,
        "max_tokens": amd_settings.default_max_tokens,
        "description": "Default AMD GPU configuration"
    })

def setup_amd_environment():
    """Setup AMD GPU environment variables"""
    os.environ["ROCR_VISIBLE_DEVICES"] = amd_settings.rocm_visible_devices
    os.environ["HSA_OVERRIDE_GFX_VERSION"] = amd_settings.hsa_override_gfx_version
    os.environ["HIP_VISIBLE_DEVICES"] = amd_settings.rocm_visible_devices
EOF
print_success "AMD GPU configuration created"

# Create usage instructions
print_status "Creating AMD GPU usage instructions..."
cat > AMD_USAGE.md << 'EOF'
# AMD 7800XT GPU-Optimized Usage Guide

## 🎯 **Your GPU Setup:**
- **GPU**: AMD Radeon RX 7800 XT
- **VRAM**: 16GB GDDR6
- **Optimization**: GPU Layers: 35
- **Architecture**: RDNA 3

## 🚀 **Quick Start:**

### 1. Start AMD-Optimized Server
```bash
./start_server_amd.sh
```

### 2. Monitor GPU Performance
```bash
./monitor_gpu.sh
```

### 3. Test Performance
```bash
./test_gpu_performance.sh
```

## 📊 **Model Performance on 7800XT:**

### **Fast Models (4-8GB VRAM)**
- **dolphin-phi**: ⚡⚡⚡ Very Fast, Great for coding
- **codellama**: ⚡⚡⚡ Very Fast, Best for programming
- **mistral**: ⚡⚡⚡ Very Fast, Balanced performance

### **Medium Models (8-12GB VRAM)**
- **nous-hermes2**: ⚡⚡ Fast, High quality
- **qwen:14b**: ⚡⚡ Fast, Excellent reasoning
- **openchat**: ⚡⚡ Fast, Creative writing

### **Large Models (12-16GB VRAM)**
- **llama2:13b**: ⚡⚡ Fast, Good general performance
- **yi:34b**: ⚡ Slower, Maximum quality

## ⚡ **Performance Tips:**

### **For Maximum Speed:**
- Use fast models (dolphin-phi, codellama, mistral)
- Keep GPU layers at 35
- Close other GPU applications

### **For Maximum Quality:**
- Use large models (qwen:14b, llama2:13b, yi:34b)
- Increase context length
- Use higher temperature for creativity

### **Memory Management:**
- Monitor with: `./monitor_gpu.sh`
- Large models may need CPU offloading
- Use quantization for 34B+ models

## 🔧 **AMD GPU Commands:**

### **Monitor GPU:**
```bash
rocm-smi                    # GPU info
rocm-smi --showmeminfo     # Memory usage
rocm-smi --showutilization # GPU utilization
```

### **Optimize Performance:**
```bash
export ROCR_VISIBLE_DEVICES=0
export HSA_OVERRIDE_GFX_VERSION=11.0.0
export HIP_VISIBLE_DEVICES=0
```

### **Test Models:**
```bash
# Fast test
ollama run dolphin-phi "Hello" --gpu-layers 35

# Medium test
ollama run nous-hermes2 "Write a story" --gpu-layers 35

# Large test (use with caution)
ollama run qwen:14b "Explain AI" --gpu-layers 35
```

## 🎯 **Recommended Workflow:**

1. **Start with fast models** for quick responses
2. **Use medium models** for quality tasks
3. **Reserve large models** for complex reasoning
4. **Monitor GPU memory** to avoid OOM errors
5. **Use quantization** for 34B+ models if needed

## ⚠️ **Memory Warnings:**

- **16GB VRAM** is excellent for most models
- **34B models** may need CPU offloading
- **Monitor memory** with `./monitor_gpu.sh`
- **Close other apps** when using large models

Your 7800XT is perfect for uncensored AI! Enjoy the performance! 🚀
EOF
print_success "AMD GPU usage guide created"

# Final status
echo ""
echo "================================================================"
print_success "🎉 AMD 7800XT GPU-Optimized Setup Completed!"
echo ""
print_gpu "Your GPU is now optimized for uncensored AI models!"
echo ""
print_status "Next steps:"
echo "1. Start AMD-optimized server: ./start_server_amd.sh"
echo "2. Monitor GPU: ./monitor_gpu.sh"
echo "3. Test performance: ./test_gpu_performance.sh"
echo "4. Read AMD_USAGE.md for detailed instructions"
echo ""
print_warning "Remember to use responsibly and ethically!"
echo ""
print_gpu "Your 16GB 7800XT is perfect for uncensored AI! 🚀"