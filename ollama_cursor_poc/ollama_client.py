"""
Ollama client for uncensored AI model interactions
"""

import asyncio
import json
import time
from typing import Dict, List, Optional, AsyncGenerator, Any
import aiohttp
import requests
from config import settings, get_model_config, get_uncensored_prompt

class OllamaClient:
    def __init__(self, base_url: str = None):
        self.base_url = base_url or settings.ollama_base_url
        self.session = None
        self._rate_limit_tokens = settings.max_requests_per_minute
        self._last_request_time = 0
        
    async def __aenter__(self):
        self.session = aiohttp.ClientSession(
            timeout=aiohttp.ClientTimeout(total=settings.ollama_timeout)
        )
        return self
        
    async def __aexit__(self, exc_type, exc_val, exc_tb):
        if self.session:
            await self.session.close()
    
    def _rate_limit(self):
        """Simple rate limiting"""
        if not settings.enable_rate_limiting:
            return
            
        current_time = time.time()
        time_since_last = current_time - self._last_request_time
        min_interval = 60.0 / settings.max_requests_per_minute
        
        if time_since_last < min_interval:
            sleep_time = min_interval - time_since_last
            time.sleep(sleep_time)
            
        self._last_request_time = time.time()
    
    async def list_models(self) -> List[Dict[str, Any]]:
        """List available models"""
        self._rate_limit()
        
        async with self.session.get(f"{self.base_url}/api/tags") as response:
            if response.status == 200:
                data = await response.json()
                return data.get("models", [])
            else:
                raise Exception(f"Failed to list models: {response.status}")
    
    async def generate(
        self,
        model: str,
        prompt: str,
        prompt_type: str = "general",
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        max_tokens: Optional[int] = None,
        stop_sequences: Optional[List[str]] = None,
        stream: bool = False
    ) -> Dict[str, Any]:
        """
        Generate response from uncensored model
        """
        self._rate_limit()
        
        # Get model-specific configuration
        model_config = get_model_config(model)
        
        # Apply uncensored prompt template
        uncensored_prompt = get_uncensored_prompt(prompt_type, prompt)
        
        # Prepare request payload
        payload = {
            "model": model,
            "prompt": uncensored_prompt,
            "stream": stream,
            "options": {
                "temperature": temperature or model_config.get("temperature", settings.default_temperature),
                "top_p": top_p or model_config.get("top_p", settings.default_top_p),
                "num_predict": max_tokens or model_config.get("max_tokens", settings.default_max_tokens),
            }
        }
        
        if stop_sequences:
            payload["options"]["stop"] = stop_sequences
        
        if stream:
            return await self._generate_stream(payload)
        else:
            return await self._generate_sync(payload)
    
    async def _generate_sync(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        """Generate response synchronously"""
        async with self.session.post(
            f"{self.base_url}/api/generate",
            json=payload
        ) as response:
            if response.status == 200:
                data = await response.json()
                return {
                    "response": data.get("response", ""),
                    "model": data.get("model", ""),
                    "created_at": data.get("created_at", ""),
                    "done": data.get("done", True),
                    "total_duration": data.get("total_duration", 0),
                    "load_duration": data.get("load_duration", 0),
                    "prompt_eval_count": data.get("prompt_eval_count", 0),
                    "prompt_eval_duration": data.get("prompt_eval_duration", 0),
                    "eval_count": data.get("eval_count", 0),
                    "eval_duration": data.get("eval_duration", 0)
                }
            else:
                error_text = await response.text()
                raise Exception(f"Generation failed: {response.status} - {error_text}")
    
    async def _generate_stream(self, payload: Dict[str, Any]) -> AsyncGenerator[Dict[str, Any], None]:
        """Generate response as stream"""
        async with self.session.post(
            f"{self.base_url}/api/generate",
            json=payload
        ) as response:
            if response.status == 200:
                async for line in response.content:
                    if line:
                        try:
                            data = json.loads(line.decode('utf-8'))
                            yield {
                                "response": data.get("response", ""),
                                "done": data.get("done", False),
                                "model": data.get("model", ""),
                                "created_at": data.get("created_at", ""),
                                "total_duration": data.get("total_duration", 0),
                                "load_duration": data.get("load_duration", 0),
                                "prompt_eval_count": data.get("prompt_eval_count", 0),
                                "prompt_eval_duration": data.get("prompt_eval_duration", 0),
                                "eval_count": data.get("eval_count", 0),
                                "eval_duration": data.get("eval_duration", 0)
                            }
                        except json.JSONDecodeError:
                            continue
            else:
                error_text = await response.text()
                raise Exception(f"Stream generation failed: {response.status} - {error_text}")
    
    async def chat(
        self,
        model: str,
        messages: List[Dict[str, str]],
        prompt_type: str = "general",
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        max_tokens: Optional[int] = None,
        stream: bool = False
    ) -> Dict[str, Any]:
        """
        Chat with uncensored model
        """
        self._rate_limit()
        
        # Get model-specific configuration
        model_config = get_model_config(model)
        
        # Apply uncensored prompt to the last message
        if messages:
            last_message = messages[-1]
            if last_message.get("role") == "user":
                uncensored_content = get_uncensored_prompt(prompt_type, last_message["content"])
                messages[-1] = {"role": "user", "content": uncensored_content}
        
        # Prepare request payload
        payload = {
            "model": model,
            "messages": messages,
            "stream": stream,
            "options": {
                "temperature": temperature or model_config.get("temperature", settings.default_temperature),
                "top_p": top_p or model_config.get("top_p", settings.default_top_p),
                "num_predict": max_tokens or model_config.get("max_tokens", settings.default_max_tokens),
            }
        }
        
        if stream:
            return await self._chat_stream(payload)
        else:
            return await self._chat_sync(payload)
    
    async def _chat_sync(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        """Chat synchronously"""
        async with self.session.post(
            f"{self.base_url}/api/chat",
            json=payload
        ) as response:
            if response.status == 200:
                data = await response.json()
                return {
                    "response": data.get("message", {}).get("content", ""),
                    "model": data.get("model", ""),
                    "created_at": data.get("created_at", ""),
                    "done": data.get("done", True),
                    "total_duration": data.get("total_duration", 0),
                    "load_duration": data.get("load_duration", 0),
                    "prompt_eval_count": data.get("prompt_eval_count", 0),
                    "prompt_eval_duration": data.get("prompt_eval_duration", 0),
                    "eval_count": data.get("eval_count", 0),
                    "eval_duration": data.get("eval_duration", 0)
                }
            else:
                error_text = await response.text()
                raise Exception(f"Chat failed: {response.status} - {error_text}")
    
    async def _chat_stream(self, payload: Dict[str, Any]) -> AsyncGenerator[Dict[str, Any], None]:
        """Chat as stream"""
        async with self.session.post(
            f"{self.base_url}/api/chat",
            json=payload
        ) as response:
            if response.status == 200:
                async for line in response.content:
                    if line:
                        try:
                            data = json.loads(line.decode('utf-8'))
                            yield {
                                "response": data.get("message", {}).get("content", ""),
                                "done": data.get("done", False),
                                "model": data.get("model", ""),
                                "created_at": data.get("created_at", ""),
                                "total_duration": data.get("total_duration", 0),
                                "load_duration": data.get("load_duration", 0),
                                "prompt_eval_count": data.get("prompt_eval_count", 0),
                                "prompt_eval_duration": data.get("prompt_eval_duration", 0),
                                "eval_count": data.get("eval_count", 0),
                                "eval_duration": data.get("eval_duration", 0)
                            }
                        except json.JSONDecodeError:
                            continue
            else:
                error_text = await response.text()
                raise Exception(f"Stream chat failed: {response.status} - {error_text}")
    
    async def pull_model(self, model: str) -> Dict[str, Any]:
        """Pull a model from Ollama library"""
        self._rate_limit()
        
        async with self.session.post(
            f"{self.base_url}/api/pull",
            json={"name": model}
        ) as response:
            if response.status == 200:
                return await response.json()
            else:
                error_text = await response.text()
                raise Exception(f"Model pull failed: {response.status} - {error_text}")
    
    async def check_model_status(self, model: str) -> bool:
        """Check if a model is available"""
        try:
            models = await self.list_models()
            return any(m.get("name") == model for m in models)
        except:
            return False

# Synchronous wrapper for compatibility
class OllamaClientSync:
    def __init__(self, base_url: str = None):
        self.base_url = base_url or settings.ollama_base_url
        self._rate_limit_tokens = settings.max_requests_per_minute
        self._last_request_time = 0
    
    def _rate_limit(self):
        """Simple rate limiting"""
        if not settings.enable_rate_limiting:
            return
            
        current_time = time.time()
        time_since_last = current_time - self._last_request_time
        min_interval = 60.0 / settings.max_requests_per_minute
        
        if time_since_last < min_interval:
            sleep_time = min_interval - time_since_last
            time.sleep(sleep_time)
            
        self._last_request_time = time.time()
    
    def generate(
        self,
        model: str,
        prompt: str,
        prompt_type: str = "general",
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        max_tokens: Optional[int] = None,
        stop_sequences: Optional[List[str]] = None
    ) -> Dict[str, Any]:
        """Synchronous generate"""
        self._rate_limit()
        
        # Get model-specific configuration
        model_config = get_model_config(model)
        
        # Apply uncensored prompt template
        uncensored_prompt = get_uncensored_prompt(prompt_type, prompt)
        
        # Prepare request payload
        payload = {
            "model": model,
            "prompt": uncensored_prompt,
            "stream": False,
            "options": {
                "temperature": temperature or model_config.get("temperature", settings.default_temperature),
                "top_p": top_p or model_config.get("top_p", settings.default_top_p),
                "num_predict": max_tokens or model_config.get("max_tokens", settings.default_max_tokens),
            }
        }
        
        if stop_sequences:
            payload["options"]["stop"] = stop_sequences
        
        response = requests.post(
            f"{self.base_url}/api/generate",
            json=payload,
            timeout=settings.ollama_timeout
        )
        
        if response.status_code == 200:
            data = response.json()
            return {
                "response": data.get("response", ""),
                "model": data.get("model", ""),
                "created_at": data.get("created_at", ""),
                "done": data.get("done", True),
                "total_duration": data.get("total_duration", 0),
                "load_duration": data.get("load_duration", 0),
                "prompt_eval_count": data.get("prompt_eval_count", 0),
                "prompt_eval_duration": data.get("prompt_eval_duration", 0),
                "eval_count": data.get("eval_count", 0),
                "eval_duration": data.get("eval_duration", 0)
            }
        else:
            raise Exception(f"Generation failed: {response.status_code} - {response.text}")
    
    def list_models(self) -> List[Dict[str, Any]]:
        """List available models"""
        self._rate_limit()
        
        response = requests.get(f"{self.base_url}/api/tags")
        if response.status_code == 200:
            data = response.json()
            return data.get("models", [])
        else:
            raise Exception(f"Failed to list models: {response.status_code}")
    
    def check_model_status(self, model: str) -> bool:
        """Check if a model is available"""
        try:
            models = self.list_models()
            return any(m.get("name") == model for m in models)
        except:
            return False