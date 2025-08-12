from fastapi import FastAPI, WebSocket, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
import ollama
import asyncio
import json
from typing import List, Optional
import os
from pathlib import Path

app = FastAPI(title="Ollama Cursor POC", version="1.0.0")

# Configure CORS for Cursor integration
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # In production, specify exact origins
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Configuration for uncensored models
UNCENSORED_MODELS = [
    "dolphin-phi",  # Uncensored Phi model
    "dolphin-mistral",  # Uncensored Mistral model
    "wizard-vicuna-uncensored",  # Uncensored Vicuna model
    "llama2-uncensored",  # Uncensored Llama2
]

# Request/Response models
class ChatRequest(BaseModel):
    model: str
    prompt: str
    system_prompt: Optional[str] = None
    temperature: Optional[float] = 0.7
    max_tokens: Optional[int] = 2048

class ModelInfo(BaseModel):
    name: str
    size: str
    modified: str
    digest: str

class ChatResponse(BaseModel):
    response: str
    model: str
    tokens_used: Optional[int] = None

# Check if Ollama is running
async def check_ollama_status():
    try:
        models = ollama.list()
        return True
    except Exception as e:
        return False

@app.get("/")
async def root():
    return {
        "message": "Ollama Cursor POC API",
        "status": "running",
        "ollama_connected": await check_ollama_status()
    }

@app.get("/models", response_model=List[ModelInfo])
async def list_models():
    """List all available Ollama models"""
    try:
        models = ollama.list()
        return [
            ModelInfo(
                name=model['name'],
                size=model.get('size', 'Unknown'),
                modified=model.get('modified_at', 'Unknown'),
                digest=model.get('digest', 'Unknown')
            )
            for model in models['models']
        ]
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Failed to list models: {str(e)}")

@app.get("/uncensored-models")
async def list_uncensored_models():
    """List recommended uncensored models"""
    return {
        "models": UNCENSORED_MODELS,
        "note": "These models may need to be pulled first using 'ollama pull <model-name>'"
    }

@app.post("/chat", response_model=ChatResponse)
async def chat(request: ChatRequest):
    """Send a chat message to Ollama"""
    try:
        # Prepare messages
        messages = []
        
        if request.system_prompt:
            messages.append({
                'role': 'system',
                'content': request.system_prompt
            })
        
        messages.append({
            'role': 'user',
            'content': request.prompt
        })
        
        # Call Ollama
        response = ollama.chat(
            model=request.model,
            messages=messages,
            options={
                'temperature': request.temperature,
                'num_predict': request.max_tokens,
            }
        )
        
        return ChatResponse(
            response=response['message']['content'],
            model=request.model,
            tokens_used=response.get('eval_count')
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Chat failed: {str(e)}")

@app.websocket("/ws/chat")
async def websocket_chat(websocket: WebSocket):
    """WebSocket endpoint for streaming chat responses"""
    await websocket.accept()
    
    try:
        while True:
            # Receive message
            data = await websocket.receive_json()
            
            model = data.get('model', 'llama2')
            prompt = data.get('prompt', '')
            system_prompt = data.get('system_prompt', None)
            temperature = data.get('temperature', 0.7)
            
            # Prepare messages
            messages = []
            if system_prompt:
                messages.append({'role': 'system', 'content': system_prompt})
            messages.append({'role': 'user', 'content': prompt})
            
            # Stream response
            stream = ollama.chat(
                model=model,
                messages=messages,
                stream=True,
                options={
                    'temperature': temperature,
                }
            )
            
            # Send chunks as they arrive
            full_response = ""
            for chunk in stream:
                if 'message' in chunk and 'content' in chunk['message']:
                    content = chunk['message']['content']
                    full_response += content
                    await websocket.send_json({
                        'type': 'chunk',
                        'content': content
                    })
            
            # Send completion signal
            await websocket.send_json({
                'type': 'complete',
                'full_response': full_response
            })
            
    except Exception as e:
        await websocket.send_json({
            'type': 'error',
            'message': str(e)
        })
    finally:
        await websocket.close()

@app.post("/pull-model")
async def pull_model(model_name: str):
    """Pull a new model from Ollama registry"""
    try:
        # This will be a long-running operation
        ollama.pull(model_name)
        return {"status": "success", "model": model_name}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Failed to pull model: {str(e)}")

# Serve static files if frontend is built
frontend_path = Path(__file__).parent.parent / "frontend"
if frontend_path.exists():
    app.mount("/static", StaticFiles(directory=str(frontend_path), html=True), name="static")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)