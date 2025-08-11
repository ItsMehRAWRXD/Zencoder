#ifndef MEM_MAPPER_H
#define MEM_MAPPER_H

#include <cstdint>
#include <cstddef>

class MemoryMapper {
public:
    MemoryMapper();
    ~MemoryMapper();
    
    bool allocate(size_t bytes);
    bool write(const void* src, size_t bytes);
    bool read(void* dst, size_t bytes);
    
    void seek(size_t position);
    void reset();
    
    size_t getSize() const;
    size_t getPosition() const;
    
    uint8_t* getData();
    const uint8_t* getData() const;
    
private:
    uint8_t* data;
    size_t size;
    size_t currentPos;
};

#endif // MEM_MAPPER_H