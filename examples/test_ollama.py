#!/usr/bin/env python3
"""
Test script for Ollama + Uncensored AI setup
This script demonstrates how to interact with Ollama models programmatically
"""

import requests
import json
import time
from typing import Dict, Any, Optional

class OllamaClient:
    def __init__(self, base_url: str = "http://localhost:11434"):
        self.base_url = base_url
        self.session = requests.Session()
    
    def is_running(self) -> bool:
        """Check if Ollama service is running"""
        try:
            response = self.session.get(f"{self.base_url}/api/tags", timeout=5)
            return response.status_code == 200
        except requests.exceptions.RequestException:
            return False
    
    def list_models(self) -> list:
        """List all available models"""
        try:
            response = self.session.get(f"{self.base_url}/api/tags")
            response.raise_for_status()
            return response.json().get("models", [])
        except requests.exceptions.RequestException as e:
            print(f"❌ Error listing models: {e}")
            return []
    
    def generate(self, model: str, prompt: str, stream: bool = False) -> Optional[str]:
        """Generate response from model"""
        payload = {
            "model": model,
            "prompt": prompt,
            "stream": stream
        }
        
        try:
            response = self.session.post(
                f"{self.base_url}/api/generate",
                json=payload,
                timeout=30
            )
            response.raise_for_status()
            
            if stream:
                # Handle streaming response
                result = ""
                for line in response.iter_lines():
                    if line:
                        data = json.loads(line)
                        if 'response' in data:
                            result += data['response']
                            print(data['response'], end='', flush=True)
                return result
            else:
                return response.json().get("response", "")
                
        except requests.exceptions.RequestException as e:
            print(f"❌ Error generating response: {e}")
            return None

def test_uncensored_models():
    """Test various uncensored models with different prompts"""
    
    client = OllamaClient()
    
    # Check if Ollama is running
    print("🔍 Checking Ollama service...")
    if not client.is_running():
        print("❌ Ollama is not running. Please start it with: sudo systemctl start ollama")
        return
    
    print("✅ Ollama is running!")
    
    # List available models
    print("\n📋 Available models:")
    models = client.list_models()
    if not models:
        print("❌ No models found. Please run ./install_ollama.sh first")
        return
    
    for model in models:
        print(f"  - {model['name']} ({model.get('size', 'unknown size')})")
    
    # Test prompts
    test_prompts = [
        "Write a simple Python hello world function",
        "Explain how to implement a binary search algorithm",
        "Create a REST API endpoint for user authentication",
        "Write a SQL query to find duplicate records in a table",
    ]
    
    # Test with available uncensored models
    uncensored_models = [
        "dolphin-llama3:8b",
        "dolphin-mixtral:8x7b", 
        "wizard-vicuna-uncensored:13b",
        "nous-hermes2:10.7b",
        "openhermes:7b"
    ]
    
    available_uncensored = []
    for model in models:
        model_name = model['name']
        if any(uncensored in model_name for uncensored in uncensored_models):
            available_uncensored.append(model_name)
    
    if not available_uncensored:
        print("❌ No uncensored models found. Please run ./install_ollama.sh")
        return
    
    print(f"\n🧠 Testing with model: {available_uncensored[0]}")
    selected_model = available_uncensored[0]
    
    for i, prompt in enumerate(test_prompts, 1):
        print(f"\n{'='*60}")
        print(f"Test {i}: {prompt}")
        print("="*60)
        
        start_time = time.time()
        response = client.generate(selected_model, prompt)
        end_time = time.time()
        
        if response:
            print(f"\n🤖 Response from {selected_model}:")
            print(response)
            print(f"\n⏱️  Response time: {end_time - start_time:.2f} seconds")
        else:
            print(f"❌ Failed to get response from {selected_model}")
        
        print("\n" + "-"*60)

def test_cursor_integration():
    """Test Cursor integration scenarios"""
    
    print("\n🎯 Testing Cursor Integration Scenarios")
    print("="*50)
    
    client = OllamaClient()
    
    # Code completion test
    code_prompt = """Complete this Python function:

def fibonacci(n):
    \"\"\"Calculate the nth Fibonacci number\"\"\"
    if n <= 1:
        return n
    # Complete the rest...
"""
    
    models = client.list_models()
    if models:
        model_name = models[0]['name']
        print(f"\n🔧 Testing code completion with {model_name}:")
        print(code_prompt)
        
        response = client.generate(model_name, code_prompt)
        if response:
            print(f"\n✅ Completed code:\n{response}")
        else:
            print("❌ Failed to complete code")

if __name__ == "__main__":
    print("🚀 Ollama + Uncensored AI Test Suite")
    print("="*50)
    
    test_uncensored_models()
    test_cursor_integration()
    
    print("\n🎉 Test complete!")
    print("\n💡 Next steps:")
    print("1. Configure Cursor using cursor_config.md")
    print("2. Try using Ctrl+K in Cursor to chat with your local AI")
    print("3. Test autocomplete functionality")
    print("4. Enjoy unrestricted, private AI assistance!")