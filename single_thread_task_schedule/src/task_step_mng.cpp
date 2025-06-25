#include "task_step_mng.h"

StepMng::~StepMng() { delete stepInfo_; }
// 初始化
uint32_t StepMng::Init(StepInfo* para, uint32_t paraLen)
{
    stepInfo_ = new StepInfo[paraLen];
    for (uint32_t i = 0; i < paraLen; ++i) {
        stepInfo_[i] = para[i];
    }
    len_ = paraLen;
    return 0;
}
// 根据 stepId 获取对应 step 函数
Func StepMng::GetStep(uint32_t stepId)
{
    for (uint32_t i = 0; i < len_; ++i) {
        if (stepInfo_[i].stepId == stepId) {
            return stepInfo_[i].step;
        }
    }
    return nullptr;
}
TaskMng::~TaskMng() { delete taskInfo_; }
// 初始化
uint32_t TaskMng::Init(TaskInfo* para, uint32_t paraLen)
{
    taskInfo_ = new TaskInfo[paraLen];
    for (uint32_t i = 0; i < paraLen; ++i) {
        taskInfo_[i] = para[i];
    }
    len_ = paraLen;
    return 0;
}
// 根据 taskId 获取对应 task 函数
Func TaskMng::GetTask(uint32_t taskId)
{
    for (uint32_t i = 0; i < len_; ++i) {
        if (taskInfo_[i].taskId == taskId) {
            return taskInfo_[i].task;
        }
    }
    return nullptr;
}
