
#ifndef API_H
#define API_H

#include "work_flow_mng.h"

uint32_t AddStep(Scheduler* scheduler, uint32_t stepId);
uint32_t AddWaitMsg(Scheduler* scheduler, uint32_t msgType);

uint32_t StartTask(uint32_t taskId);

#endif
