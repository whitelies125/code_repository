#include "msg_handler.h"

#include "api.h"
#include "msgtype.h"
#include "task_step_id.h"
#include "work_flow_mng.h"

uint32_t PidDispatcher::OnMsg(const void* msg) { return Dispatch(msg); }
uint32_t PidDispatcher::GetKey(const void* msg)
{
    auto msgHead = static_cast<const MsgHead*>(msg);
    return msgHead->senderPid;
};
uint32_t MsgTypeDispatcher::OnMsg(const void* msg) { return Dispatch(msg); }
uint32_t MsgTypeDispatcher::GetKey(const void* msg)
{
    auto msgHead = static_cast<const MsgHead*>(msg);
    return msgHead->msgType;
};

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

uint32_t MsgTypeProcessor::OnMsg(const void* msg)
{
    auto msgHead = static_cast<const MsgHead*>(msg);
    auto msgType = msgHead->msgType;
    if (HandleBySuspendScheduler(msgType) == 0) return 0;
    if (HandleByNewTask(msgType) == 0) return 0;
    return -1;
}
