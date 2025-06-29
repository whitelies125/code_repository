#include <iostream>
#include <queue>
#include <thread>

#include "api.h"
#include "msg_handler.h"
#include "msgtype.h"
#include "task_step_id.h"
#include "work_flow_mng.h"

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
static PidDispatcher pidDispatcher;
static MsgTypeDispatcher userDispatcher;
static MsgTypeDispatcher threadTwoDispatcher;
static MsgTypeProcessor msgTypeProcessor;
enum Pid {
    PID_USER = 0,
    PID_THREAD_ONE = 1,
    PID_THREAD_TWO = 2,
    PID_BUTT
};

static uint32_t MsgTransInit()
{
    pidDispatcher.Register(PID_USER, &userDispatcher);
    pidDispatcher.Register(PID_THREAD_TWO, &threadTwoDispatcher);
    userDispatcher.Register(MSG_USER_ACCESS, &msgTypeProcessor);
    userDispatcher.Register(MSG_USER_REQUEST, &msgTypeProcessor);
    userDispatcher.Register(MSG_USER_LOGOUT, &msgTypeProcessor);
    return 0;
}

static uint32_t WorkFlowInit()
{
    MsgTransInit();
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

static uint32_t ThreadOneInit()
{
    WorkFlowInit();
    // 模拟消息队列
    std::queue<Msg> msgQueue;
    Msg msg;
    msg.senderPid = PID_USER;
    msg.msgType = MSG_USER_ACCESS;
    msgQueue.push(msg);
    msg.senderPid = PID_USER;
    msg.msgType = MSG_USER_REQUEST;
    msgQueue.push(msg);
    msg.senderPid = PID_USER;
    msg.msgType = MSG_USER_LOGOUT;
    msgQueue.push(msg);
    while (!msgQueue.empty()) {
        if (msgQueue.empty()) continue;
        auto msgType = msgQueue.front();
        msgQueue.pop();
        pidDispatcher.OnMsg(&msgType);
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
