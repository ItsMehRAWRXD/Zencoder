#!/bin/bash

# Advanced Coding Models Setup for AMD RX 7800 XT (16GB)
# This script sets up the most powerful coding models available

set -e

echo "🚀 Advanced Coding Models Setup for RX 7800 XT"
echo "=============================================="

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_gpu() { echo -e "${PURPLE}[GPU]${NC} $1"; }
print_model() { echo -e "${CYAN}[MODEL]${NC} $1"; }

# Check if Ollama is running
check_ollama() {
    print_status "Checking Ollama service..."
    
    if ! systemctl is-active ollama >/dev/null 2>&1; then
        print_warning "Ollama is not running. Starting it..."
        sudo systemctl start ollama
        sleep 5
    fi
    
    if curl -s http://localhost:11434/api/tags >/dev/null 2>&1; then
        print_success "Ollama is ready!"
    else
        print_error "Ollama is not responding. Please check the service."
        exit 1
    fi
}

# Function to check VRAM usage
check_vram() {
    if command -v rocm-smi >/dev/null 2>&1; then
        echo ""
        print_gpu "Current VRAM usage:"
        rocm-smi --showmeminfo vram
        echo ""
    fi
}

# Download models with progress and VRAM monitoring
download_model() {
    local model=$1
    local description=$2
    local vram_usage=$3
    
    print_model "Downloading: $model"
    echo "Description: $description"
    echo "Expected VRAM: $vram_usage"
    echo ""
    
    if ollama list | grep -q "^$model"; then
        print_success "$model is already installed"
        return 0
    fi
    
    # Check available space
    available_space=$(df /tmp | awk 'NR==2{print $4}')
    available_space_gb=$((available_space / 1024 / 1024))
    
    if [ $available_space_gb -lt 20 ]; then
        print_warning "Low disk space: ${available_space_gb}GB available"
        read -p "Continue anyway? (y/N): " continue_download
        if [[ ! $continue_download =~ ^[Yy]$ ]]; then
            return 1
        fi
    fi
    
    print_status "Downloading $model... This may take a while."
    
    # Download with timeout and retry
    local max_attempts=3
    local attempt=1
    
    while [ $attempt -le $max_attempts ]; do
        if timeout 3600 ollama pull "$model"; then
            print_success "✅ $model downloaded successfully!"
            
            # Test the model quickly
            print_status "Testing $model..."
            local test_response
            test_response=$(timeout 30 ollama run "$model" "print('hello world')" 2>/dev/null || echo "timeout")
            
            if [[ "$test_response" == *"timeout"* ]]; then
                print_warning "⚠️  $model loaded but response was slow"
            else
                print_success "🚀 $model is working perfectly!"
            fi
            
            check_vram
            return 0
        else
            print_warning "Attempt $attempt failed for $model"
            ((attempt++))
            if [ $attempt -le $max_attempts ]; then
                print_status "Retrying in 10 seconds..."
                sleep 10
            fi
        fi
    done
    
    print_error "❌ Failed to download $model after $max_attempts attempts"
    return 1
}

