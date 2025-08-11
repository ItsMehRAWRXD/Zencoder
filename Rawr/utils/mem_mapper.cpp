#include "mem_mapper.h"
#include <cstring>

MemoryMapper::MemoryMapper() : data(nullptr), size(0), currentPos(0) {
}

MemoryMapper::~MemoryMapper() {
    if (data) {
        delete[] data;
    }
}

bool MemoryMapper::allocate(size_t bytes) {
    if (data) {
        delete[] data;
    }
    
    data = new uint8_t[bytes];
    if (!data) {
        return false;
    }
    
    size = bytes;
    currentPos = 0;
    std::memset(data, 0, size);
    return true;
}

bool MemoryMapper::write(const void* src, size_t bytes) {
    if (!data || currentPos + bytes > size) {
        return false;
    }
    
    std::memcpy(data + currentPos, src, bytes);
    currentPos += bytes;
    return true;
}

bool MemoryMapper::read(void* dst, size_t bytes) {
    if (!data || currentPos + bytes > size) {
        return false;
    }
    
    std::memcpy(dst, data + currentPos, bytes);
    currentPos += bytes;
    return true;
}

void MemoryMapper::seek(size_t position) {
    if (position <= size) {
        currentPos = position;
    }
}

void MemoryMapper::reset() {
    currentPos = 0;
}

size_t MemoryMapper::getSize() const {
    return size;
}

size_t MemoryMapper::getPosition() const {
    return currentPos;
}

uint8_t* MemoryMapper::getData() {
    return data;
}

const uint8_t* MemoryMapper::getData() const {
    return data;
}