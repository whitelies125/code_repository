#include "memory_pool.h"

Block::~Block() { delete[] buff_; }
uint32_t Block::Init(uint32_t num, uint32_t size)
{
    size_t total = num * size;
    buff_ = new uint8_t[total];
    size_ = size;
    return index.Init(num);
}
void* Block::Alloc(uint32_t size)
{
    if (size > size_) return nullptr;
    uint32_t id = index.AllocId();  // 申请一个未使用的 id
    if (id == UINT32_MAX) return nullptr;
    uint32_t offset = id * size_;  // 偏移 = id * 内存块大小
    return buff_ + offset;         // 返回 第 id 个内存块的首地址
}
bool Block::Free(void* addr)
{
    uint32_t id = (uint32_t)((uint8_t*)addr - buff_) / size_;  // id = (addr - 内存池首地址) / 内存块大小
                                                     //
    return index.FreeId(id);                         // 只需要 id分配器释放 id 为可用即可
}

MemPool::~MemPool() { delete[] blocks_; }
uint32_t MemPool::Init(BlockInfo* para, uint32_t paraLen)
{
    blocks_ = new Block[paraLen];
    for (uint32_t i = 0; i < paraLen; ++i) {
        blocks_[i].Init(para[i].num, para[i].size);
    }
    len_ = paraLen;
    return 0;
}
void* MemPool::Alloc(size_t size)
{
    for (uint32_t i = 0; i < len_; ++i) {
        if (void* ptr = blocks_[i].Alloc(size); ptr != nullptr) {
            return ptr;
        }
    }
    return nullptr;
}
void MemPool::Free(void* addr)
{
    for (uint32_t i = 0; i < len_; ++i) {
        if (!blocks_[i].Free(addr)) {
            return;
        }
    }
    return;
}
