#ifndef SCHEDULER_MNG_H
#define SCHEDULER_MNG_H

#include "index.h"
#include "task_step_mng.h"

class Scheduler {
public:
    void SetTask(Func task);
    uint32_t Run();
    void AddStep(Func step);
    void SetWaitMsg(uint32_t msgType) { waitMsg_ = msgType; }
    uint32_t GetWaitMsg() { return waitMsg_; }

public:
    static constexpr uint32_t MAX_STEP_NUM = 10;

private:
    uint32_t curStep_;
    uint32_t stepNum_;
    Func step_[MAX_STEP_NUM];  // task step 在代码上都是回调函数，合并处理
    uint32_t waitMsg_;
};

class SchedulerMng {
public:
    // 初始化: 调度器个数等
    uint32_t Init(uint32_t size);
    // 返回一个调度器
    Scheduler* Alloc();
    // 释放调度器
    bool Free(void* addr);
    Scheduler* FindWaitMsg(uint32_t msgType);

private:
    uint32_t len_;
    Scheduler* schedulers_;
    Index<uint32_t> index_;  // 仍然是使用 id分配器来管理独占资源
};
#endif
