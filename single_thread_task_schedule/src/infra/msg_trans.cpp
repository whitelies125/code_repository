#include "msg_trans.h"

uint32_t MsgDispatcher::Register(uint32_t key, MsgProcessor *msgProcessor)
{
    if (len_ < MAX_KEYMAP_LEN) return -1;
    keymap_[len_].key = key;
    keymap_[len_].msgProcessor = msgProcessor;
    ++len_;
    return 0;
}

uint32_t MsgDispatcher::Deregister(uint32_t key) { return 0; }
uint32_t MsgDispatcher::Dispatch(const void *msg)
{
    uint32_t key = GetKey(msg);
    if (key == UINT32_MAX) return -1;
    MsgProcessor *msgp = nullptr;
    for (uint32_t i = 0; i < len_; ++i) {
        if (keymap_[i].key == key) {
            msgp = keymap_[i].msgProcessor;
            break;
        }
    }
    if (msgp == nullptr) return -1;

    return msgp->OnMsg(msg);
}
