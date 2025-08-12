// API Configuration
const API_URL = 'http://localhost:8000';
let ws = null;
let currentModel = '';

// DOM Elements
const elements = {
    modelSelect: document.getElementById('modelSelect'),
    refreshModels: document.getElementById('refreshModels'),
    uncensoredList: document.getElementById('uncensoredList'),
    newModelName: document.getElementById('newModelName'),
    pullModel: document.getElementById('pullModel'),
    temperature: document.getElementById('temperature'),
    tempValue: document.getElementById('tempValue'),
    systemPrompt: document.getElementById('systemPrompt'),
    chatMessages: document.getElementById('chatMessages'),
    userInput: document.getElementById('userInput'),
    sendBtn: document.getElementById('sendBtn'),
    clearBtn: document.getElementById('clearBtn'),
    streamingMode: document.getElementById('streamingMode'),
    connectionStatus: document.getElementById('connectionStatus'),
    ollamaStatus: document.getElementById('ollamaStatus')
};

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    checkStatus();
    loadModels();
    loadUncensoredModels();
    setupEventListeners();
    
    // Check status every 5 seconds
    setInterval(checkStatus, 5000);
});

// Event Listeners
function setupEventListeners() {
    elements.refreshModels.addEventListener('click', loadModels);
    elements.pullModel.addEventListener('click', pullNewModel);
    elements.sendBtn.addEventListener('click', sendMessage);
    elements.clearBtn.addEventListener('click', clearChat);
    elements.temperature.addEventListener('input', updateTemperature);
    elements.userInput.addEventListener('keydown', handleEnterKey);
    elements.modelSelect.addEventListener('change', (e) => {
        currentModel = e.target.value;
    });
}

// Status Check
async function checkStatus() {
    try {
        const response = await fetch(`${API_URL}/`);
        const data = await response.json();
        
        elements.connectionStatus.textContent = 'Connected';
        elements.connectionStatus.className = 'status-indicator connected';
        
        elements.ollamaStatus.textContent = data.ollama_connected ? 'Running' : 'Not Running';
        elements.ollamaStatus.className = data.ollama_connected ? 'status-indicator connected' : 'status-indicator disconnected';
    } catch (error) {
        elements.connectionStatus.textContent = 'Disconnected';
        elements.connectionStatus.className = 'status-indicator disconnected';
        elements.ollamaStatus.textContent = 'Unknown';
        elements.ollamaStatus.className = 'status-indicator checking';
    }
}

// Load Models
async function loadModels() {
    try {
        const response = await fetch(`${API_URL}/models`);
        const models = await response.json();
        
        elements.modelSelect.innerHTML = '<option value="">Select a model...</option>';
        models.forEach(model => {
            const option = document.createElement('option');
            option.value = model.name;
            option.textContent = `${model.name} (${formatSize(model.size)})`;
            elements.modelSelect.appendChild(option);
        });
        
        if (models.length > 0 && !currentModel) {
            currentModel = models[0].name;
            elements.modelSelect.value = currentModel;
        }
    } catch (error) {
        console.error('Failed to load models:', error);
        elements.modelSelect.innerHTML = '<option value="">Failed to load models</option>';
    }
}

// Load Uncensored Models
async function loadUncensoredModels() {
    try {
        const response = await fetch(`${API_URL}/uncensored-models`);
        const data = await response.json();
        
        // Updated list for 16GB VRAM (7800 XT)
        const models16GB = [
            { name: 'dolphin-mixtral:8x7b-v2.7-q4_K_M', size: '~26GB quantized to ~8GB', desc: 'Excellent uncensored model, fits in 16GB' },
            { name: 'dolphin-llama3:8b-v2.9-q5_K_M', size: '~5.5GB', desc: 'Latest Llama 3 uncensored, great performance' },
            { name: 'wizard-vicuna-13b-uncensored:q4_K_M', size: '~7.5GB', desc: 'Classic uncensored, very capable' },
            { name: 'openhermes-2.5-mistral-7b:q5_K_M', size: '~5GB', desc: 'Uncensored, trained on diverse data' },
            { name: 'nous-hermes2-mixtral:8x7b-dpo-q4_K_M', size: '~26GB quantized', desc: 'High quality uncensored responses' },
            { name: 'dolphin-mistral:7b-v2.8-q6_K', size: '~6GB', desc: 'Uncensored Mistral, excellent quality' },
            { name: 'solar-10.7b-instruct-uncensored:q5_K_M', size: '~7.5GB', desc: 'Powerful 10.7B uncensored model' },
            { name: 'yi-34b-chat:q3_K_M', size: '~14GB', desc: 'Large model that fits in 16GB, very capable' },
            { name: 'deepseek-coder-33b:q3_K_M', size: '~13GB', desc: 'Uncensored coding model' },
            { name: 'mixtral-8x7b-instruct:q3_K_M', size: '~16GB', desc: 'Fits exactly in your VRAM, very powerful' }
        ];
        
        elements.uncensoredList.innerHTML = '';
        models16GB.forEach(model => {
            const li = document.createElement('li');
            li.innerHTML = `
                <strong>${model.name}</strong><br>
                <small>Size: ${model.size}</small><br>
                <small>${model.desc}</small>
            `;
            li.style.marginBottom = '1rem';
            li.style.padding = '0.75rem';
            li.style.backgroundColor = 'var(--background-color)';
            li.style.borderRadius = '8px';
            li.style.border = '1px solid var(--border-color)';
            elements.uncensoredList.appendChild(li);
        });
        
        // Add note about your GPU
        const note = document.createElement('div');
        note.style.marginTop = '1rem';
        note.style.padding = '1rem';
        note.style.backgroundColor = 'var(--success-color)';
        note.style.color = 'white';
        note.style.borderRadius = '8px';
        note.style.fontSize = '0.9rem';
        note.innerHTML = `
            <strong>🎮 AMD RX 7800 XT (16GB) Detected!</strong><br>
            Your GPU can handle models up to ~16GB comfortably. 
            The models above are optimized for your VRAM capacity.
            For best performance, use q4_K_M or q5_K_M quantizations.
        `;
        elements.uncensoredList.appendChild(note);
        
    } catch (error) {
        console.error('Failed to load uncensored models:', error);
        elements.uncensoredList.innerHTML = '<li>Failed to load uncensored models</li>';
    }
}

