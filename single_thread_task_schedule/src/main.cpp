#include <queue>

#include "api.h"
#include "index.h"
#include "memory_pool.h"
#include "msgtype.h"
#include "task_step_id.h"
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

#define Task(x)                              \
    uint32_t Task##x(void* scheduler)        \
    {                                        \
        std::cout << "Task" #x << std::endl; \
        return 0;                            \
    }

static WorkFlowMng wfMng;
uint32_t Task1(void* scheduler)
{
    std ::cout << "Task1" << std ::endl;

    auto sche = static_cast<Scheduler*>(scheduler);
    // 添加 step
    AddStep(sche, STEPID_DATA_PREPARE);
    AddStep(sche, STEPID_ALLOC_RESOURCE);
    // 设置等待消息
    AddWaitMsg(sche, 2);
    return 0;
}
Task(2) Task(3)

    uint32_t Step1(void* scheduler)
{
    std::cout << "Task1.Proc1" << std::endl;
    return 1;
}

uint32_t Step2(void* scheduler)
{
    std::cout << "Task1.Proc2" << std::endl;
    return 0;
}

uint32_t WorkFlowInit()
{
    constexpr TaskInfo taskInfo[] = {
        {TASKID_USER_ACCESS, Task1},
        {TASKID_USER_REQUEST, Task2},
        {TASKID_USER_LOGOUT, Task3},
    };
    constexpr StepInfo stepInfo[] = {
        {STEPID_DATA_PREPARE, Step1},
        {STEPID_ALLOC_RESOURCE, Step2},
    };
    uint32_t taskLen = sizeof(taskInfo) / sizeof(taskInfo[0]);
    auto workflow = WorkFlowMng::GetWorkFlowMng().GetWorkSpace(0);
    if (workflow == nullptr) return -1;
    return workflow->Init(taskLen, taskInfo, taskLen, stepInfo,
                          sizeof(stepInfo) / sizeof(stepInfo[0]));
}

Scheduler* GetSchedulerByMsg(uint32_t msgType)
{
    auto workflow = WorkFlowMng::GetWorkFlowMng().GetWorkSpace(0);
    if (workflow == nullptr) return nullptr;
    return workflow->FindWaitMsg(msgType);
}

uint32_t GetTaskIdByMsg(uint32_t msgType)
{
    struct {
        uint32_t msgType;
        uint32_t taskId;
    } para[] = {
        {MSG_USER_ACCESS, TASKID_USER_ACCESS},
        {MSG_USER_REQUEST, TASKID_USER_REQUEST},
        {MSG_USER_LOGOUT, TASKID_USER_LOGOUT},
    };
    for (const auto& it : para) {
        if (it.msgType == msgType) {
            return it.taskId;
        }
    }
    return UINT32_MAX;
}

uint32_t HandleBySuspendScheduler(uint32_t msgType)
{
    Scheduler* sche = GetSchedulerByMsg(msgType);  // 查看是否有挂起的 scheduler 等待该消息
    if (sche != nullptr) {
        sche->SetWaitMsg(UINT32_MAX);
        sche->Run();
        return 0;
    }
    return -1;
}

uint32_t HandleByNewTask(uint32_t msgType)
{
    uint32_t taskId = GetTaskIdByMsg(msgType);  // 从获取处理该消息的 taskId
    return StartTask(taskId);                   // 执行 taskId 对应的 task
}

uint32_t CheckSche()
{
    using namespace std;
    WorkFlowInit();
    // 模拟消息队列
    queue<int> msg({MSG_USER_ACCESS, MSG_USER_REQUEST, MSG_USER_LOGOUT});
    while (!msg.empty()) {
        if (msg.empty()) continue;
        auto msgType = msg.front();
        msg.pop();
        if (HandleBySuspendScheduler(msgType) == 0) continue;
        if (HandleByNewTask(msgType) == 0) continue;
        cout << "error msg no handler : " << msgType << endl;
    }
    return 0;
}

int main()
{
    auto& WorkFlowMng = WorkFlowMng::GetWorkFlowMng();
    WorkFlowMng.Init(1);
    CheckIndex();
    CheckBlock();
    CheckMemPool();
    CheckSche();
}
