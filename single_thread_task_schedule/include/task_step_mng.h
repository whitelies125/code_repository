#ifndef TASK_STEP_MNG_H
#define TASK_STEP_MNG_H

#include <cstdint>

using Func = uint32_t (*)(void*);
struct TaskInfo {
    uint32_t taskId;
    Func task;
};
struct StepInfo {
    uint32_t stepId;
    Func step;
};

class StepMng {
public:
    ~StepMng();
    // 初始化
    uint32_t Init(const StepInfo* para, uint32_t paraLen);
    // 根据 stepId 获取对于 step 函数
    Func GetStep(uint32_t stepId);

private:
    uint32_t len_;
    StepInfo* stepInfo_;
};

class TaskMng {
public:
    ~TaskMng();
    // 初始化
    uint32_t Init(const TaskInfo* para, uint32_t paraLen);
    // uint32_t RegisterStep(uint32_t taskId, StepInfo* para, uint32_t paraLen);
    // 根据 taskId 获取对于 task 函数
    Func GetTask(uint32_t taskId);

private:
    uint32_t len_;
    TaskInfo* taskInfo_;
};

#endif
