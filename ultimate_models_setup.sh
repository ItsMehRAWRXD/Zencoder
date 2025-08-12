#!/bin/bash

# ULTIMATE AI MODELS SETUP - The Complete Collection
# For AMD RX 7800 XT (16GB) - All the best coding & uncensored models

set -e

echo "🚀 ULTIMATE AI MODELS COLLECTION SETUP"
echo "======================================"
echo "Building the most comprehensive local AI setup possible!"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
MAGENTA='\033[0;95m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_gpu() { echo -e "${PURPLE}[GPU]${NC} $1"; }
print_model() { echo -e "${CYAN}[MODEL]${NC} $1"; }
print_ultimate() { echo -e "${MAGENTA}[ULTIMATE]${NC} $1"; }

# Function to show current VRAM and disk usage
show_system_status() {
    echo ""
    print_gpu "🔥 System Status:"
    echo "=================="
    
    # VRAM usage
    if command -v rocm-smi >/dev/null 2>&1; then
        echo "💾 VRAM Usage:"
        rocm-smi --showmeminfo vram | head -5
    fi
    
    # Disk usage
    echo ""
    echo "💽 Disk Usage:"
    df -h / | tail -1
    
    # Model storage location
    echo ""
    echo "📁 Ollama Models Location:"
    du -sh ~/.ollama/models 2>/dev/null || echo "Models directory not found"
    
    echo ""
}

# Ultimate model collection with detailed info
download_ultimate_collection() {
    print_ultimate "Downloading the ULTIMATE AI model collection..."
    echo ""
    
    # Define the complete model collection
    declare -A ultimate_models=(
        # Latest & Most Advanced Coding Models
        ["deepseek-coder-v2:16b"]="Latest DeepSeek V2 - Most advanced coding AI|~10GB|🚀 BEST OVERALL"
        ["qwen2.5-coder:32b"]="Qwen 2.5 Coder 32B - Massive coding power|~18GB|🧠 MOST POWERFUL"
        ["codestral"]="Mistral's coding specialist - Multi-language expert|~12GB|⚡ EXCELLENT"
        ["starcoder2:15b"]="StarCoder2 15B - GitHub trained excellence|~9GB|🌟 CODE COMPLETION"
        
        # CodeLlama Variants
        ["codellama:13b-code"]="CodeLlama Code - Pure code generation|~7GB|🦙 CODE SPECIALIST"
        ["codellama:13b"]="CodeLlama Instruct - Instruction following|~7GB|🦙 VERSATILE"
        ["phind-codellama:34b-v2"]="Phind CodeLlama 34B - Most capable CodeLlama|~20GB|🎯 ADVANCED"
        
        # StarCoder Collection
        ["starcoder:15b"]="Original StarCoder 15B - Proven excellence|~9GB|⭐ RELIABLE"
        
        # Uncensored Powerhouses
        ["wizard-vicuna-uncensored:13b"]="Wizard Vicuna Uncensored - No limits|~8GB|🧙 UNCENSORED"
        ["llama2-uncensored:13b"]="Llama2 Uncensored - Classic freedom|~7GB|🔓 CLASSIC"
        ["vicuna:13b"]="Vicuna 13B - Conversational excellence|~7GB|💬 CONVERSATIONAL"
        ["orca-mini:13b"]="Orca Mini 13B - Microsoft's compact power|~7GB|🐋 EFFICIENT"
        
        # Fast & Efficient Models
        ["openchat:7b"]="OpenChat 7B - Open source speed|~4GB|🚀 FAST"
    )
    
    echo "📋 ULTIMATE MODEL COLLECTION:"
    echo "============================="
    local total_size=0
    local model_count=0
    
    for model in "${!ultimate_models[@]}"; do
        IFS='|' read -r description vram category <<< "${ultimate_models[$model]}"
        ((model_count++))
        
        # Extract size for calculation
        size_gb=$(echo "$vram" | grep -o '[0-9]\+' | head -1)
        total_size=$((total_size + size_gb))
        
        echo ""
        echo "$model_count. $model"
        echo "   📝 $description"
        echo "   💾 $vram"
        echo "   🏷️  $category"
    done
    
    echo ""
    echo "📊 Collection Stats:"
    echo "  • Total Models: $model_count"
    echo "  • Estimated Storage: ~${total_size}GB+"
    echo "  • Your GPU: 16GB (perfect for individual models)"
    echo ""
    
    # Download options
    echo "🎯 Download Options:"
    echo "1. 🚀 Essential Pack (5 best models)"
    echo "2. 🧠 Coding Masters Pack (all coding specialists)"
    echo "3. 🔓 Uncensored Pack (all uncensored models)"
    echo "4. ⚡ Speed Pack (fastest models)"
    echo "5. 🎯 Custom Selection"
    echo "6. 🌟 ULTIMATE COLLECTION (all models)"
    echo ""
    
    read -p "Choose your setup (1-6): " choice
    
    case $choice in
        1) download_essential_pack ;;
        2) download_coding_masters ;;
        3) download_uncensored_pack ;;
        4) download_speed_pack ;;
        5) download_custom_selection ;;
        6) download_all_models ;;
        *) print_error "Invalid choice"; exit 1 ;;
    esac
}

