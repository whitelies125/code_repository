#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include "msg_trans.h"

class PidDispatcher : public MsgProcessor, public MsgDispatcher {
public:
    virtual uint32_t OnMsg(const void* msg) override;
    virtual uint32_t GetKey(const void* msg) override;
};

class MsgTypeDispatcher : public MsgProcessor, public MsgDispatcher {
public:
    virtual uint32_t OnMsg(const void* msg) override;
    virtual uint32_t GetKey(const void* msg) override;
};

class MsgTypeProcessor : public MsgProcessor {
public:
    virtual uint32_t OnMsg(const void* msg) override;
};
#endif
