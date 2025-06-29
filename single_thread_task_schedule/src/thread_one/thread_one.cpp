#include <iostream>
#include <queue>
#include <thread>

#include "api.h"
#include "msg_trans.h"
#include "msgtype.h"
#include "scheduler_mng.h"
#include "task_step_id.h"

class PidDispatcher : public MsgProcessor, MsgDispatcher {
public:
    virtual uint32_t OnMsg(const void* msg) { return Dispatch(msg); }
    virtual uint32_t GetKey(const void* msg)
    {
        auto msgHead = static_cast<const MsgHead*>(msg);
        return msgHead->senderPid;
    };
};

class MsgTypeDispatcher : public MsgProcessor, MsgDispatcher {
public:
    virtual uint32_t OnMsg(const void* msg) { return Dispatch(msg); }
    virtual uint32_t GetKey(const void* msg)
    {
        auto msgHead = static_cast<const MsgHead*>(msg);
        return msgHead->msgType;
    };
};

#define Task(x)                              \
    static uint32_t Task##x(void* scheduler) \
    {                                        \
        std::cout << "Task" #x << std::endl; \
        return 0;                            \
    }

static uint32_t Task1(void* scheduler)
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

static uint32_t Step1(void* scheduler)
{
    std::cout << "Task1.Proc1" << std::endl;
    return 1;
}

static uint32_t Step2(void* scheduler)
{
    std::cout << "Task1.Proc2" << std::endl;
    return 0;
}

static uint32_t WorkFlowInit()
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

static Scheduler* GetSchedulerByMsg(uint32_t msgType)
{
    auto workflow = WorkFlowMng::GetWorkFlowMng().GetWorkSpace(0);
    if (workflow == nullptr) return nullptr;
    return workflow->FindWaitMsg(msgType);
}

static uint32_t GetTaskIdByMsg(uint32_t msgType)
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

static uint32_t HandleBySuspendScheduler(uint32_t msgType)
{
    Scheduler* sche = GetSchedulerByMsg(msgType);  // 查看是否有挂起的 scheduler 等待该消息
    if (sche != nullptr) {
        sche->SetWaitMsg(UINT32_MAX);
        sche->Run();
        return 0;
    }
    return -1;
}

static uint32_t HandleByNewTask(uint32_t msgType)
{
    uint32_t taskId = GetTaskIdByMsg(msgType);  // 从获取处理该消息的 taskId
    return StartTask(taskId);                   // 执行 taskId 对应的 task
}

static uint32_t ThreadOneInit()
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

uint32_t ThreadOneMain()
{
    std::cout << "thread 1 start." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ThreadOneInit();
    std::cout << "thread 1 end." << std::endl;
    return 0;
}
