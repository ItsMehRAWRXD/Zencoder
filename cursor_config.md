# Cursor + Ollama Configuration Guide

This guide explains how to configure Cursor to use Ollama with uncensored AI models.

## Prerequisites

1. Ollama installed and running (use `./install_ollama.sh`)
2. Uncensored models downloaded
3. Cursor IDE installed

## Configuration Steps

### 1. Enable Ollama in Cursor Settings

Open Cursor settings (`Ctrl+,` or `Cmd+,`) and navigate to:
- **Extensions** → **Continue** → **Settings**

Or edit your settings JSON directly:

```json
{
  "continue.modelProvider": "ollama",
  "continue.ollamaUrl": "http://localhost:11434",
  "continue.models": [
    {
      "name": "dolphin-llama3:8b",
      "provider": "ollama",
      "model": "dolphin-llama3:8b"
    },
    {
      "name": "dolphin-mixtral:8x7b", 
      "provider": "ollama",
      "model": "dolphin-mixtral:8x7b"
    },
    {
      "name": "wizard-vicuna-uncensored:13b",
      "provider": "ollama", 
      "model": "wizard-vicuna-uncensored:13b"
    },
    {
      "name": "nous-hermes2:10.7b",
      "provider": "ollama",
      "model": "nous-hermes2:10.7b"
    }
  ]
}
```

### 2. Alternative: Use Continue Extension

1. Install the **Continue** extension in Cursor
2. Open the Continue sidebar (`Ctrl+Shift+M`)
3. Click the gear icon for settings
4. Add Ollama configuration:

```json
{
  "models": [
    {
      "title": "Dolphin Llama3 8B",
      "provider": "ollama",
      "model": "dolphin-llama3:8b",
      "apiBase": "http://localhost:11434"
    },
    {
      "title": "Dolphin Mixtral 8x7B",
      "provider": "ollama", 
      "model": "dolphin-mixtral:8x7b",
      "apiBase": "http://localhost:11434"
    },
    {
      "title": "Wizard Vicuna Uncensored 13B",
      "provider": "ollama",
      "model": "wizard-vicuna-uncensored:13b", 
      "apiBase": "http://localhost:11434"
    }
  ],
  "tabAutocompleteModel": {
    "title": "Dolphin Code",
    "provider": "ollama",
    "model": "dolphin-llama3:8b",
    "apiBase": "http://localhost:11434"
  }
}
```

### 3. Manual Cursor Settings Configuration

Create or edit `~/.cursor/User/settings.json`:

```json
{
  "cursor.chat.model": "ollama/dolphin-llama3:8b",
  "cursor.chat.ollamaBaseUrl": "http://localhost:11434",
  "cursor.autocomplete.model": "ollama/dolphin-llama3:8b",
  "cursor.autocomplete.ollamaBaseUrl": "http://localhost:11434"
}
```

## Model Recommendations

### For Coding Tasks
- **dolphin-llama3:8b** - Great balance of speed and capability
- **nous-hermes2:10.7b** - Excellent for code generation

### For Complex Reasoning  
- **dolphin-mixtral:8x7b** - More powerful but slower
- **wizard-vicuna-uncensored:13b** - Very capable but requires more RAM

### For Quick Autocomplete
- **dolphin-llama3:8b** - Fast and responsive

## Testing Your Setup

1. Open Cursor
2. Create a new file (e.g., `test.py`)
3. Use `Ctrl+K` (or `Cmd+K`) to open the AI chat
4. Ask a question like: "Write a Python function to calculate fibonacci numbers"
5. The AI should respond using your local Ollama model

## Troubleshooting

### Ollama Not Running
```bash
sudo systemctl status ollama
sudo systemctl start ollama
```

### Check Available Models
```bash
ollama list
```

### Test Ollama Directly
```bash
curl http://localhost:11434/api/generate -d '{
  "model": "dolphin-llama3:8b",
  "prompt": "Why is the sky blue?",
  "stream": false
}'
```

### Cursor Not Connecting
1. Verify Ollama is running on port 11434
2. Check firewall settings
3. Restart Cursor after configuration changes

## Benefits of This Setup

✅ **Complete Privacy** - All AI processing happens locally  
✅ **No Censorship** - Use uncensored models for unrestricted assistance  
✅ **No API Costs** - Free to use once set up  
✅ **Offline Capable** - Works without internet connection  
✅ **Customizable** - Add any Ollama-compatible model  

## Security Notes

- These models run locally and don't send data to external servers
- Uncensored models can generate any type of content - use responsibly
- Consider the ethical implications of the code you're developing

## Next Steps

- Try the example scripts in `examples/`
- Experiment with different models for different tasks
- Customize the configuration based on your hardware capabilities