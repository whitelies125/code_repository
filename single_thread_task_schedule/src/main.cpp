#include <thread>

#include "index.h"
#include "memory_pool.h"
#include "thread_one.h"
#include "work_flow_mng.h"

void CheckIndex()
{
    using namespace std;
    uint32_t size = 3;
    Index<uint8_t> index;
    index.Init(size);
    // ArrIndex<uint8_t, 3> index;
    cout << "alloc test" << endl;
    for (uint32_t i = 0; i < size; ++i) {
        cout << "id : " << (uint32_t)index.AllocId() << ", status : ";
        index.Print();
    }
    cout << (uint32_t)index.AllocId() << ", status : ";
    index.Print();
    cout << "free test" << endl;
    for (uint8_t i = 0; i < size; ++i) {
        cout << "id : " << (uint32_t)index.FreeId(i) << ", status : ";
        index.Print();
    }
    cout << (uint32_t)index.FreeId(0) << ", status : ";
    index.Print();
    cout << (uint32_t)index.FreeId(5) << ", status : ";
    index.Print();
    cout << endl;
}

void CheckBlock()
{
    using namespace std;
    Block block;
    block.Init(3, 1024);
    cout << "alloc 1: " << (void*)block.Alloc(0) << endl;     // 申请成功
    cout << "alloc 2: " << (void*)block.Alloc(512) << endl;   // 申请成功
    cout << "alloc 3: " << (void*)block.Alloc(1025) << endl;  // 超出内存块大小，申请失败
    void* ptr = nullptr;
    cout << "alloc 4: " << (ptr = (void*)block.Alloc(1024)) << endl;  // 申请成功
    cout << "alloc 5: " << (void*)block.Alloc(1024) << endl;          // 内存池耗尽，申请失败
    block.Free(ptr);                                                  // 归还 1 个内存块
    cout << "alloc 6: " << (void*)block.Alloc(1024) << endl;          // 申请成功
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
    cout << "alloc 1: " << (void*)memPool.Alloc(2049) << endl;  // 申请失败，超出最大内存块大小
    void* ptr = nullptr;
    cout << "alloc 2: " << (ptr = (void*)memPool.Alloc(0)) << endl;  // 申请成功，使用 1K 大小内存块
    cout << "alloc 3: " << (void*)memPool.Alloc(512) << endl;        // 申请成功，使用 2K 大小内存块
    cout << "alloc 4: " << (void*)memPool.Alloc(1024) << endl;       // 申请失败，内存池耗尽
    memPool.Free(ptr);                                               // 归还 1K 大小内存块
    cout << "alloc 5: " << (void*)memPool.Alloc(1025)
         << endl;  // 申请失败，1025B 超出可用最大内存块大小
    cout << "alloc 6: " << (void*)memPool.Alloc(1024) << endl;  // 申请成功，使用 1K 大小内存块
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