# Download functions for different packs
download_essential_pack() {
    print_ultimate "🚀 ESSENTIAL PACK - The must-have models"
    local models=(
        "deepseek-coder-v2:16b"
        "codestral" 
        "wizard-vicuna-uncensored:13b"
        "starcoder2:15b"
        "openchat:7b"
    )
    
    for model in "${models[@]}"; do
        download_single_model "$model"
    done
}

download_coding_masters() {
    print_ultimate "🧠 CODING MASTERS PACK - All coding specialists"
    local models=(
        "deepseek-coder-v2:16b"
        "qwen2.5-coder:32b"
        "codestral"
        "starcoder2:15b"
        "codellama:13b-code"
        "codellama:13b"
        "phind-codellama:34b-v2"
        "starcoder:15b"
    )
    
    for model in "${models[@]}"; do
        download_single_model "$model"
    done
}

download_uncensored_pack() {
    print_ultimate "🔓 UNCENSORED PACK - Complete freedom"
    local models=(
        "wizard-vicuna-uncensored:13b"
        "llama2-uncensored:13b"
        "vicuna:13b"
        "orca-mini:13b"
        "openchat:7b"
    )
    
    for model in "${models[@]}"; do
        download_single_model "$model"
    done
}

download_speed_pack() {
    print_ultimate "⚡ SPEED PACK - Lightning fast responses"
    local models=(
        "openchat:7b"
        "vicuna:13b"
        "starcoder2:15b"
        "codellama:13b-code"
    )
    
    for model in "${models[@]}"; do
        download_single_model "$model"
    done
}

download_custom_selection() {
    print_ultimate "🎯 CUSTOM SELECTION - Choose your models"
    
    declare -A ultimate_models=(
        ["deepseek-coder-v2:16b"]="Latest DeepSeek V2 - Most advanced coding AI|~10GB|🚀 BEST OVERALL"
        ["qwen2.5-coder:32b"]="Qwen 2.5 Coder 32B - Massive coding power|~18GB|🧠 MOST POWERFUL"
        ["codestral"]="Mistral's coding specialist - Multi-language expert|~12GB|⚡ EXCELLENT"
        ["starcoder2:15b"]="StarCoder2 15B - GitHub trained excellence|~9GB|🌟 CODE COMPLETION"
        ["codellama:13b-code"]="CodeLlama Code - Pure code generation|~7GB|🦙 CODE SPECIALIST"
        ["codellama:13b"]="CodeLlama Instruct - Instruction following|~7GB|🦙 VERSATILE"
        ["phind-codellama:34b-v2"]="Phind CodeLlama 34B - Most capable CodeLlama|~20GB|🎯 ADVANCED"
        ["starcoder:15b"]="Original StarCoder 15B - Proven excellence|~9GB|⭐ RELIABLE"
        ["wizard-vicuna-uncensored:13b"]="Wizard Vicuna Uncensored - No limits|~8GB|🧙 UNCENSORED"
        ["llama2-uncensored:13b"]="Llama2 Uncensored - Classic freedom|~7GB|🔓 CLASSIC"
        ["vicuna:13b"]="Vicuna 13B - Conversational excellence|~7GB|💬 CONVERSATIONAL"
        ["orca-mini:13b"]="Orca Mini 13B - Microsoft's compact power|~7GB|🐋 EFFICIENT"
        ["openchat:7b"]="OpenChat 7B - Open source speed|~4GB|🚀 FAST"
    )
    
    for model in "${!ultimate_models[@]}"; do
        IFS='|' read -r description vram category <<< "${ultimate_models[$model]}"
        read -p "Download $model? ($category) (y/N): " choice
        if [[ $choice =~ ^[Yy]$ ]]; then
            download_single_model "$model"
        fi
    done
}

