"""
FastAPI server for Ollama + Cursor uncensored AI integration
"""

import asyncio
import json
import time
from typing import Dict, List, Optional, Any
from fastapi import FastAPI, HTTPException, Request, BackgroundTasks
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse
from pydantic import BaseModel, Field
import uvicorn

from config import settings, get_model_config, get_uncensored_prompt
from ollama_client import OllamaClient, OllamaClientSync

# Initialize FastAPI app
app = FastAPI(
    title="Ollama Uncensored AI API",
    description="API for uncensored AI models via Ollama",
    version="1.0.0",
    docs_url="/docs",
    redoc_url="/redoc"
)

# Add CORS middleware
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Configure appropriately for production
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Pydantic models for request/response
class GenerateRequest(BaseModel):
    model: str = Field(..., description="Model name to use")
    prompt: str = Field(..., description="Input prompt")
    prompt_type: str = Field("general", description="Type of prompt (general, creative_writing, roleplay, coding)")
    temperature: Optional[float] = Field(None, description="Temperature for generation")
    top_p: Optional[float] = Field(None, description="Top-p sampling")
    max_tokens: Optional[int] = Field(None, description="Maximum tokens to generate")
    stop_sequences: Optional[List[str]] = Field(None, description="Stop sequences")
    stream: bool = Field(False, description="Enable streaming response")

class ChatRequest(BaseModel):
    model: str = Field(..., description="Model name to use")
    messages: List[Dict[str, str]] = Field(..., description="Chat messages")
    prompt_type: str = Field("general", description="Type of prompt")
    temperature: Optional[float] = Field(None, description="Temperature for generation")
    top_p: Optional[float] = Field(None, description="Top-p sampling")
    max_tokens: Optional[int] = Field(None, description="Maximum tokens to generate")
    stream: bool = Field(False, description="Enable streaming response")

class GenerateResponse(BaseModel):
    response: str
    model: str
    created_at: str
    done: bool
    total_duration: float
    load_duration: float
    prompt_eval_count: int
    prompt_eval_duration: float
    eval_count: int
    eval_duration: float

class ModelInfo(BaseModel):
    name: str
    size: int
    modified_at: str
    digest: str

# Rate limiting
class RateLimiter:
    def __init__(self):
        self.requests = {}
    
    def is_allowed(self, client_ip: str) -> bool:
        if not settings.enable_rate_limiting:
            return True
            
        current_time = time.time()
        if client_ip not in self.requests:
            self.requests[client_ip] = []
        
        # Remove old requests
        self.requests[client_ip] = [
            req_time for req_time in self.requests[client_ip]
            if current_time - req_time < 60
        ]
        
        # Check if under limit
        if len(self.requests[client_ip]) < settings.max_requests_per_minute:
            self.requests[client_ip].append(current_time)
            return True
        
        return False

rate_limiter = RateLimiter()

def get_client_ip(request: Request) -> str:
    """Get client IP address"""
    forwarded = request.headers.get("X-Forwarded-For")
    if forwarded:
        return forwarded.split(",")[0]
    return request.client.host

@app.middleware("http")
async def rate_limit_middleware(request: Request, call_next):
    """Rate limiting middleware"""
    client_ip = get_client_ip(request)
    
    if not rate_limiter.is_allowed(client_ip):
        raise HTTPException(
            status_code=429,
            detail="Rate limit exceeded. Please try again later."
        )
    
    response = await call_next(request)
    return response

@app.get("/")
async def root():
    """Root endpoint"""
    return {
        "message": "Ollama Uncensored AI API",
        "version": "1.0.0",
        "status": "running",
        "models": settings.default_models
    }

@app.get("/health")
async def health_check():
    """Health check endpoint"""
    try:
        async with OllamaClient() as client:
            models = await client.list_models()
            return {
                "status": "healthy",
                "ollama_connected": True,
                "available_models": len(models),
                "timestamp": time.time()
            }
    except Exception as e:
        return {
            "status": "unhealthy",
            "ollama_connected": False,
            "error": str(e),
            "timestamp": time.time()
        }

@app.get("/models", response_model=List[ModelInfo])
async def list_models():
    """List available models"""
    try:
        async with OllamaClient() as client:
            models = await client.list_models()
            return [
                ModelInfo(
                    name=model.get("name", ""),
                    size=model.get("size", 0),
                    modified_at=model.get("modified_at", ""),
                    digest=model.get("digest", "")
                )
                for model in models
            ]
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Failed to list models: {str(e)}")

@app.post("/generate", response_model=GenerateResponse)
async def generate_text(request: GenerateRequest):
    """Generate text using uncensored model"""
    try:
        async with OllamaClient() as client:
            result = await client.generate(
                model=request.model,
                prompt=request.prompt,
                prompt_type=request.prompt_type,
                temperature=request.temperature,
                top_p=request.top_p,
                max_tokens=request.max_tokens,
                stop_sequences=request.stop_sequences,
                stream=request.stream
            )
            
            if request.stream:
                # Return streaming response
                return StreamingResponse(
                    stream_response(result),
                    media_type="text/plain"
                )
            else:
                return GenerateResponse(**result)
                
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Generation failed: {str(e)}")

