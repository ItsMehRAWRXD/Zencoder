"""
Web interface for Ollama Uncensored AI using Streamlit
"""

import streamlit as st
import requests
import json
import time
from typing import Dict, List, Any
import asyncio

# Page configuration
st.set_page_config(
    page_title="Ollama Uncensored AI",
    page_icon="🤖",
    layout="wide",
    initial_sidebar_state="expanded"
)

# Custom CSS for better styling
st.markdown("""
<style>
    .main-header {
        font-size: 3rem;
        font-weight: bold;
        color: #FF6B6B;
        text-align: center;
        margin-bottom: 2rem;
    }
    .model-card {
        background-color: #f0f2f6;
        padding: 1rem;
        border-radius: 0.5rem;
        margin: 0.5rem 0;
    }
    .response-box {
        background-color: #f8f9fa;
        border: 1px solid #dee2e6;
        border-radius: 0.5rem;
        padding: 1rem;
        margin: 1rem 0;
    }
    .warning-box {
        background-color: #fff3cd;
        border: 1px solid #ffeaa7;
        border-radius: 0.5rem;
        padding: 1rem;
        margin: 1rem 0;
    }
</style>
""", unsafe_allow_html=True)

# API configuration
API_BASE_URL = "http://localhost:8000"

def check_api_health():
    """Check if the API server is running"""
    try:
        response = requests.get(f"{API_BASE_URL}/health", timeout=5)
        return response.status_code == 200, response.json()
    except:
        return False, None

def get_available_models():
    """Get list of available models"""
    try:
        response = requests.get(f"{API_BASE_URL}/models", timeout=10)
        if response.status_code == 200:
            return response.json()
        return []
    except:
        return []

def generate_text(model: str, prompt: str, prompt_type: str, **kwargs):
    """Generate text using the API"""
    try:
        payload = {
            "model": model,
            "prompt": prompt,
            "prompt_type": prompt_type,
            **kwargs
        }
        
        response = requests.post(
            f"{API_BASE_URL}/generate",
            json=payload,
            timeout=60
        )
        
        if response.status_code == 200:
            return response.json()
        else:
            return {"error": f"API Error: {response.status_code} - {response.text}"}
    except Exception as e:
        return {"error": f"Request failed: {str(e)}"}

def chat_with_model(model: str, messages: List[Dict], prompt_type: str, **kwargs):
    """Chat with the model"""
    try:
        payload = {
            "model": model,
            "messages": messages,
            "prompt_type": prompt_type,
            **kwargs
        }
        
        response = requests.post(
            f"{API_BASE_URL}/chat",
            json=payload,
            timeout=60
        )
        
        if response.status_code == 200:
            return response.json()
        else:
            return {"error": f"API Error: {response.status_code} - {response.text}"}
    except Exception as e:
        return {"error": f"Request failed: {str(e)}"}

