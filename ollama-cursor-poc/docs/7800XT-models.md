# Uncensored AI Models for AMD RX 7800 XT (16GB VRAM)

This guide provides detailed recommendations for uncensored AI models optimized for your AMD Radeon RX 7800 XT with 16GB VRAM.

## 🎮 Your GPU Specifications

- **GPU**: AMD Radeon RX 7800 XT
- **VRAM**: 16GB GDDR6
- **Recommended Model Size**: Up to 14GB (leaving 2GB for system overhead)
- **Optimal Quantization**: q4_K_M or q5_K_M for best quality/performance

## 🚀 Top 10 Uncensored Models for 16GB VRAM

### 1. **Dolphin Llama 3** ⭐⭐⭐⭐⭐
```bash
ollama pull dolphin-llama3:8b-v2.9-q5_K_M
```
- **Size**: ~5.5GB
- **Performance**: Excellent
- **Use Case**: General purpose, creative writing, coding
- **Notes**: Latest Llama 3 architecture, extremely fast on your GPU

### 2. **Dolphin Mixtral** ⭐⭐⭐⭐⭐
```bash
ollama pull dolphin-mixtral:8x7b-v2.7-q4_K_M
```
- **Size**: ~8GB (quantized from 47B)
- **Performance**: Outstanding
- **Use Case**: Complex reasoning, multi-language support
- **Notes**: MoE architecture provides excellent quality

### 3. **Yi 34B Chat** ⭐⭐⭐⭐⭐
```bash
ollama pull yi:34b-chat-q3_K_M
```
- **Size**: ~14GB
- **Performance**: Exceptional
- **Use Case**: Long context, complex tasks
- **Notes**: Maximizes your VRAM, extremely capable

### 4. **Wizard Vicuna Uncensored** ⭐⭐⭐⭐
```bash
ollama pull wizard-vicuna:13b-uncensored-q4_K_M
```
- **Size**: ~7.5GB
- **Performance**: Very Good
- **Use Case**: General chat, creative tasks
- **Notes**: Well-tested, reliable uncensored model

### 5. **OpenHermes 2.5 Mistral** ⭐⭐⭐⭐
```bash
ollama pull openhermes2.5-mistral:7b-q5_K_M
```
- **Size**: ~5GB
- **Performance**: Great
- **Use Case**: Technical discussions, coding
- **Notes**: Trained on diverse datasets

### 6. **Dolphin Mistral** ⭐⭐⭐⭐
```bash
ollama pull dolphin-mistral:7b-v2.8-q6_K
```
- **Size**: ~6GB
- **Performance**: Excellent
- **Use Case**: Fast responses, general purpose
- **Notes**: High quality quantization

### 7. **Solar Instruct Uncensored** ⭐⭐⭐⭐
```bash
ollama pull solar:10.7b-instruct-uncensored-q5_K_M
```
- **Size**: ~7.5GB
- **Performance**: Very Good
- **Use Case**: Instruction following, analysis
- **Notes**: Unique 10.7B parameter size

### 8. **Nous Hermes 2 Mixtral DPO** ⭐⭐⭐⭐
```bash
ollama pull nous-hermes2-mixtral:8x7b-dpo-q4_K_M
```
- **Size**: ~8GB
- **Performance**: Excellent
- **Use Case**: High quality responses
- **Notes**: DPO training improves output quality

### 9. **DeepSeek Coder** ⭐⭐⭐⭐
```bash
ollama pull deepseek-coder:33b-instruct-q3_K_M
```
- **Size**: ~13GB
- **Performance**: Exceptional for coding
- **Use Case**: Programming, technical tasks
- **Notes**: Specialized for code generation

### 10. **Mixtral Instruct** ⭐⭐⭐⭐⭐
```bash
ollama pull mixtral:8x7b-instruct-v0.1-q3_K_M
```
- **Size**: ~16GB
- **Performance**: State-of-the-art
- **Use Case**: Any task requiring maximum capability
- **Notes**: Uses full VRAM, cutting-edge performance

## 📊 Performance Optimization Tips

### For AMD GPUs:

1. **Enable ROCm** (if supported):
```bash
# Check ROCm installation
rocm-smi

# Set environment variables
export HSA_OVERRIDE_GFX_VERSION=11.0.0
export OLLAMA_ROCM=1
```

2. **Monitor GPU Usage**:
```bash
# Real-time GPU monitoring
watch -n 1 rocm-smi

# Or use radeontop
sudo apt install radeontop
radeontop
```

3. **Optimize Ollama Settings**:
```bash
# Set GPU layers (use all available)
export OLLAMA_NUM_GPU_LAYERS=999

# Set context size
export OLLAMA_CONTEXT_SIZE=4096
```

## 🔧 Quantization Guide

For your 16GB VRAM:

| Quantization | Quality | Speed | VRAM Usage | Recommendation |
|--------------|---------|-------|-------------|----------------|
| q8_0         | Best    | Slow  | Highest     | Not recommended for large models |
| q6_K         | Excellent | Good | High      | Great for 7B models |
| q5_K_M       | Very Good | Fast | Moderate  | **Best balance** ✅ |
| q4_K_M       | Good    | Faster | Lower     | **Recommended** ✅ |
| q3_K_M       | Acceptable | Very Fast | Lowest | For 30B+ models |

## 🚨 Troubleshooting

### Model Too Large Error:
```bash
# Check available VRAM
rocm-smi --showmeminfo vram

# Try lower quantization
ollama pull model-name:q3_K_M  # instead of q5_K_M
```

### Slow Performance:
1. Close other GPU applications
2. Reduce context size: `export OLLAMA_CONTEXT_SIZE=2048`
3. Use faster quantization (q4_K_M instead of q6_K)

### GPU Not Detected:
```bash
# Verify Ollama GPU support
ollama run dolphin-mistral --verbose

# Check GPU is recognized
lspci | grep -i amd
```

## 📝 Example Configurations

### Maximum Quality Setup (12GB usage):
```bash
ollama pull yi:34b-chat-q3_K_M
# Leaves 4GB for system overhead
```

### Balanced Setup (8GB usage):
```bash
ollama pull dolphin-mixtral:8x7b-v2.7-q4_K_M
# Great quality with room for multitasking
```

### Speed-Optimized Setup (5GB usage):
```bash
ollama pull dolphin-llama3:8b-v2.9-q5_K_M
# Fastest responses, still high quality
```

## 🎯 Use Case Recommendations

- **Creative Writing**: dolphin-llama3:8b-v2.9-q5_K_M
- **Coding**: deepseek-coder:33b-instruct-q3_K_M
- **Complex Analysis**: yi:34b-chat-q3_K_M
- **Multi-language**: dolphin-mixtral:8x7b-v2.7-q4_K_M
- **Fast Chat**: dolphin-mistral:7b-v2.8-q6_K

## 🔗 Additional Resources

- [Ollama Model Library](https://ollama.ai/library)
- [AMD ROCm Documentation](https://rocmdocs.amd.com/)
- [Quantization Explained](https://github.com/ggerganov/llama.cpp/blob/master/examples/quantize/README.md)

---

**Note**: Model availability may change. Use `ollama search uncensored` to find the latest models.