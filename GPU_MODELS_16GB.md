# Uncensored AI Models for 16GB AMD RX 7800 XT

This guide lists all the best **uncensored AI models** that will run excellently on your **16GB AMD RX 7800 XT**. These models are completely unfiltered and will provide unrestricted assistance for coding, writing, and any other tasks.

## 🚀 Quick Setup

```bash
# Run the GPU-optimized setup
chmod +x gpu_optimized_setup.sh
./gpu_optimized_setup.sh
```

## 🎯 Recommended Model Combinations

### For Maximum Performance
- **Autocomplete**: `dolphin-phi:2.7b` (~2GB) - Lightning fast
- **General Chat**: `dolphin-llama3:8b` (~5GB) - Perfect balance
- **Complex Tasks**: `wizard-vicuna-uncensored:13b` (~8GB) - Most capable
- **Coding**: `deepseek-coder:6.7b` (~4GB) - Programming specialist

**Total VRAM Usage**: ~19GB (with model swapping - Ollama handles this automatically)

## 📋 Complete Uncensored Model List

### ⚡ Ultra-Fast Models (2-4GB VRAM)
Perfect for autocomplete and quick responses

| Model | VRAM | Speed | Quality | Best For |
|-------|------|-------|---------|----------|
| `dolphin-phi:2.7b` | ~2GB | 🚀🚀🚀🚀🚀 | ⭐⭐⭐ | Autocomplete, quick tasks |
| `openhermes:7b` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐ | Fast responses, general chat |
| `starling-lm:7b` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐ | High-quality quick responses |
| `neural-chat:7b` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐ | Intel's uncensored model |
| `zephyr:7b-beta` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐ | Mistral-based, less filtered |
| `openchat:7b` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐ | Open-source uncensored |

### 🔥 Balanced Models (5-8GB VRAM)
Great balance of speed and capability

| Model | VRAM | Speed | Quality | Best For |
|-------|------|-------|---------|----------|
| `dolphin-llama3:8b` | ~5GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐⭐ | **BEST OVERALL** - Uncensored Llama3 |
| `nous-hermes2:10.7b` | ~6GB | 🚀🚀🚀 | ⭐⭐⭐⭐⭐ | Excellent for coding |
| `llama2-uncensored:13b` | ~7GB | 🚀🚀🚀 | ⭐⭐⭐⭐ | Classic uncensored Llama2 |
| `vicuna:13b` | ~7GB | 🚀🚀🚀 | ⭐⭐⭐⭐ | Great conversational model |
| `orca-mini:13b` | ~7GB | 🚀🚀🚀 | ⭐⭐⭐⭐ | Microsoft's uncensored variant |

### 🧠 Powerful Models (8-12GB VRAM)
Maximum capability for complex tasks

| Model | VRAM | Speed | Quality | Best For |
|-------|------|-------|---------|----------|
| `wizard-vicuna-uncensored:13b` | ~8GB | 🚀🚀 | ⭐⭐⭐⭐⭐ | **MOST CAPABLE** - Very powerful |
| `llama2-uncensored:70b-q4` | ~10GB | 🚀🚀 | ⭐⭐⭐⭐⭐ | Quantized 70B model |
| `nous-hermes-llama2:13b` | ~8GB | 🚀🚀 | ⭐⭐⭐⭐⭐ | Advanced reasoning |

### 💻 Coding Specialists (4-8GB VRAM)
Specialized for programming tasks

| Model | VRAM | Speed | Quality | Best For |
|-------|------|-------|---------|----------|
| `deepseek-coder:6.7b` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐⭐ | **BEST FOR CODING** |
| `codellama:13b` | ~7GB | 🚀🚀🚀 | ⭐⭐⭐⭐⭐ | Meta's code model |
| `codellama:7b-instruct` | ~4GB | 🚀🚀🚀🚀 | ⭐⭐⭐⭐ | Instruction-tuned coding |
| `phind-codellama:34b-v2` | ~12GB | 🚀🚀 | ⭐⭐⭐⭐⭐ | Advanced coding (if available) |
| `starcoder:15b` | ~9GB | 🚀🚀 | ⭐⭐⭐⭐⭐ | GitHub Copilot alternative |

### 🎭 Experimental & Alternative Models

| Model | VRAM | Description |
|-------|------|-------------|
| `dolphin-mixtral:8x7b` | ~26GB | Uses model sharding - slower but very capable |
| `sqlcoder:7b` | ~4GB | SQL specialist |
| `meditron:7b` | ~4GB | Medical/scientific tasks |
| `yarn-mistral:7b-128k` | ~4GB | Extended context length |

## 🔧 Installation Commands

### Essential Pack (Recommended Start)
```bash
ollama pull dolphin-llama3:8b      # Best overall model
ollama pull dolphin-phi:2.7b       # Ultra-fast autocomplete  
ollama pull deepseek-coder:6.7b    # Coding specialist
ollama pull openhermes:7b          # Quick responses
```