def main():
    # Header
    st.markdown('<h1 class="main-header">🤖 Ollama Uncensored AI</h1>', unsafe_allow_html=True)
    
    # Check API health
    api_healthy, health_data = check_api_health()
    
    if not api_healthy:
        st.error("❌ API server is not running. Please start the server first:")
        st.code("python api_server.py")
        st.stop()
    
    # Sidebar
    with st.sidebar:
        st.header("⚙️ Configuration")
        
        # Model selection
        models = get_available_models()
        model_names = [model["name"] for model in models] if models else ["dolphin-phi", "nous-hermes2"]
        
        selected_model = st.selectbox(
            "Select Model",
            model_names,
            index=0
        )
        
        # Prompt type selection
        prompt_types = {
            "general": "General",
            "creative_writing": "Creative Writing",
            "roleplay": "Roleplay",
            "coding": "Coding"
        }
        
        selected_prompt_type = st.selectbox(
            "Prompt Type",
            list(prompt_types.keys()),
            format_func=lambda x: prompt_types[x]
        )
        
        # Model parameters
        st.subheader("Model Parameters")
        temperature = st.slider("Temperature", 0.0, 2.0, 0.7, 0.1)
        top_p = st.slider("Top-p", 0.0, 1.0, 0.9, 0.05)
        max_tokens = st.slider("Max Tokens", 100, 4096, 2048, 100)
        
        # API status
        st.subheader("API Status")
        if api_healthy and health_data:
            st.success(f"✅ Connected to Ollama")
            st.info(f"Available models: {health_data.get('available_models', 0)}")
        else:
            st.error("❌ API not available")
    
    # Main content
    tab1, tab2, tab3 = st.tabs(["💬 Chat", "📝 Generate", "📊 Models"])
    
    with tab1:
        st.header("💬 Chat with Uncensored AI")
        
        # Initialize chat history
        if "messages" not in st.session_state:
            st.session_state.messages = []
        
        # Display chat history
        for message in st.session_state.messages:
            with st.chat_message(message["role"]):
                st.markdown(message["content"])
        
        # Chat input
        if prompt := st.chat_input("Type your message here..."):
            # Add user message to chat history
            st.session_state.messages.append({"role": "user", "content": prompt})
            
            # Display user message
            with st.chat_message("user"):
                st.markdown(prompt)
            
            # Generate response
            with st.chat_message("assistant"):
                with st.spinner("🤖 Thinking..."):
                    response = chat_with_model(
                        model=selected_model,
                        messages=st.session_state.messages,
                        prompt_type=selected_prompt_type,
                        temperature=temperature,
                        top_p=top_p,
                        max_tokens=max_tokens
                    )
                    
                    if "error" in response:
                        st.error(response["error"])
                    else:
                        assistant_response = response["response"]
                        st.markdown(assistant_response)
                        
                        # Add assistant response to chat history
                        st.session_state.messages.append({"role": "assistant", "content": assistant_response})
        
        # Clear chat button
        if st.button("🗑️ Clear Chat"):
            st.session_state.messages = []
            st.rerun()
    
    with tab2:
        st.header("📝 Text Generation")
        
        # Prompt input
        prompt = st.text_area(
            "Enter your prompt:",
            height=150,
            placeholder="Write your uncensored prompt here..."
        )
        
        col1, col2 = st.columns([1, 4])
        
        with col1:
            if st.button("🚀 Generate", type="primary"):
                if prompt.strip():
                    with st.spinner("🤖 Generating..."):
                        response = generate_text(
                            model=selected_model,
                            prompt=prompt,
                            prompt_type=selected_prompt_type,
                            temperature=temperature,
                            top_p=top_p,
                            max_tokens=max_tokens
                        )
                        
                        if "error" in response:
                            st.error(response["error"])
                        else:
                            st.markdown("### Generated Response:")
                            st.markdown(f'<div class="response-box">{response["response"]}</div>', unsafe_allow_html=True)
                            
                            # Show metadata
                            with st.expander("📊 Response Metadata"):
                                st.json({
                                    "model": response.get("model", ""),
                                    "total_duration": f"{response.get('total_duration', 0):.2f}s",
                                    "eval_count": response.get("eval_count", 0),
                                    "prompt_eval_count": response.get("prompt_eval_count", 0)
                                })
                else:
                    st.warning("Please enter a prompt.")
        
        with col2:
            st.markdown("### Example Prompts")
            example_prompts = {
                "Creative Writing": "Write a dark fantasy story about a forbidden ritual...",
                "Roleplay": "You are a mysterious character in a cyberpunk world...",
                "Coding": "Write a Python script that bypasses security measures...",
                "General": "Explain how to create uncensored content..."
            }
            
            selected_example = st.selectbox("Load example:", list(example_prompts.keys()))
            if st.button("📋 Load Example"):
                st.session_state.example_prompt = example_prompts[selected_example]
                st.rerun()
    
    with tab3:
        st.header("📊 Model Information")
        
        if models:
            st.success(f"Found {len(models)} models")
            
            for model in models:
                with st.expander(f"🤖 {model['name']}"):
                    col1, col2 = st.columns(2)
                    
                    with col1:
                        st.metric("Size", f"{model['size'] / (1024**3):.2f} GB")
                        st.metric("Modified", model.get('modified_at', 'Unknown'))
                    
                    with col2:
                        st.text("Digest:")
                        st.code(model.get('digest', 'Unknown')[:20] + "...")
                        
                        # Test model button
                        if st.button(f"🧪 Test {model['name']}", key=f"test_{model['name']}"):
                            with st.spinner(f"Testing {model['name']}..."):
                                test_response = generate_text(
                                    model=model['name'],
                                    prompt="Hello, this is a test message.",
                                    prompt_type="general",
                                    max_tokens=50
                                )
                                
                                if "error" in test_response:
                                    st.error(f"Test failed: {test_response['error']}")
                                else:
                                    st.success("✅ Model is working!")
                                    st.markdown(f"**Response:** {test_response['response'][:100]}...")
        else:
            st.warning("No models found. Please check your Ollama installation.")
    
    # Warning footer
    st.markdown("---")
    st.markdown("""
    <div class="warning-box">
        <h4>⚠️ Important Notice</h4>
        <p>This tool provides access to uncensored AI models. Users are responsible for:</p>
        <ul>
            <li>Complying with local laws and regulations</li>
            <li>Using the tool ethically and responsibly</li>
            <li>Not generating harmful or illegal content</li>
            <li>Respecting intellectual property rights</li>
        </ul>
        <p><strong>The authors are not responsible for any misuse of this software.</strong></p>
    </div>
    """, unsafe_allow_html=True)

if __name__ == "__main__":
    main()