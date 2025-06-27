#ifndef WORK_FLOW_MNG_H
#define WORK_FLOW_MNG_H

#include <cstdint>

#include "scheduler_mng.h"
#include "task_step_mng.h"

class WorkFlowMng {
public:
    uint32_t Init(uint32_t num, const TaskInfo* task, uint32_t taskLen, const StepInfo* step,
                  uint32_t stepLen);
    uint32_t StartTask(uint32_t taskId);

    Func GetStep(uint32_t stepId) { return stepMng_.GetStep(stepId); }
    Scheduler* FindWaitMsg(uint32_t msgType) { return scheMng_.FindWaitMsg(msgType); }

private:
    StepMng stepMng_;
    TaskMng taskMng_;
    SchedulerMng scheMng_;
};

#endif
