
#include "work_flow_mng.h"

uint32_t WorkFlowMng::Init(TaskInfo* para, uint32_t paraLen)
{
    taskMng_.Init(para, paraLen);
    scheMng_.Init(paraLen);
    return 0;
}

uint32_t WorkFlowMng::InitStepInfo(StepInfo* para, uint32_t paraLen)
{
    stepMng_.Init(para, paraLen);
    return 0;
}
uint32_t WorkFlowMng::StartTask(uint32_t taskId)
{
    // 申请 scheduler
    Scheduler* scheduler = scheMng_.Alloc();
    if (scheduler == nullptr) return -1;
    // 绑定 task
    scheduler->SetTask(taskMng_.GetTask(taskId));
    // 执行 task
    uint32_t res  = scheduler->Run();
    if(res) return 0;
    // task 结束，释放 scheduler
    scheMng_.Free(scheduler);
    return 0;
}
