#ifndef MSGTYPE_H
#define MSGTYPE_H

#include <cstdint>

#define MSG_HEAD         \
    uint32_t senderPid;    \
    uint32_t recevierPid; \
    uint32_t msgType;    \
    uint32_t transId;

struct MsgHead {
    uint32_t senderPid;
    uint32_t recevierPid;
    uint32_t msgType;
    uint32_t transId;
};

struct MsgUserAccess {
    uint32_t accessCause;
};

struct MsgUserRequest {
    uint32_t userId;
};

struct MsgUserLogout {
    uint32_t userId;
};

struct Msg {
    MSG_HEAD
    union {
        MsgUserAccess msgUserAccess;
        MsgUserRequest msgUserRequest;
        MsgUserLogout msgUserLogout;
    } u;
};

enum MsgType {
    MSG_USER_ACCESS = 0,
    MSG_USER_REQUEST,
    MSG_USER_LOGOUT,
    MSG_BUTT
};

#endif
