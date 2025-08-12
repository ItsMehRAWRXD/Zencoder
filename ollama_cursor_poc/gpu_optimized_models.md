# GPU-Optimized Uncensored AI Models for 16GB 7800XT

## 🎯 **Your GPU Specs:**
- **GPU**: AMD Radeon RX 7800 XT
- **VRAM**: 16GB GDDR6
- **Architecture**: RDNA 3
- **Performance**: Excellent for AI inference

## 🚀 **Recommended Uncensored Models**

### **High-Performance Models (7B-13B Parameters)**

#### **1. Dolphin Models (Fast & Uncensored)**
```bash
# Fast, uncensored, great for coding
ollama pull dolphin-phi:latest
ollama pull dolphin-2.6-mistral:latest
ollama pull dolphin-2.7-mixtral:latest

# Creative writing and roleplay
ollama pull dolphin-2.6-mistral:latest
```

**VRAM Usage**: ~8-12GB
**Speed**: Very Fast
**Best For**: Coding, general tasks, creative writing

#### **2. Nous-Hermes Models (High Quality)**
```bash
# High-quality uncensored responses
ollama pull nous-hermes2:latest
ollama pull nous-hermes2-mixtral:latest
ollama pull nous-hermes2-yi:latest
```

**VRAM Usage**: ~10-14GB
**Speed**: Fast
**Best For**: Complex reasoning, detailed responses

#### **3. CodeLlama Models (Coding Focused)**
```bash
# Uncensored coding assistant
ollama pull codellama:latest
ollama pull codellama:13b
ollama pull codellama:python
ollama pull codellama:instruct
```

**VRAM Usage**: ~8-12GB
**Speed**: Very Fast
**Best For**: Programming, code generation

### **Advanced Models (13B-70B Parameters)**

#### **4. Llama2 Uncensored Variants**
```bash
# Uncensored Llama2 models
ollama pull llama2-uncensored:latest
ollama pull llama2:13b
ollama pull llama2:70b
```

**VRAM Usage**: 13B (~12-14GB), 70B (~16GB+)
**Speed**: 13B (Fast), 70B (Slower)
**Best For**: General tasks, complex reasoning

#### **5. Mistral Models (High Quality)**
```bash
# High-performance uncensored models
ollama pull mistral:latest
ollama pull mistral:7b
ollama pull mistral:instruct
```

**VRAM Usage**: ~8-10GB
**Speed**: Very Fast
**Best For**: General tasks, instruction following

#### **6. Yi Models (Chinese-English Bilingual)**
```bash
# Bilingual uncensored models
ollama pull yi:latest
ollama pull yi:34b
ollama pull yi:chat
```

**VRAM Usage**: 6B (~6-8GB), 34B (~16GB+)
**Speed**: 6B (Very Fast), 34B (Slower)
**Best For**: Bilingual tasks, complex reasoning

### **Specialized Uncensored Models**

#### **7. Neural Chat Models**
```bash
# Uncensored chat models
ollama pull neural-chat:latest
ollama pull neural-chat:7b
```

**VRAM Usage**: ~8-10GB
**Speed**: Fast
**Best For**: Conversational AI, roleplay

#### **8. OpenChat Models**
```bash
# Open-source uncensored chat
ollama pull openchat:latest
ollama pull openchat:3.5
```

**VRAM Usage**: ~8-10GB
**Speed**: Fast
**Best For**: Chat, roleplay, creative writing

#### **9. Qwen Models (Alibaba)**
```bash
# High-quality uncensored models
ollama pull qwen:latest
ollama pull qwen:7b
ollama pull qwen:14b
```

**VRAM Usage**: 7B (~8-10GB), 14B (~12-14GB)
**Speed**: Fast
**Best For**: General tasks, coding, reasoning

## 🎯 **Optimal Model Selection for Your Use Case**

### **For Coding & Development:**
```bash
# Primary: Fast and accurate
ollama pull codellama:latest
ollama pull dolphin-phi:latest

# Secondary: More detailed responses
ollama pull nous-hermes2:latest
ollama pull qwen:14b
```

### **For Creative Writing & Roleplay:**
```bash
# Primary: Creative and uncensored
ollama pull dolphin-2.6-mistral:latest
ollama pull openchat:latest

# Secondary: High-quality responses
ollama pull nous-hermes2-mixtral:latest
ollama pull neural-chat:latest
```

