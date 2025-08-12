#!/bin/bash

# Ollama + Uncensored AI Setup Script
# This script installs Ollama and sets up uncensored AI models for use with Cursor

set -e

echo "🚀 Setting up Ollama with uncensored AI models..."

# Check if running on Linux
if [[ "$OSTYPE" != "linux-gnu"* ]]; then
    echo "❌ This script is designed for Linux. Please adapt for your OS."
    exit 1
fi

# Install Ollama
echo "📦 Installing Ollama..."
if ! command -v ollama &> /dev/null; then
    curl -fsSL https://ollama.com/install.sh | sh
    echo "✅ Ollama installed successfully"
else
    echo "✅ Ollama is already installed"
fi

# Start Ollama service
echo "🔄 Starting Ollama service..."
sudo systemctl enable ollama
sudo systemctl start ollama

# Wait for Ollama to be ready
echo "⏳ Waiting for Ollama to be ready..."
sleep 5

# Function to pull a model with retry logic
pull_model() {
    local model=$1
    local max_attempts=3
    local attempt=1
    
    echo "🔄 Pulling model: $model"
    
    while [ $attempt -le $max_attempts ]; do
        if ollama pull $model; then
            echo "✅ Successfully pulled $model"
            return 0
        else
            echo "❌ Attempt $attempt failed for $model"
            ((attempt++))
            if [ $attempt -le $max_attempts ]; then
                echo "🔄 Retrying in 10 seconds..."
                sleep 10
            fi
        fi
    done
    
    echo "❌ Failed to pull $model after $max_attempts attempts"
    return 1
}

# Pull uncensored models
echo "🧠 Pulling uncensored AI models..."

# Dolphin models (uncensored Llama variants)
pull_model "dolphin-llama3:8b"
pull_model "dolphin-mixtral:8x7b"

# WizardLM uncensored
pull_model "wizard-vicuna-uncensored:13b"

# Other popular uncensored models
pull_model "nous-hermes2:10.7b"
pull_model "openhermes:7b"

echo "🎉 Ollama setup complete!"
echo ""
echo "📋 Available models:"
ollama list
echo ""
echo "🔧 Next steps:"
echo "1. Configure Cursor to use Ollama (see cursor_config.md)"
echo "2. Test the setup with the provided examples"
echo "3. Start coding with uncensored AI assistance!"