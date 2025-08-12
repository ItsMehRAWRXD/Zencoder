# Ollama + Cursor + Uncensored AI POC

A complete proof-of-concept setup for running **uncensored AI models locally** with **Ollama** and integrating them with **Cursor IDE** for private, unrestricted coding assistance.

## 🎯 What This Provides

✅ **Complete Privacy** - All AI processing happens locally on your machine  
✅ **No Censorship** - Use uncensored models for unrestricted assistance  
✅ **No API Costs** - Free to use once set up  
✅ **Offline Capable** - Works without internet connection  
✅ **Production Ready** - Automated setup and configuration  

## 🚀 Quick Start

### Option 1: Automated Setup (Recommended)

```bash
# Make the quick start script executable
chmod +x examples/quick_start.sh

# Run the automated setup
./examples/quick_start.sh
```

This script will:
- Install Ollama
- Download uncensored AI models
- Configure Cursor IDE
- Set up Continue extension
- Test the entire setup

### Option 2: Manual Setup

```bash
# 1. Install Ollama and models
chmod +x install_ollama.sh
./install_ollama.sh

# 2. Follow configuration guide
# See cursor_config.md for detailed instructions
```

## 📋 What Gets Installed

### Uncensored AI Models

| Model | Size | Best For | RAM Required |
|-------|------|----------|--------------|
| **dolphin-llama3:8b** | ~4.7GB | Fast coding, autocomplete | 8GB+ |
| **nous-hermes2:10.7b** | ~6.3GB | Balanced performance | 8GB+ |
| **openhermes:7b** | ~4.1GB | Quick responses | 6GB+ |
| **dolphin-mixtral:8x7b** | ~26GB | Complex reasoning | 16GB+ |
| **wizard-vicuna-uncensored:13b** | ~7.3GB | Advanced tasks | 16GB+ |

### Cursor Integration

- **Chat Interface** - Use `Ctrl+K` to chat with AI
- **Inline Editing** - Use `Ctrl+I` for code suggestions
- **Autocomplete** - Tab completion powered by local AI
- **Custom Commands** - Explain, debug, optimize, test code

## 🔧 Usage Examples

### In Cursor IDE

1. **Chat with AI** - Press `Ctrl+K` (or `Cmd+K`)
   ```
   You: "Create a REST API for user authentication"
   AI: [Provides complete, uncensored code]
   ```

2. **Inline Code Generation** - Press `Ctrl+I` (or `Cmd+I`)
   ```python
   # Select code and press Ctrl+I
   def fibonacci(n):
       # AI will complete this function
   ```

3. **Code Explanation** - Select code and use `/explain`
4. **Debug Code** - Select code and use `/debug`
5. **Generate Tests** - Select code and use `/test`

### Programmatic Usage

```python
# Test the setup
python3 examples/test_ollama.py

# Use the OllamaClient class in your projects
from examples.test_ollama import OllamaClient

client = OllamaClient()
response = client.generate("dolphin-llama3:8b", "Write a Python function")
print(response)
```

## 📁 Project Structure

```
ollama-cursor-uncensored-poc/
├── install_ollama.sh              # Basic Ollama setup script
├── cursor_config.md               # Detailed configuration guide
├── README.md                      # This file
└── examples/
    ├── quick_start.sh             # Automated full setup
    ├── test_ollama.py             # Python test script
    └── continue_config.json       # Continue extension config
```

## 🛠️ System Requirements

### Minimum Requirements
- **OS**: Linux (Ubuntu, Debian, CentOS, etc.)
- **RAM**: 8GB (for basic models)
- **Storage**: 20GB free space
- **CPU**: x64 architecture

### Recommended Requirements
- **RAM**: 16GB+ (for larger models)
- **Storage**: 50GB+ free space
- **GPU**: NVIDIA GPU with CUDA (optional, for faster inference)

## 🔍 Troubleshooting

### Common Issues

**Ollama not starting:**
```bash
sudo systemctl status ollama
sudo systemctl restart ollama
```

**Models running slowly:**
```bash
# Use smaller models
ollama pull openhermes:7b
```

**Out of memory:**
```bash
# Check system resources
free -h
htop

# Restart Ollama service
sudo systemctl restart ollama
```

**Cursor not connecting:**
1. Verify Ollama is running: `curl http://localhost:11434/api/tags`
2. Check Cursor settings in `~/.cursor/User/settings.json`
3. Restart Cursor after configuration changes

### Useful Commands

```bash
# Check Ollama status
systemctl status ollama

# List installed models
ollama list

# Test a model directly
ollama run dolphin-llama3:8b "Hello, write Python code"

# View Ollama logs
journalctl -u ollama -f

# Check system resources
free -h && df -h
```

## 🔒 Privacy & Security

### Data Privacy
- **All processing is local** - No data leaves your machine
- **No telemetry** - Configured to disable all tracking
- **Offline capable** - Works without internet connection

### Model Safety
- **Uncensored models** can generate any type of content
- **Use responsibly** - Consider ethical implications
- **Content filtering** - You are responsible for appropriate use

### Security Considerations
- Models run with user permissions (not root)
- API only accessible locally (localhost:11434)
- No external network connections required

## 🎨 Customization

### Adding More Models

```bash
# Browse available models
ollama search llama

# Pull additional models
ollama pull codellama:13b
ollama pull deepseek-coder:6.7b
ollama pull starcoder:15b
```

### Cursor Settings

Edit `~/.cursor/User/settings.json`:
```json
{
    "cursor.chat.model": "ollama/your-preferred-model",
    "cursor.autocomplete.enabled": true,
    "cursor.autocomplete.model": "ollama/fast-model"
}
```

### Continue Extension Config

Edit `~/.continue/config.json` to add custom models, commands, and settings.

## 📈 Performance Tips

### For Better Speed
1. Use smaller models for autocomplete: `openhermes:7b`
2. Use larger models for complex tasks: `dolphin-mixtral:8x7b`
3. Enable GPU acceleration if available
4. Increase system RAM if possible

### For Better Quality
1. Use larger models: `wizard-vicuna-uncensored:13b`
2. Adjust temperature settings in config
3. Use model-specific prompting techniques
4. Fine-tune context length based on needs

## 🤝 Contributing

This is a proof-of-concept project. Feel free to:
- Report issues and bugs
- Suggest improvements
- Add support for other editors
- Contribute additional models or configurations

## ⚖️ Legal & Ethical Notice

- **Educational Purpose**: This POC is for educational and research purposes
- **Responsible Use**: Users are responsible for ethical and legal use
- **Model Licenses**: Respect individual model licenses and terms
- **Content Warning**: Uncensored models can generate inappropriate content

## 📚 Additional Resources

- [Ollama Documentation](https://ollama.com/docs)
- [Cursor IDE Documentation](https://cursor.sh/docs)
- [Continue Extension](https://github.com/continuedev/continue)
- [Model Hub](https://ollama.com/library)

---

**Enjoy your private, uncensored AI coding assistant! 🚀**