### **For General Tasks & Reasoning:**
```bash
# Primary: Balanced performance
ollama pull mistral:latest
ollama pull qwen:14b

# Secondary: Advanced reasoning
ollama pull nous-hermes2:latest
ollama pull yi:34b
```

## ⚡ **Performance Optimization for 7800XT**

### **1. Enable GPU Acceleration**
```bash
# Set environment variables for AMD GPU
export ROCR_VISIBLE_DEVICES=0
export HSA_OVERRIDE_GFX_VERSION=11.0.0
```

### **2. Optimize Model Loading**
```bash
# Use quantization for larger models
ollama pull llama2:13b-q4_K_M
ollama pull codellama:13b-q4_K_M
ollama pull nous-hermes2-mixtral-q4_K_M
```

### **3. Memory Management**
```bash
# Monitor GPU memory usage
nvidia-smi  # or equivalent for AMD
rocm-smi   # for AMD GPUs
```

## 🔧 **Installation Commands**

### **Quick Setup for Your GPU:**
```bash
# Install Ollama
curl -fsSL https://ollama.ai/install.sh | sh

# Pull recommended models for 16GB VRAM
ollama pull dolphin-phi:latest
ollama pull codellama:latest
ollama pull nous-hermes2:latest
ollama pull mistral:latest
ollama pull qwen:14b
ollama pull openchat:latest

# Optional: Larger models (use with caution)
ollama pull llama2:13b
ollama pull yi:34b
```

### **Model-Specific Optimizations:**
```bash
# For maximum performance
ollama pull dolphin-phi:latest --gpu-layers 35
ollama pull codellama:latest --gpu-layers 35
ollama pull nous-hermes2:latest --gpu-layers 35

# For memory efficiency
ollama pull dolphin-phi:latest --gpu-layers 20
ollama pull codellama:latest --gpu-layers 20
```

## 📊 **Performance Comparison**

| Model | Size | VRAM Usage | Speed | Quality | Best Use |
|-------|------|------------|-------|---------|----------|
| dolphin-phi | 2.7B | ~4GB | ⚡⚡⚡ | ⭐⭐⭐ | Coding, Fast |
| codellama | 7B | ~8GB | ⚡⚡⚡ | ⭐⭐⭐⭐ | Programming |
| nous-hermes2 | 7B | ~10GB | ⚡⚡ | ⭐⭐⭐⭐⭐ | Reasoning |
| mistral | 7B | ~8GB | ⚡⚡⚡ | ⭐⭐⭐⭐ | General |
| qwen:14b | 14B | ~12GB | ⚡⚡ | ⭐⭐⭐⭐⭐ | Complex Tasks |
| llama2:13b | 13B | ~12GB | ⚡⚡ | ⭐⭐⭐⭐ | General |
| yi:34b | 34B | ~16GB+ | ⚡ | ⭐⭐⭐⭐⭐ | Advanced |

## 🎯 **Recommended Starting Models**

### **For Immediate Use:**
1. **`dolphin-phi`** - Fast, uncensored, great for coding
2. **`codellama`** - Best for programming tasks
3. **`nous-hermes2`** - High-quality responses
4. **`mistral`** - Balanced performance

### **For Advanced Use:**
1. **`qwen:14b`** - Excellent reasoning capabilities
2. **`llama2:13b`** - Good general performance
3. **`yi:34b`** - Maximum quality (if memory allows)

## ⚠️ **Memory Management Tips**

### **Monitor Your GPU Memory:**
```bash
# Install ROCm tools for AMD
sudo apt install rocm-smi

# Monitor GPU usage
rocm-smi
```

### **Optimize Model Loading:**
- Start with smaller models (7B-13B)
- Use quantization for larger models
- Close other GPU-intensive applications
- Monitor memory usage during inference

### **Fallback Strategy:**
If a model doesn't fit in VRAM:
1. Try a smaller variant
2. Use CPU offloading
3. Reduce batch size
4. Use quantization

## 🚀 **Quick Start Commands**

```bash
# 1. Install Ollama
curl -fsSL https://ollama.ai/install.sh | sh

# 2. Pull your first models
ollama pull dolphin-phi:latest
ollama pull codellama:latest

# 3. Test with our PoC
cd ollama_cursor_poc
./setup_script.sh

# 4. Start the server
./start_server.sh

# 5. Test in Cursor IDE
# Configure Cursor to use: http://localhost:8000/v1/chat/completions
```

Your 16GB 7800XT is perfect for running most uncensored AI models efficiently. Start with the 7B-13B models for the best balance of performance and quality!