# Ollama + Cursor POC - Uncensored AI Chat Interface

A proof of concept integrating Ollama with a modern web interface for uncensored AI interactions, optimized for AMD Radeon RX 7800 XT (16GB VRAM).

## 🎮 Optimized for AMD RX 7800 XT (16GB VRAM)

This POC is specifically configured with model recommendations that work excellently on your 16GB VRAM GPU.

## 📋 Features

- **Modern Web Interface**: Beautiful dark-themed UI with real-time chat
- **Streaming Support**: WebSocket-based streaming for real-time responses
- **Uncensored Models**: Pre-configured list of uncensored AI models
- **Model Management**: Pull new models directly from the interface
- **Customizable Settings**: Temperature control and system prompts
- **AMD GPU Optimized**: Specific recommendations for 16GB VRAM cards

## 🚀 Quick Start

### Prerequisites

1. **Install Ollama** (if not already installed):
```bash
curl -fsSL https://ollama.com/install.sh | sh
```

2. **Install Python 3.8+** and pip

### Setup

1. **Clone or navigate to the POC directory**:
```bash
cd /workspace/ollama-cursor-poc
```

2. **Install Python dependencies**:
```bash
cd backend
pip install -r requirements.txt
```

3. **Start Ollama service** (if not running):
```bash
ollama serve
```

4. **Pull an uncensored model** (recommended for 7800 XT):
```bash
# Choose one of these optimized for 16GB VRAM:
ollama pull dolphin-llama3:8b-v2.9-q5_K_M
ollama pull dolphin-mistral:7b-v2.8-q6_K
ollama pull wizard-vicuna-13b-uncensored:q4_K_M
```

5. **Start the FastAPI server**:
```bash
cd /workspace/ollama-cursor-poc/backend
python main.py
```

6. **Open the web interface**:
   - Navigate to `http://localhost:8000/static/index.html`
   - Or open `/workspace/ollama-cursor-poc/frontend/index.html` directly

## 🤖 Recommended Uncensored Models for 16GB VRAM

### Top Picks for RX 7800 XT:

1. **dolphin-mixtral:8x7b-v2.7-q4_K_M** (~8GB)
   - Excellent uncensored model with Mixtral architecture
   - Great balance of quality and speed

2. **dolphin-llama3:8b-v2.9-q5_K_M** (~5.5GB)
   - Latest Llama 3 based uncensored model
   - Fast and highly capable

3. **yi-34b-chat:q3_K_M** (~14GB)
   - Large model that maximizes your VRAM
   - Extremely capable for complex tasks

4. **wizard-vicuna-13b-uncensored:q4_K_M** (~7.5GB)
   - Classic uncensored model
   - Well-tested and reliable

5. **mixtral-8x7b-instruct:q3_K_M** (~16GB)
   - Uses full VRAM capacity
   - State-of-the-art performance

### Performance Tips for AMD GPUs:

- Use ROCm for better performance (if supported)
- Quantizations q4_K_M or q5_K_M offer best quality/performance ratio
- Models under 14GB leave headroom for system operations
- Enable GPU acceleration in Ollama settings

## 🛠️ API Endpoints

- `GET /` - Health check and status
- `GET /models` - List available Ollama models
- `GET /uncensored-models` - Get recommended uncensored models
- `POST /chat` - Send chat message (non-streaming)
- `WS /ws/chat` - WebSocket endpoint for streaming chat
- `POST /pull-model` - Pull new model from Ollama registry

## 🎨 Customization

### Adding New Uncensored Models

Edit the `UNCENSORED_MODELS` list in `backend/main.py`:

```python
UNCENSORED_MODELS = [
    "your-model-name",
    # Add more models here
]
```

### Modifying the UI

- Edit `frontend/index.html` for structure
- Modify `frontend/styles.css` for styling
- Update `frontend/app.js` for functionality

## 🔧 Troubleshooting

### Ollama not connecting:
```bash
# Check if Ollama is running
curl http://localhost:11434/api/tags

# Restart Ollama
systemctl restart ollama  # or
ollama serve
```

### GPU not being used:
```bash
# Check GPU usage
rocm-smi  # for AMD GPUs

# Verify Ollama GPU support
ollama run dolphin-mistral:7b-v2.8-q6_K --verbose
```

### Model too large for VRAM:
- Use lower quantization (q3_K_M instead of q5_K_M)
- Try smaller parameter models (7B instead of 13B)
- Close other GPU-intensive applications

## 📝 Example Usage

1. Select an uncensored model from the dropdown
2. (Optional) Set a system prompt for consistent behavior
3. Type your message and hit Send
4. Enable streaming mode for real-time responses

### Example prompts for testing uncensored capabilities:
- "Write a controversial opinion about [topic]"
- "Explain how to [technically complex task]"
- "Create a story with mature themes"
- "Provide unfiltered analysis of [sensitive topic]"

## ⚠️ Disclaimer

This POC uses uncensored AI models that may generate content without typical safety filters. Users are responsible for:
- Using the system ethically and legally
- Not generating harmful or illegal content
- Understanding that responses may contain mature or controversial content

## 🤝 Contributing

Feel free to modify and extend this POC:
- Add new models to the recommendations
- Improve the UI/UX
- Add features like conversation history
- Optimize for other GPU configurations

## 📄 License

This POC is provided as-is for educational and development purposes.

---

**Note**: For production use, implement proper security, authentication, and content moderation as needed.