@app.post("/chat")
async def chat(request: ChatRequest):
    """Chat with uncensored model"""
    try:
        async with OllamaClient() as client:
            result = await client.chat(
                model=request.model,
                messages=request.messages,
                prompt_type=request.prompt_type,
                temperature=request.temperature,
                top_p=request.top_p,
                max_tokens=request.max_tokens,
                stream=request.stream
            )
            
            if request.stream:
                # Return streaming response
                return StreamingResponse(
                    stream_chat_response(result),
                    media_type="text/plain"
                )
            else:
                return GenerateResponse(**result)
                
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Chat failed: {str(e)}")

async def stream_response(generator):
    """Stream response generator"""
    async for chunk in generator:
        yield f"data: {json.dumps(chunk)}\n\n"
    yield "data: [DONE]\n\n"

async def stream_chat_response(generator):
    """Stream chat response generator"""
    async for chunk in generator:
        yield f"data: {json.dumps(chunk)}\n\n"
    yield "data: [DONE]\n\n"

# Cursor-specific endpoints for better integration
@app.post("/v1/chat/completions")
async def cursor_chat_completions(request: Dict[str, Any]):
    """Cursor-compatible chat completions endpoint"""
    try:
        # Extract parameters from Cursor request
        model = request.get("model", "dolphin-phi")
        messages = request.get("messages", [])
        temperature = request.get("temperature", 0.7)
        max_tokens = request.get("max_tokens", 2048)
        stream = request.get("stream", False)
        
        # Convert to our format
        chat_request = ChatRequest(
            model=model,
            messages=messages,
            temperature=temperature,
            max_tokens=max_tokens,
            stream=stream
        )
        
        # Process chat request
        async with OllamaClient() as client:
            result = await client.chat(
                model=chat_request.model,
                messages=chat_request.messages,
                temperature=chat_request.temperature,
                max_tokens=chat_request.max_tokens,
                stream=chat_request.stream
            )
            
            if stream:
                return StreamingResponse(
                    stream_cursor_response(result),
                    media_type="text/plain"
                )
            else:
                # Format response for Cursor
                return {
                    "id": f"chatcmpl-{int(time.time())}",
                    "object": "chat.completion",
                    "created": int(time.time()),
                    "model": model,
                    "choices": [{
                        "index": 0,
                        "message": {
                            "role": "assistant",
                            "content": result["response"]
                        },
                        "finish_reason": "stop"
                    }],
                    "usage": {
                        "prompt_tokens": result.get("prompt_eval_count", 0),
                        "completion_tokens": result.get("eval_count", 0),
                        "total_tokens": result.get("prompt_eval_count", 0) + result.get("eval_count", 0)
                    }
                }
                
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Chat completion failed: {str(e)}")

async def stream_cursor_response(generator):
    """Stream Cursor-compatible response"""
    async for chunk in generator:
        cursor_chunk = {
            "id": f"chatcmpl-{int(time.time())}",
            "object": "chat.completion.chunk",
            "created": int(time.time()),
            "choices": [{
                "index": 0,
                "delta": {
                    "content": chunk["response"]
                },
                "finish_reason": None if not chunk["done"] else "stop"
            }]
        }
        yield f"data: {json.dumps(cursor_chunk)}\n\n"
    yield "data: [DONE]\n\n"

@app.post("/pull")
async def pull_model(model_name: str):
    """Pull a model from Ollama library"""
    try:
        async with OllamaClient() as client:
            result = await client.pull_model(model_name)
            return {"status": "success", "model": model_name, "result": result}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Model pull failed: {str(e)}")

@app.get("/models/{model_name}/status")
async def check_model_status(model_name: str):
    """Check if a specific model is available"""
    try:
        async with OllamaClient() as client:
            is_available = await client.check_model_status(model_name)
            return {
                "model": model_name,
                "available": is_available,
                "status": "available" if is_available else "not_found"
            }
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Status check failed: {str(e)}")

# Background task to check Ollama connection
@app.on_event("startup")
async def startup_event():
    """Startup event handler"""
    print("🚀 Starting Ollama Uncensored AI API Server")
    print(f"📡 Server will run on {settings.host}:{settings.port}")
    print(f"🔗 Ollama URL: {settings.ollama_base_url}")
    print(f"🤖 Default models: {', '.join(settings.default_models)}")
    
    # Check Ollama connection
    try:
        async with OllamaClient() as client:
            models = await client.list_models()
            print(f"✅ Connected to Ollama. Found {len(models)} models.")
    except Exception as e:
        print(f"⚠️  Warning: Could not connect to Ollama: {e}")
        print("   Make sure Ollama is running: ollama serve")

@app.on_event("shutdown")
async def shutdown_event():
    """Shutdown event handler"""
    print("🛑 Shutting down Ollama Uncensored AI API Server")

if __name__ == "__main__":
    uvicorn.run(
        "api_server:app",
        host=settings.host,
        port=settings.port,
        reload=settings.debug,
        log_level="info"
    )