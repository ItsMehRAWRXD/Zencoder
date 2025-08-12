# Ollama + Cursor Uncensored AI PoC

This proof of concept demonstrates how to integrate Ollama with uncensored AI models for use with Cursor IDE.

## Features

- **Uncensored AI Models**: Support for various uncensored models like Dolphin, Nous-Hermes, and others
- **Cursor Integration**: Seamless integration with Cursor IDE
- **Local Processing**: All AI processing happens locally via Ollama
- **Custom Prompts**: Support for uncensored and creative prompts
- **API Server**: RESTful API for easy integration
- **Web Interface**: Simple web UI for testing

## Prerequisites

1. **Ollama**: Install from https://ollama.ai/
2. **Python 3.8+**: For the integration scripts
3. **Cursor IDE**: For the IDE integration

## Quick Start

### 1. Install Ollama
```bash
curl -fsSL https://ollama.ai/install.sh | sh
```

### 2. Pull Uncensored Models
```bash
# Pull some popular uncensored models
ollama pull dolphin-phi
ollama pull nous-hermes2
ollama pull codellama
ollama pull llama2-uncensored
```

### 3. Install Python Dependencies
```bash
cd ollama_cursor_poc
pip install -r requirements.txt
```

### 4. Start the API Server
```bash
python api_server.py
```

### 5. Use with Cursor
- Configure Cursor to use the local API endpoint
- Or use the provided Cursor extension

## Models Available

### Uncensored Models
- `dolphin-phi`: Fast, uncensored model
- `nous-hermes2`: High-quality uncensored responses
- `llama2-uncensored`: Uncensored version of Llama2
- `codellama`: Uncensored coding assistant

### Creative Models
- `dolphin-2.6-mistral`: Creative writing and roleplay
- `nous-hermes2-mixtral`: Advanced reasoning and creativity

## Usage Examples

### Basic API Usage
```python
import requests

response = requests.post('http://localhost:8000/generate', json={
    'model': 'dolphin-phi',
    'prompt': 'Write a creative story about...',
    'max_tokens': 500
})
print(response.json()['response'])
```

### Cursor Integration
1. Open Cursor IDE
2. Go to Settings > AI
3. Set custom API endpoint to `http://localhost:8000/generate`
4. Use Ctrl+K for AI assistance

## Configuration

### API Configuration
Edit `config.py` to customize:
- Default models
- Response parameters
- Security settings
- Rate limiting

### Model Parameters
- `temperature`: Controls creativity (0.0-2.0)
- `top_p`: Nucleus sampling (0.0-1.0)
- `max_tokens`: Maximum response length
- `stop_sequences`: Custom stop tokens

## Security Considerations

⚠️ **Important**: This setup provides uncensored AI access. Use responsibly:

1. **Local Only**: All processing happens on your machine
2. **No Logging**: Responses are not logged or stored
3. **Custom Prompts**: Full control over input/output
4. **Rate Limiting**: Built-in protection against abuse

## Troubleshooting

### Common Issues
1. **Ollama not running**: Start with `ollama serve`
2. **Model not found**: Pull the model first with `ollama pull <model>`
3. **Port conflicts**: Change port in `config.py`
4. **Memory issues**: Use smaller models or increase system RAM

### Performance Tips
- Use smaller models for faster responses
- Adjust `max_tokens` based on your needs
- Consider using GPU acceleration if available

## Advanced Features

### Custom Model Training
```bash
# Create custom model with specific parameters
ollama create my-uncensored-model -f Modelfile
```

### Batch Processing
```python
# Process multiple prompts
python batch_processor.py --input prompts.txt --output responses.json
```

### Web Interface
```bash
# Start web UI
python web_interface.py
# Open http://localhost:8080
```

## Contributing

Feel free to contribute improvements:
1. Fork the repository
2. Create a feature branch
3. Submit a pull request

## License

This project is for educational and research purposes. Use responsibly and in accordance with applicable laws and regulations.

## Disclaimer

This tool provides access to uncensored AI models. Users are responsible for:
- Complying with local laws and regulations
- Using the tool ethically and responsibly
- Not generating harmful or illegal content
- Respecting intellectual property rights

The authors are not responsible for any misuse of this software.