# Main model downloads
download_advanced_models() {
    print_gpu "Downloading advanced coding models optimized for 16GB VRAM..."
    echo ""
    
    # Array of models with descriptions and VRAM usage
    declare -A models=(
        ["deepseek-coder-v2:16b"]="Latest DeepSeek Coder V2 - Most advanced coding model|~10GB"
        ["qwen2.5-coder:32b"]="Qwen 2.5 Coder 32B - Massive coding capabilities|~18GB (uses model sharding)"
        ["codestral"]="Mistral's coding specialist - Excellent for multiple languages|~12GB"
        ["starcoder2:15b"]="StarCoder2 15B - GitHub trained, excellent code completion|~9GB"
        ["codellama:13b-code"]="CodeLlama Code variant - Optimized for pure code generation|~7GB"
        ["vicuna"]="Vicuna latest - Great conversational coding assistant|~7GB"
    )
    
    echo "Available models for download:"
    echo "==============================="
    local i=1
    for model in "${!models[@]}"; do
        IFS='|' read -r description vram <<< "${models[$model]}"
        echo "$i. $model"
        echo "   📝 $description"
        echo "   💾 $vram"
        echo ""
        ((i++))
    done
    
    read -p "Download all models? (y/N): " download_all
    
    if [[ $download_all =~ ^[Yy]$ ]]; then
        # Download all models
        for model in "${!models[@]}"; do
            IFS='|' read -r description vram <<< "${models[$model]}"
            download_model "$model" "$description" "$vram"
            sleep 2  # Brief pause between downloads
        done
    else
        # Interactive selection
        echo ""
        echo "Select models to download:"
        echo "1. Essential Coding Pack (deepseek-coder-v2:16b, codestral, starcoder2:15b)"
        echo "2. Power Pack (qwen2.5-coder:32b, deepseek-coder-v2:16b)"
        echo "3. Balanced Pack (codestral, starcoder2:15b, codellama:13b-code, vicuna)"
        echo "4. Custom selection"
        
        read -p "Choose pack (1-4): " pack_choice
        
        case $pack_choice in
            1)
                download_model "deepseek-coder-v2:16b" "Latest DeepSeek Coder V2" "~10GB"
                download_model "codestral" "Mistral's coding specialist" "~12GB"
                download_model "starcoder2:15b" "StarCoder2 15B" "~9GB"
                ;;
            2)
                download_model "deepseek-coder-v2:16b" "Latest DeepSeek Coder V2" "~10GB"
                download_model "qwen2.5-coder:32b" "Qwen 2.5 Coder 32B" "~18GB"
                ;;
            3)
                download_model "codestral" "Mistral's coding specialist" "~12GB"
                download_model "starcoder2:15b" "StarCoder2 15B" "~9GB"
                download_model "codellama:13b-code" "CodeLlama Code variant" "~7GB"
                download_model "vicuna" "Vicuna conversational" "~7GB"
                ;;
            4)
                for model in "${!models[@]}"; do
                    IFS='|' read -r description vram <<< "${models[$model]}"
                    read -p "Download $model? (y/N): " choice
                    if [[ $choice =~ ^[Yy]$ ]]; then
                        download_model "$model" "$description" "$vram"
                    fi
                done
                ;;
        esac
    fi
}

# Create custom DeepSeek model with extended context
create_custom_models() {
    print_status "Creating custom models..."
    
    # Check if deepseek-coder-v2:16b exists
    if ! ollama list | grep -q "deepseek-coder-v2:16b"; then
        print_warning "deepseek-coder-v2:16b not found. Please download it first."
        return 1
    fi
    
    # Create the custom model with extended context
    print_model "Creating deepseek-coder-v2-32k with extended context..."
    
    if [ -f "modelfiles/deepseek-coder-v2-16b-32k.Modelfile" ]; then
        if ollama create deepseek-coder-v2-32k -f modelfiles/deepseek-coder-v2-16b-32k.Modelfile; then
            print_success "✅ deepseek-coder-v2-32k created successfully!"
            print_status "This model has 32k context window for large codebases"
        else
            print_error "❌ Failed to create deepseek-coder-v2-32k"
        fi
    else
        print_error "Modelfile not found: modelfiles/deepseek-coder-v2-16b-32k.Modelfile"
    fi
}