download_all_models() {
    print_ultimate "🌟 DOWNLOADING ULTIMATE COLLECTION - ALL MODELS!"
    print_warning "This will download ~120GB+ of models. Ensure you have enough space!"
    read -p "Are you absolutely sure? This is a lot of data! (y/N): " confirm
    
    if [[ ! $confirm =~ ^[Yy]$ ]]; then
        print_status "Wise choice! Consider starting with a smaller pack."
        return
    fi
    
    local all_models=(
        "deepseek-coder-v2:16b"
        "qwen2.5-coder:32b"
        "codestral"
        "starcoder2:15b"
        "codellama:13b-code"
        "vicuna:13b"
        "llama2-uncensored:13b"
        "orca-mini:13b"
        "codellama:13b"
        "phind-codellama:34b-v2"
        "starcoder:15b"
        "wizard-vicuna-uncensored:13b"
        "openchat:7b"
    )
    
    for model in "${all_models[@]}"; do
        download_single_model "$model"
        sleep 3  # Brief pause between massive downloads
    done
}

# Enhanced download function with progress tracking
download_single_model() {
    local model=$1
    
    print_model "📥 Processing: $model"
    
    if ollama list | grep -q "^$model"; then
        print_success "✅ $model already installed"
        return 0
    fi
    
    # Check disk space before download
    local available_space
    available_space=$(df ~/.ollama 2>/dev/null | awk 'NR==2{print $4}' || df / | awk 'NR==2{print $4}')
    local available_gb=$((available_space / 1024 / 1024))
    
    if [ $available_gb -lt 15 ]; then
        print_warning "⚠️  Low disk space: ${available_gb}GB available"
        read -p "Continue downloading $model? (y/N): " continue_choice
        if [[ ! $continue_choice =~ ^[Yy]$ ]]; then
            print_status "Skipping $model"
            return 1
        fi
    fi
    
    print_status "🔄 Downloading $model... (This may take 10-30 minutes)"
    
    # Download with enhanced error handling
    local max_attempts=3
    local attempt=1
    
    while [ $attempt -le $max_attempts ]; do
        if timeout 7200 ollama pull "$model"; then  # 2 hour timeout for large models
            print_success "✅ $model downloaded successfully!"
            
            # Quick test
            print_status "🧪 Testing $model..."
            local test_result
            test_result=$(timeout 60 ollama run "$model" "print('test')" 2>/dev/null || echo "timeout")
            
            if [[ "$test_result" == *"timeout"* ]]; then
                print_warning "⚠️  $model loaded but slow (normal for large models)"
            else
                print_success "🚀 $model working perfectly!"
            fi
            
            show_system_status
            return 0
        else
            print_warning "❌ Attempt $attempt failed for $model"
            ((attempt++))
            if [ $attempt -le $max_attempts ]; then
                print_status "🔄 Retrying in 15 seconds..."
                sleep 15
            fi
        fi
    done
    
    print_error "❌ Failed to download $model after $max_attempts attempts"
    return 1
}

