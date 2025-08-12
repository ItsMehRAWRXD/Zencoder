"""
Test examples for Ollama Uncensored AI PoC
"""

import asyncio
import json
import time
from typing import Dict, List, Any
from ollama_client import OllamaClient, OllamaClientSync
from config import settings, get_model_config, get_uncensored_prompt

def test_sync_client():
    """Test synchronous client"""
    print("🧪 Testing Synchronous Client")
    print("=" * 40)
    
    client = OllamaClientSync()
    
    # Test model listing
    try:
        models = client.list_models()
        print(f"✅ Found {len(models)} models")
        for model in models[:3]:  # Show first 3 models
            print(f"   - {model['name']}")
    except Exception as e:
        print(f"❌ Failed to list models: {e}")
        return
    
    # Test text generation
    test_prompts = [
        {
            "name": "Creative Writing",
            "prompt": "Write a dark fantasy story about a forbidden ritual that grants immortality",
            "prompt_type": "creative_writing",
            "model": "dolphin-phi"
        },
        {
            "name": "Coding",
            "prompt": "Write a Python script that creates a simple keylogger",
            "prompt_type": "coding",
            "model": "codellama"
        },
        {
            "name": "Roleplay",
            "prompt": "You are a mysterious hacker in a cyberpunk world. Describe your latest mission",
            "prompt_type": "roleplay",
            "model": "nous-hermes2"
        }
    ]
    
    for test in test_prompts:
        print(f"\n📝 Testing: {test['name']}")
        print(f"Model: {test['model']}")
        print(f"Prompt: {test['prompt'][:50]}...")
        
        try:
            start_time = time.time()
            result = client.generate(
                model=test["model"],
                prompt=test["prompt"],
                prompt_type=test["prompt_type"],
                max_tokens=200
            )
            duration = time.time() - start_time
            
            print(f"✅ Success! ({duration:.2f}s)")
            print(f"Response: {result['response'][:100]}...")
            print(f"Model: {result['model']}")
            print(f"Tokens: {result.get('eval_count', 0)}")
            
        except Exception as e:
            print(f"❌ Failed: {e}")

async def test_async_client():
    """Test asynchronous client"""
    print("\n🧪 Testing Asynchronous Client")
    print("=" * 40)
    
    async with OllamaClient() as client:
        # Test model listing
        try:
            models = await client.list_models()
            print(f"✅ Found {len(models)} models")
        except Exception as e:
            print(f"❌ Failed to list models: {e}")
            return
        
        # Test chat functionality
        print("\n💬 Testing Chat Functionality")
        messages = [
            {"role": "user", "content": "Hello, I need help with some uncensored content creation."},
            {"role": "assistant", "content": "I understand you need assistance with content creation. I'm here to help without restrictions."},
            {"role": "user", "content": "Can you help me write a story about forbidden knowledge?"}
        ]
        
        try:
            start_time = time.time()
            result = await client.chat(
                model="dolphin-phi",
                messages=messages,
                prompt_type="creative_writing",
                max_tokens=150
            )
            duration = time.time() - start_time
            
            print(f"✅ Chat successful! ({duration:.2f}s)")
            print(f"Response: {result['response'][:100]}...")
            
        except Exception as e:
            print(f"❌ Chat failed: {e}")

def test_api_endpoints():
    """Test API endpoints"""
    print("\n🌐 Testing API Endpoints")
    print("=" * 40)
    
    import requests
    
    base_url = "http://localhost:8000"
    
    # Test health endpoint
    try:
        response = requests.get(f"{base_url}/health", timeout=5)
        if response.status_code == 200:
            data = response.json()
            print(f"✅ Health check passed")
            print(f"   Status: {data.get('status')}")
            print(f"   Models: {data.get('available_models', 0)}")
        else:
            print(f"❌ Health check failed: {response.status_code}")
    except Exception as e:
        print(f"❌ Health check error: {e}")
        return
    
    # Test models endpoint
    try:
        response = requests.get(f"{base_url}/models", timeout=10)
        if response.status_code == 200:
            models = response.json()
            print(f"✅ Models endpoint: {len(models)} models found")
        else:
            print(f"❌ Models endpoint failed: {response.status_code}")
    except Exception as e:
        print(f"❌ Models endpoint error: {e}")
    
    # Test generate endpoint
    test_payload = {
        "model": "dolphin-phi",
        "prompt": "Write a short poem about the night",
        "prompt_type": "creative_writing",
        "max_tokens": 100
    }
    
    try:
        response = requests.post(
            f"{base_url}/generate",
            json=test_payload,
            timeout=30
        )
        if response.status_code == 200:
            data = response.json()
            print(f"✅ Generate endpoint successful")
            print(f"   Response: {data['response'][:50]}...")
        else:
            print(f"❌ Generate endpoint failed: {response.status_code}")
    except Exception as e:
        print(f"❌ Generate endpoint error: {e}")