### Power User Pack
```bash
ollama pull wizard-vicuna-uncensored:13b  # Most capable
ollama pull nous-hermes2:10.7b           # Advanced reasoning
ollama pull codellama:13b                # Advanced coding
ollama pull starling-lm:7b               # High quality
```

### Speed Demon Pack
```bash
ollama pull dolphin-phi:2.7b       # Ultra-fast
ollama pull neural-chat:7b         # Fast Intel model
ollama pull zephyr:7b-beta         # Fast Mistral
ollama pull openchat:7b            # Fast open-source
```

### Coding Master Pack
```bash
ollama pull deepseek-coder:6.7b    # Best coding model
ollama pull codellama:13b          # Meta's code model
ollama pull codellama:7b-instruct  # Instruction-tuned
ollama pull starcoder:15b          # GitHub alternative
```

## ⚡ Performance Optimization

### VRAM Management
```bash
# Monitor VRAM usage
watch -n 1 rocm-smi

# Check model sizes
ollama list

# Remove unused models
ollama rm model-name
```

### Speed Optimization Tips

1. **Use appropriate model sizes**:
   - Autocomplete: 2.7B-7B models
   - Chat: 7B-13B models
   - Complex tasks: 13B+ models

2. **Optimize context length**:
   - Short tasks: 2048 tokens
   - Normal tasks: 4096 tokens
   - Long tasks: 8192+ tokens

3. **Temperature settings**:
   - Fast responses: 0.3-0.5
   - Balanced: 0.7
   - Creative: 0.8-1.0

## 🎯 Best Model Combinations for Your 16GB GPU

### Configuration 1: Balanced Performance
```json
{
  "chat": "dolphin-llama3:8b",
  "autocomplete": "dolphin-phi:2.7b", 
  "coding": "deepseek-coder:6.7b",
  "complex": "wizard-vicuna-uncensored:13b"
}
```

### Configuration 2: Maximum Speed
```json
{
  "chat": "openhermes:7b",
  "autocomplete": "dolphin-phi:2.7b",
  "coding": "codellama:7b-instruct", 
  "complex": "nous-hermes2:10.7b"
}
```

### Configuration 3: Maximum Quality
```json
{
  "chat": "wizard-vicuna-uncensored:13b",
  "autocomplete": "openhermes:7b",
  "coding": "codellama:13b",
  "complex": "dolphin-mixtral:8x7b"
}
```

## 🔥 Special Features by Model

### Most Uncensored
1. **dolphin-llama3:8b** - Completely uncensored, best balance
2. **wizard-vicuna-uncensored:13b** - Most powerful uncensored
3. **llama2-uncensored:13b** - Classic uncensored model

### Best for Coding
1. **deepseek-coder:6.7b** - Specialized coding model
2. **codellama:13b** - Meta's advanced code model
3. **starcoder:15b** - GitHub Copilot alternative

### Fastest Response
1. **dolphin-phi:2.7b** - Lightning fast
2. **openhermes:7b** - Quick and capable
3. **neural-chat:7b** - Fast Intel optimization

### Most Capable
1. **wizard-vicuna-uncensored:13b** - Advanced reasoning
2. **nous-hermes2:10.7b** - Excellent problem solving
3. **dolphin-mixtral:8x7b** - Most advanced (slow on 16GB)

## 🛠️ Testing Your Setup

### Quick Test Commands
```bash
# Test basic model
ollama run dolphin-llama3:8b "Write a Python hello world function"

# Test coding model  
ollama run deepseek-coder:6.7b "Create a REST API endpoint"

# Test fast model
ollama run dolphin-phi:2.7b "Quick Python snippet for file reading"

# Monitor GPU usage
rocm-smi -d
```

### Benchmark Script
```bash
# Run the included test script
python3 examples/test_ollama.py
```

## 🎮 Gaming with AI

Your RX 7800 XT can handle both gaming and AI simultaneously:

- **Light gaming + AI**: Use 7B models (~4GB VRAM), leaves 12GB for games
- **Heavy gaming**: Temporarily stop Ollama or use 2.7B models only
- **AI-only mode**: Use multiple 13B models simultaneously

## 🔧 Troubleshooting

### Common Issues
```bash
# If models are slow
sudo systemctl restart ollama

# If out of VRAM
ollama rm unused-model-name

# Check ROCm status
rocm-smi

# Monitor system resources
htop
```

### Performance Issues
- **Slow responses**: Use smaller models or lower context length
- **Out of memory**: Remove unused models or use quantized versions
- **GPU not used**: Check ROCm installation and Ollama configuration

## 🎉 Enjoy Your Uncensored AI Setup!

With your 16GB AMD RX 7800 XT, you have enough power to run multiple uncensored AI models simultaneously. Start with the Essential Pack and experiment with different combinations to find what works best for your workflow.

**Pro Tip**: Use `dolphin-phi:2.7b` for autocomplete and `wizard-vicuna-uncensored:13b` for complex tasks - this combination gives you both speed and power!