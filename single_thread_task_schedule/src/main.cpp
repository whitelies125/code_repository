#include <cassert>
#include <thread>

#include "index.h"
#include "memory_pool.h"
#include "thread_one.h"
#include "work_flow_mng.h"

#define EXPECT_EQ(expacted, actual)                                                      \
    if (expacted != actual) {                                                            \
        std::cout << std::endl;                                                          \
        std::cout << "error: File: " << __FILE__ << ", Line: " << __LINE__ << std::endl; \
        std::cout << "expacted: " << expacted << std::endl;                              \
        std::cout << "actual: " << actual << std::endl;                                  \
        std::cout << std::endl;                                                          \
    }

#define EXPECT_NE(expacted, actual)                                                      \
    if (expacted == actual) {                                                            \
        std::cout << std::endl;                                                          \
        std::cout << "error: File: " << __FILE__ << ", Line: " << __LINE__ << std::endl; \
        std::cout << "expacted: " << expacted << std::endl;                              \
        std::cout << "actual: " << actual << std::endl;                                  \
        std::cout << std::endl;                                                          \
    }

void CheckIndex()
{
    using namespace std;
    uint32_t size = 3;
    Index<uint8_t> index;
    index.Init(size);
    // ArrIndex<uint8_t, 3> index;
    for (uint32_t i = 0; i < size; ++i) {
        // 测试申请完 0，1，2 内存块，预期申请成功
        uint32_t id = index.AllocId();
        EXPECT_EQ(i, id);
        index.Print();
    }

    // 测试再申请一块内存块，预期申请失败
    uint32_t id = index.AllocId();
    EXPECT_EQ(UINT8_MAX, id);
    index.Print();
    for (uint8_t i = 0; i < size; ++i) {
        // 测试释放0，1，2内存块，预期释放成功
        uint32_t ret = index.FreeId(i);
        EXPECT_EQ(0, ret);
        index.Print();
    }

    // 测试再次释放0内存块，预期释放失败
    uint32_t ret = index.FreeId(0);
    EXPECT_EQ(UINT8_MAX, ret);
    index.Print();
    // 测试释放超出范围id的内存块，预期释放失败
    ret = index.FreeId(5);
    EXPECT_EQ(UINT8_MAX, ret);
    index.Print();
    cout << endl;
}

void CheckBlock()
{
    using namespace std;
    Block block;
    block.Init(3, 1024);
    void* ptr1 = block.Alloc(0);  // 申请成功
    EXPECT_NE(nullptr, ptr1)
    void* ptr2 = block.Alloc(512);  // 申请成功
    EXPECT_NE(nullptr, ptr2)
    void* ptr3 = block.Alloc(1025);  // 超出内存块大小，申请失败
    EXPECT_EQ(nullptr, ptr3)
    void* ptr4 = block.Alloc(1024);  // 申请成功
    EXPECT_NE(nullptr, ptr2)
    bool ret = block.Free(ptr1);  // 归还 1 个内存块
    EXPECT_EQ(true, ret)
    void* ptr5 = block.Alloc(1024);  // 申请成功
    EXPECT_NE(nullptr, ptr5)
    cout << endl;
}
void CheckMemPool()
{
    using namespace std;
    BlockInfo para[] = {
        {1, 1024},
        {2, 2048},
    };
    MemPool memPool;
    memPool.Init(para, sizeof(para) / sizeof(para[0]));
    void* ptr1 = memPool.Alloc(2049);  // 申请失败，超出最大内存块大小
    EXPECT_EQ(nullptr, ptr1)
    void* ptr2 = memPool.Alloc(0);  // 申请成功，使用 1K 大小内存块
    EXPECT_NE(nullptr, ptr2)
    void* ptr3 = memPool.Alloc(512);  // 申请成功，使用 2K 大小内存块
    EXPECT_NE(nullptr, ptr3)
    void* ptr4 = memPool.Alloc(1024);  // 申请成功，使用 2K 大小内存块
    EXPECT_NE(nullptr, ptr4)
    void* ptr5 = memPool.Alloc(1024);  // 申请失败，内存池耗尽
    EXPECT_EQ(nullptr, ptr5)
    memPool.Free(ptr1);                // 归还 1K 大小内存块
    void* ptr6 = memPool.Alloc(1025);  // 申请失败，超出最大内存块大小
    EXPECT_EQ(nullptr, ptr6)
    void* ptr7 = memPool.Alloc(1024);  // 申请成功，使用 1K 大小内存块
    EXPECT_EQ(nullptr, ptr7)
    cout << endl;
}

void CheckThread()
{
    std::cout << "main thread start." << std::endl;
    std::jthread threadOne {ThreadOneMain};
    threadOne.join();
    std::cout << "main thread end." << std::endl;
    return;
}
int main()
{
    auto& WorkFlowMng = WorkFlowMng::GetWorkFlowMng();
    WorkFlowMng.Init(1);
    CheckIndex();
    CheckBlock();
    CheckMemPool();
    CheckThread();
}