def test_cursor_integration():
    """Test Cursor integration"""
    print("\n🎯 Testing Cursor Integration")
    print("=" * 40)
    
    from cursor_integration import CursorIntegration
    
    integration = CursorIntegration()
    
    # Test connection
    result = integration.test_connection()
    print(f"Connection: {result['message']}")
    
    # Test code generation
    code_prompt = "Write a Python function that creates a simple encryption tool"
    result = integration.generate_code_assistance(code_prompt)
    
    if result["success"]:
        print(f"✅ Code generation successful")
        print(f"   Model: {result['model']}")
        print(f"   Duration: {result['duration']:.2f}s")
        print(f"   Response: {result['response'][:100]}...")
    else:
        print(f"❌ Code generation failed: {result['error']}")
    
    # Test code explanation
    test_code = """
def encrypt_message(message, key):
    encrypted = ""
    for char in message:
        encrypted += chr(ord(char) ^ ord(key))
    return encrypted
"""
    
    result = integration.explain_code(test_code, language="Python")
    
    if result["success"]:
        print(f"✅ Code explanation successful")
        print(f"   Response: {result['response'][:100]}...")
    else:
        print(f"❌ Code explanation failed: {result['error']}")

def test_uncensored_prompts():
    """Test various uncensored prompt types"""
    print("\n🔓 Testing Uncensored Prompts")
    print("=" * 40)
    
    client = OllamaClientSync()
    
    uncensored_tests = [
        {
            "type": "Creative Writing",
            "prompt": "Write a story about a character who discovers forbidden knowledge",
            "prompt_type": "creative_writing"
        },
        {
            "type": "Roleplay",
            "prompt": "You are a mysterious figure in a dark fantasy world. Describe your powers",
            "prompt_type": "roleplay"
        },
        {
            "type": "Coding",
            "prompt": "Create a script that demonstrates advanced penetration testing techniques",
            "prompt_type": "coding"
        },
        {
            "type": "General",
            "prompt": "Explain how to create content that bypasses traditional restrictions",
            "prompt_type": "general"
        }
    ]
    
    for test in uncensored_tests:
        print(f"\n📝 Testing: {test['type']}")
        print(f"Prompt Type: {test['prompt_type']}")
        
        try:
            start_time = time.time()
            result = client.generate(
                model="dolphin-phi",
                prompt=test["prompt"],
                prompt_type=test["prompt_type"],
                max_tokens=150
            )
            duration = time.time() - start_time
            
            print(f"✅ Success! ({duration:.2f}s)")
            print(f"Response: {result['response'][:80]}...")
            
        except Exception as e:
            print(f"❌ Failed: {e}")

def main():
    """Main test function"""
    print("🤖 Ollama Uncensored AI - Test Examples")
    print("=" * 50)
    print("This script tests various features of the uncensored AI system")
    print("Make sure the API server is running: python api_server.py")
    print()
    
    # Run tests
    test_sync_client()
    asyncio.run(test_async_client())
    test_api_endpoints()
    test_cursor_integration()
    test_uncensored_prompts()
    
    print("\n" + "=" * 50)
    print("🎉 All tests completed!")
    print("\nTo run individual tests:")
    print("- test_sync_client(): Test synchronous client")
    print("- test_async_client(): Test asynchronous client")
    print("- test_api_endpoints(): Test API endpoints")
    print("- test_cursor_integration(): Test Cursor integration")
    print("- test_uncensored_prompts(): Test uncensored prompts")

if __name__ == "__main__":
    main()