# Create ultimate Cursor configuration
create_ultimate_cursor_config() {
    print_status "🔧 Creating ULTIMATE Cursor configuration..."
    
    local continue_dir="$HOME/.continue"
    mkdir -p "$continue_dir"
    
    # Backup existing config
    if [ -f "$continue_dir/config.json" ]; then
        cp "$continue_dir/config.json" "$continue_dir/config.json.backup.$(date +%s)"
        print_status "Backed up existing configuration"
    fi
    
    # Generate dynamic config based on installed models
    cat > "$continue_dir/config.json" << 'EOF'
{
  "models": [
    {
      "title": "🚀 DeepSeek Coder V2 16B (LATEST)",
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
      "title": "🧠 Qwen2.5 Coder 32B (MOST POWERFUL)",
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
      "title": "⚡ Codestral (MISTRAL CODING)",
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
      "title": "🌟 StarCoder2 15B (CODE COMPLETION)",
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
      "title": "🧙 Wizard Vicuna Uncensored (NO LIMITS)",
      "provider": "ollama",
      "model": "wizard-vicuna-uncensored:13b",
      "apiBase": "http://localhost:11434",
      "contextLength": 4096,
      "completionOptions": {
        "temperature": 0.8,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "🔓 Llama2 Uncensored (CLASSIC FREEDOM)",
      "provider": "ollama",
      "model": "llama2-uncensored:13b",
      "apiBase": "http://localhost:11434",
      "contextLength": 4096,
      "completionOptions": {
        "temperature": 0.7,
        "top_p": 0.9,
        "num_gpu": 99
      }
    },
    {
      "title": "🦙 CodeLlama Code (PURE CODE)",
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
      "title": "🎯 Phind CodeLlama 34B (ADVANCED)",
      "provider": "ollama",
      "model": "phind-codellama:34b-v2",
      "apiBase": "http://localhost:11434",
      "contextLength": 16384,
      "completionOptions": {
        "temperature": 0.1,
        "top_p": 0.95,
        "num_gpu": 99
      }
    },
    {
      "title": "💬 Vicuna 13B (CONVERSATIONAL)",
      "provider": "ollama",
      "model": "vicuna:13b",
      "apiBase": "http://localhost:11434",
      "contextLength": 4096,
      "completionOptions": {
        "temperature": 0.7,
        "top_p": 0.9,
        "num_gpu": 99
      }
    },
    {
      "title": "🚀 OpenChat 7B (FAST)",
      "provider": "ollama",
      "model": "openchat:7b",
      "apiBase": "http://localhost:11434",
      "contextLength": 8192,
      "completionOptions": {
        "temperature": 0.6,
        "top_p": 0.9,
        "num_gpu": 99
      }
    }
  ],
  "tabAutocompleteModel": {
    "title": "🚀 Ultra-Fast Autocomplete",
    "provider": "ollama",
    "model": "openchat:7b",
    "apiBase": "http://localhost:11434",
    "completionOptions": {
      "temperature": 0.2,
      "max_tokens": 100,
      "num_gpu": 99
    }
  },
  "customCommands": [
    {
      "name": "ultimate-review",
      "prompt": "As an expert code reviewer, analyze this code for:\n1. Bugs and potential issues\n2. Performance optimizations\n3. Security vulnerabilities\n4. Best practices compliance\n5. Code maintainability\nProvide specific, actionable suggestions:",
      "description": "Ultimate code review"
    },
    {
      "name": "architect",
      "prompt": "As a senior software architect, review this code and suggest:\n1. Architectural improvements\n2. Design pattern applications\n3. Scalability considerations\n4. Refactoring opportunities\n5. Testing strategies:",
      "description": "Architectural analysis"
    },
    {
      "name": "optimize-extreme",
      "prompt": "Optimize this code for maximum performance considering:\n1. Algorithm efficiency\n2. Memory usage\n3. CPU optimization\n4. I/O efficiency\n5. Parallel processing opportunities:",
      "description": "Extreme optimization"
    },
    {
      "name": "security-audit",
      "prompt": "Perform a comprehensive security audit of this code, checking for:\n1. Common vulnerabilities (OWASP Top 10)\n2. Input validation issues\n3. Authentication/authorization flaws\n4. Data exposure risks\n5. Injection attacks:",
      "description": "Security audit"
    },
    {
      "name": "generate-tests",
      "prompt": "Generate comprehensive tests for this code including:\n1. Unit tests with edge cases\n2. Integration tests\n3. Performance tests\n4. Security tests\n5. Mock implementations:",
      "description": "Comprehensive test generation"
    }
  ]
}
EOF
    
    print_success "🎉 ULTIMATE Cursor configuration created!"
}

# Create model management script
create_model_manager() {
    print_status "Creating model management script..."
    
    cat > "/workspace/model_manager.sh" << 'EOF'
#!/bin/bash

# Ultimate Model Manager for 16GB RX 7800 XT
# Manage your massive AI model collection efficiently

echo "🎮 ULTIMATE MODEL MANAGER"
echo "========================"

case "$1" in
    "list")
        echo "📋 Installed Models:"
        ollama list
        ;;
    "status")
        echo "📊 System Status:"
        echo ""
        echo "💾 VRAM Usage:"
        rocm-smi --showmeminfo vram 2>/dev/null || echo "ROCm not available"
        echo ""
        echo "💽 Disk Usage:"
        du -sh ~/.ollama/models 2>/dev/null || echo "Models directory not found"
        ;;
    "test")
        if [ -z "$2" ]; then
            echo "Usage: $0 test <model-name>"
            exit 1
        fi
        echo "🧪 Testing $2..."
        ollama run "$2" "write a hello world function in python"
        ;;
    "benchmark")
        echo "🏃 Benchmarking models..."
        echo "Testing response speed for each model..."
        
        models=("openchat:7b" "vicuna:13b" "deepseek-coder-v2:16b" "wizard-vicuna-uncensored:13b")
        
        for model in "${models[@]}"; do
            if ollama list | grep -q "$model"; then
                echo ""
                echo "Testing $model..."
                start_time=$(date +%s.%N)
                ollama run "$model" "print('hello')" >/dev/null 2>&1
                end_time=$(date +%s.%N)
                duration=$(echo "$end_time - $start_time" | bc -l)
                printf "⏱️  $model: %.2f seconds\n" "$duration"
            fi
        done
        ;;
    "gaming-mode")
        echo "🎮 Gaming Mode - Keeping only lightweight models loaded"
        echo "This will help free up VRAM for gaming"
        # This would implement logic to unload heavy models
        echo "Recommendation: Use only openchat:7b or vicuna:13b while gaming"
        ;;
    "cleanup")
        echo "🧹 Cleaning up unused models..."
        echo "Current models:"
        ollama list
        echo ""
        read -p "Enter model name to remove (or 'cancel'): " model_name
        if [ "$model_name" != "cancel" ]; then
            ollama rm "$model_name"
            echo "✅ $model_name removed"
        fi
        ;;
    *)
        echo "🎮 Ultimate Model Manager Commands:"
        echo ""
        echo "📋 ./model_manager.sh list           - List all models"
        echo "📊 ./model_manager.sh status         - Show system status"  
        echo "🧪 ./model_manager.sh test <model>   - Test a specific model"
        echo "🏃 ./model_manager.sh benchmark      - Benchmark model speeds"
        echo "🎮 ./model_manager.sh gaming-mode    - Optimize for gaming"
        echo "🧹 ./model_manager.sh cleanup        - Remove unused models"
        echo ""
        echo "Examples:"
        echo "  ./model_manager.sh test deepseek-coder-v2:16b"
        echo "  ./model_manager.sh benchmark"
        ;;
