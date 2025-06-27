
#include "scheduler_mng.h"

void Scheduler::SetTask(Func task)
{
    if (task == nullptr) return;
    curStep_ = 0;
    stepNum_ = 1;
    step_[0] = task;
    waitMsg_ = UINT32_MAX;
};

void Scheduler::AddStep(Func step)
{
    if (step == nullptr) return;
    step_[stepNum_] = step;
    ++stepNum_;
};
uint32_t Scheduler::Run()
{
    uint32_t res = 0;
    for (; curStep_ < stepNum_; ++curStep_) {
        res = step_[curStep_](this);
        if (res) {
            ++curStep_;
            return res;
        }
    }
    return res;
}
// 初始化: 调度器个数等
uint32_t SchedulerMng::Init(uint32_t size)
{
    schedulers_ = new Scheduler[size];
    len_ = size;
    return index_.Init(size);
}
// 返回一个调度器
Scheduler* SchedulerMng::Alloc()
{
    uint32_t id = index_.AllocId();  // 申请一个未使用的 id
    if (id == UINT32_MAX) return nullptr;
    return schedulers_ + id;  // 返回下标调度器
}
// 释放调度器
bool SchedulerMng::Free(void* addr)
{
    uint32_t id = ((uint8_t*)addr - (uint8_t*)schedulers_) / sizeof(Scheduler);
    return index_.FreeId(id);
}

Scheduler* SchedulerMng::FindWaitMsg(uint32_t msgType)
{
    for (uint32_t i = 0; i < len_; ++i) {
        if (schedulers_[i].GetWaitMsg() == msgType) {
            return schedulers_;
        }
    }
    return nullptr;
}
