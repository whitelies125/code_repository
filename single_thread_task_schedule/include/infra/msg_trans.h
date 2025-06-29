#ifndef MSG_TRANS_H
#define MSG_TRANS_H

#include <cstdint>

class MsgProcessor {
public:
    virtual uint32_t OnMsg(const void *msg) = 0;
};

class MsgDispatcher {
public:
    uint32_t Register(uint32_t key, MsgProcessor *msgProcessor);
    uint32_t Deregister(uint32_t key);
    virtual uint32_t GetKey(const void *msg) = 0;
    uint32_t Dispatch(const void *msg);

private:
    struct Keymap {
        uint32_t key;
        MsgProcessor *msgProcessor;
    };
    static constexpr uint32_t MAX_KEYMAP_LEN = 10;
    uint32_t len_;
    Keymap keymap_[MAX_KEYMAP_LEN];
};

#endif