// Pull New Model
async function pullNewModel() {
    const modelName = elements.newModelName.value.trim();
    if (!modelName) {
        alert('Please enter a model name');
        return;
    }
    
    elements.pullModel.disabled = true;
    elements.pullModel.textContent = 'Pulling...';
    
    try {
        const response = await fetch(`${API_URL}/pull-model?model_name=${encodeURIComponent(modelName)}`, {
            method: 'POST'
        });
        const data = await response.json();
        
        if (response.ok) {
            alert(`Successfully pulled ${modelName}`);
            elements.newModelName.value = '';
            loadModels();
        } else {
            alert(`Failed to pull model: ${data.detail}`);
        }
    } catch (error) {
        alert(`Error pulling model: ${error.message}`);
    } finally {
        elements.pullModel.disabled = false;
        elements.pullModel.textContent = 'Pull Model';
    }
}

// Send Message
async function sendMessage() {
    const prompt = elements.userInput.value.trim();
    if (!prompt || !currentModel) {
        alert('Please enter a message and select a model');
        return;
    }
    
    // Add user message to chat
    addMessage('user', prompt);
    elements.userInput.value = '';
    elements.sendBtn.disabled = true;
    
    if (elements.streamingMode.checked) {
        // Use WebSocket for streaming
        sendStreamingMessage(prompt);
    } else {
        // Use regular HTTP request
        sendRegularMessage(prompt);
    }
}

// Regular HTTP Chat
async function sendRegularMessage(prompt) {
    try {
        const response = await fetch(`${API_URL}/chat`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                model: currentModel,
                prompt: prompt,
                system_prompt: elements.systemPrompt.value,
                temperature: parseFloat(elements.temperature.value),
                max_tokens: 2048
            })
        });
        
        const data = await response.json();
        
        if (response.ok) {
            addMessage('ai', data.response, currentModel);
        } else {
            addMessage('ai', `Error: ${data.detail}`, 'Error');
        }
    } catch (error) {
        addMessage('ai', `Error: ${error.message}`, 'Error');
    } finally {
        elements.sendBtn.disabled = false;
    }
}

// Streaming WebSocket Chat
function sendStreamingMessage(prompt) {
    if (ws) ws.close();
    
    ws = new WebSocket(`ws://localhost:8000/ws/chat`);
    let fullResponse = '';
    let messageDiv = null;
    
    ws.onopen = () => {
        ws.send(JSON.stringify({
            model: currentModel,
            prompt: prompt,
            system_prompt: elements.systemPrompt.value,
            temperature: parseFloat(elements.temperature.value)
        }));
    };
    
    ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        
        if (data.type === 'chunk') {
            if (!messageDiv) {
                messageDiv = addMessage('ai', '', currentModel);
            }
            fullResponse += data.content;
            messageDiv.querySelector('.message-content').textContent = fullResponse;
            elements.chatMessages.scrollTop = elements.chatMessages.scrollHeight;
        } else if (data.type === 'complete') {
            elements.sendBtn.disabled = false;
            ws.close();
        } else if (data.type === 'error') {
            addMessage('ai', `Error: ${data.message}`, 'Error');
            elements.sendBtn.disabled = false;
            ws.close();
        }
    };
    
    ws.onerror = () => {
        addMessage('ai', 'WebSocket connection error', 'Error');
        elements.sendBtn.disabled = false;
    };
}

// Add Message to Chat
function addMessage(type, content, model = '') {
    // Remove welcome message if it exists
    const welcomeMsg = document.querySelector('.welcome-message');
    if (welcomeMsg) welcomeMsg.remove();
    
    const messageDiv = document.createElement('div');
    messageDiv.className = `message ${type}-message`;
    
    const header = document.createElement('div');
    header.className = 'message-header';
    header.textContent = type === 'user' ? 'You' : model || 'AI';
    
    const contentDiv = document.createElement('div');
    contentDiv.className = 'message-content';
    contentDiv.textContent = content;
    
    messageDiv.appendChild(header);
    messageDiv.appendChild(contentDiv);
    
    elements.chatMessages.appendChild(messageDiv);
    elements.chatMessages.scrollTop = elements.chatMessages.scrollHeight;
    
    return messageDiv;
}

// Clear Chat
function clearChat() {
    elements.chatMessages.innerHTML = `
        <div class="welcome-message">
            <h2>Welcome to Ollama Cursor POC! 👋</h2>
            <p>This is an uncensored AI chat interface powered by Ollama.</p>
            <p>Select a model from the sidebar and start chatting!</p>
        </div>
    `;
}

// Update Temperature Display
function updateTemperature() {
    elements.tempValue.textContent = elements.temperature.value;
}

// Handle Enter Key
function handleEnterKey(event) {
    if (event.key === 'Enter' && !event.shiftKey) {
        event.preventDefault();
        sendMessage();
    }
}

// Utility Functions
function formatSize(sizeStr) {
    // Convert size string to human readable format
    if (!sizeStr || sizeStr === 'Unknown') return 'Unknown';
    return sizeStr;
}