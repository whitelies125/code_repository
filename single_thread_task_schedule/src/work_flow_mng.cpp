
#include "work_flow_mng.h"

uint32_t WorkFlowMng::Init(uint32_t num, const TaskInfo* task, uint32_t taskLen,
                           const StepInfo* step, uint32_t stepLen)
{
    scheMng_.Init(num);
    taskMng_.Init(task, taskLen);
    stepMng_.Init(step, stepLen);
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
    uint32_t res = scheduler->Run();
    if (res) return 0;
    // task 结束，释放 scheduler
    scheMng_.Free(scheduler);
    return 0;
}