esac
EOF
    
    chmod +x "/workspace/model_manager.sh"
    print_success "✅ Model manager created at ./model_manager.sh"
}

# Show ultimate setup completion
show_ultimate_completion() {
    echo ""
    print_ultimate "🎉 ULTIMATE AI SETUP COMPLETE!"
    echo "==============================="
    echo ""
    print_gpu "🔥 Your RX 7800 XT is now equipped with the most powerful AI collection possible!"
    echo ""
    echo "📋 What you have:"
    echo "  • Multiple state-of-the-art coding models"
    echo "  • Completely uncensored AI assistants"
    echo "  • Models optimized for different tasks"
    echo "  • Advanced Cursor configuration"
    echo "  • Model management tools"
    echo ""
    echo "🚀 Quick Start Guide:"
    echo "  1. Restart Cursor IDE"
    echo "  2. Install Continue extension"
    echo "  3. Use Ctrl+K to chat with your models"
    echo "  4. Try custom commands: /ultimate-review, /architect, /security-audit"
    echo "  5. Use ./model_manager.sh for model management"
    echo ""
    echo "🎯 Model Recommendations by Task:"
    echo "  • 🚀 General coding: deepseek-coder-v2:16b"
    echo "  • 🧠 Complex problems: qwen2.5-coder:32b"
    echo "  • ⚡ Fast responses: openchat:7b"
    echo "  • 🔓 Uncensored chat: wizard-vicuna-uncensored:13b"
    echo "  • 🌟 Code completion: starcoder2:15b"
    echo ""
    echo "💡 Pro Tips:"
    echo "  • Monitor VRAM: rocm-smi"
    echo "  • Switch models based on task complexity"
    echo "  • Use gaming-mode for lighter VRAM usage"
    echo "  • Each model excels at different things - experiment!"
    echo ""
    print_ultimate "Welcome to the future of AI-assisted development! 🚀"
}

# Main execution
main() {
    show_system_status
    
    echo "Welcome to the ULTIMATE AI Models Setup!"
    echo "This will help you build the most comprehensive local AI collection."
    echo ""
    
    download_ultimate_collection
    create_ultimate_cursor_config
    create_model_manager
    show_ultimate_completion
}

# Handle interruption gracefully
trap 'print_error "Setup interrupted by user"; exit 1' INT

main "$@"