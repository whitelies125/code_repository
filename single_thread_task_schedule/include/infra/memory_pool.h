#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstdint>

#include "index.h"

class Block {
public:
    ~Block();
    uint32_t Init(uint32_t num, uint32_t size);
    void* Alloc(uint32_t size);
    bool Free(void* addr);

private:
    uint32_t size_;
    uint8_t* buff_;
    Index<uint32_t> index;  // 使用 id分配器控制内存块
};

struct BlockInfo {
    uint32_t num;
    uint32_t size;
};

class MemPool {
public:
    ~MemPool();
    uint32_t Init(BlockInfo* para, uint32_t paraLen);
    void* Alloc(size_t size);
    void Free(void* addr);

private:
    uint32_t len_;
    Block* blocks_;
};

#endif
