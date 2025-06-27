
#include "api.h"

uint32_t AddStep(Scheduler* scheduler, uint32_t stepId)
{
    // 添加 step
    auto workflow = WorkFlowMng::GetWorkFlowMng().GetWorkSpace(0);
    if (workflow == nullptr) return -1;
    auto step = workflow->GetStep(stepId);
    scheduler->AddStep(step);
    return 0;
}

uint32_t AddWaitMsg(Scheduler* scheduler, uint32_t msgType)
{
    // 设置等待消息
    scheduler->SetWaitMsg(msgType);
    return 0;
}

uint32_t StartTask(uint32_t taskId)
{
    auto workflow = WorkFlowMng::GetWorkFlowMng().GetWorkSpace(0);
    if (workflow == nullptr) return -1;
    return workflow->StartTask(taskId);
}