# Update Cursor configuration for new models
update_cursor_config() {
    print_status "Updating Cursor configuration for advanced models..."
    
    CONTINUE_CONFIG_DIR="$HOME/.continue"
    mkdir -p "$CONTINUE_CONFIG_DIR"
    
    # Backup existing config
    if [ -f "$CONTINUE_CONFIG_DIR/config.json" ]; then
        cp "$CONTINUE_CONFIG_DIR/config.json" "$CONTINUE_CONFIG_DIR/config.json.backup.$(date +%s)"
    fi
    
    # Create advanced configuration
    cat > "$CONTINUE_CONFIG_DIR/config.json" << 'EOF'
{
  "models": [
    {
      "title": "🚀 DeepSeek Coder V2 16B (Latest)",
      "provider": "ollama",
      "model": "deepseek-coder-v2:16b",
      "apiBase": "http://localhost:11434",
      "contextLength": 16384,
      "completionOptions": {
        "temperature": 0.1,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "🔥 DeepSeek V2 32K Context",
      "provider": "ollama",
      "model": "deepseek-coder-v2-32k",
      "apiBase": "http://localhost:11434",
      "contextLength": 32768,
      "completionOptions": {
        "temperature": 0.1,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "⚡ Codestral (Mistral Coding)",
      "provider": "ollama",
      "model": "codestral",
      "apiBase": "http://localhost:11434",
      "contextLength": 32768,
      "completionOptions": {
        "temperature": 0.2,
        "top_p": 0.9,
        "num_gpu": 99
      }
    },
    {
      "title": "🌟 StarCoder2 15B",
      "provider": "ollama",
      "model": "starcoder2:15b",
      "apiBase": "http://localhost:11434",
      "contextLength": 16384,
      "completionOptions": {
        "temperature": 0.2,
        "top_p": 0.9,
        "num_gpu": 99
      }
    },
    {
      "title": "🦙 CodeLlama 13B Code",
      "provider": "ollama",
      "model": "codellama:13b-code",
      "apiBase": "http://localhost:11434",
      "contextLength": 16384,
      "completionOptions": {
        "temperature": 0.1,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "🔮 Qwen2.5 Coder 32B (Powerful)",
      "provider": "ollama",
      "model": "qwen2.5-coder:32b",
      "apiBase": "http://localhost:11434",
      "contextLength": 32768,
      "completionOptions": {
        "temperature": 0.1,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "💬 Vicuna (Conversational)",
      "provider": "ollama",
      "model": "vicuna",
      "apiBase": "http://localhost:11434",
      "contextLength": 4096,
      "completionOptions": {
        "temperature": 0.7,
        "top_p": 0.9,
        "num_gpu": 99
      }
    }
  ],
  "tabAutocompleteModel": {
    "title": "🚀 DeepSeek V2 Autocomplete",
    "provider": "ollama",
    "model": "deepseek-coder-v2:16b",
    "apiBase": "http://localhost:11434",
    "completionOptions": {
      "temperature": 0.1,
      "max_tokens": 100,
      "num_gpu": 99
    }
  },
  "customCommands": [
    {
      "name": "review",
      "prompt": "Review this code for bugs, performance issues, and best practices. Provide specific suggestions for improvement:",
      "description": "Comprehensive code review"
    },
    {
      "name": "optimize",
      "prompt": "Optimize this code for better performance, memory usage, and maintainability:",
      "description": "Optimize code performance"
    },
    {
      "name": "security",
      "prompt": "Analyze this code for security vulnerabilities and suggest fixes:",
      "description": "Security analysis"
    },
    {
      "name": "test",
      "prompt": "Generate comprehensive unit tests for this code, including edge cases:",
      "description": "Generate unit tests"
    },
    {
      "name": "document",
      "prompt": "Add comprehensive documentation and comments to this code:",
      "description": "Add documentation"
    }
  ]
}
EOF
    
    print_success "Cursor configuration updated with advanced models!"
}

# Performance recommendations
show_advanced_tips() {
    echo ""
    print_gpu "🎯 Advanced Model Performance Tips:"
    echo "===================================="
    echo ""
    echo "🔥 Model Selection Strategy:"
    echo "  • Autocomplete: deepseek-coder-v2:16b (fast, accurate)"
    echo "  • Code generation: codestral or starcoder2:15b"
    echo "  • Large projects: deepseek-coder-v2-32k (32k context)"
    echo "  • Complex reasoning: qwen2.5-coder:32b (most powerful)"
    echo ""
    echo "⚡ VRAM Optimization:"
    echo "  • Monitor usage: watch -n 1 rocm-smi"
    echo "  • Single model: Use one 16B model at a time"
    echo "  • Multiple models: Mix smaller models (7B + 13B)"
    echo "  • Gaming mode: Stick to models under 8GB"
    echo ""
    echo "🚀 Speed Optimization:"
    echo "  • Use temperature 0.1-0.2 for faster, deterministic results"
    echo "  • Set appropriate context lengths (don't always use max)"
    echo "  • Use num_gpu=99 to fully utilize your GPU"
    echo ""
    echo "🧠 Model Capabilities:"
    echo "  • deepseek-coder-v2:16b - Best overall coding model"
    echo "  • qwen2.5-coder:32b - Most powerful but slower"
    echo "  • codestral - Excellent for multiple languages"
    echo "  • starcoder2:15b - Great for code completion"
    echo ""
    echo "🔧 Testing Commands:"
    echo "  • Test model: ollama run deepseek-coder-v2:16b 'write a hello world in python'"
    echo "  • Check VRAM: rocm-smi --showmeminfo vram"
    echo "  • List models: ollama list"
    echo "  • Remove model: ollama rm model-name"
}

# Main execution
main() {
    check_ollama
    check_vram
    
    echo "This script will help you set up advanced coding models."
    echo "These are some of the most powerful coding AI models available!"
    echo ""
    
    download_advanced_models
    create_custom_models
    update_cursor_config
    show_advanced_tips
    
    echo ""
    print_success "🎉 Advanced coding models setup complete!"
    print_gpu "Your RX 7800 XT is now equipped with the most powerful coding AI!"
    echo ""
    echo "🚀 Quick start:"
    echo "1. Restart Cursor"
    echo "2. Use Ctrl+K to chat with your new models"
    echo "3. Try the custom commands like /review, /optimize, /security"
    echo "4. Monitor VRAM usage with: rocm-smi"
}

# Handle interruption
trap 'print_error "Setup interrupted"; exit 1' INT

main "$@"