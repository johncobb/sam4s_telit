#ifndef SOCKET_HANDLER_H_
#define SOCKET_HANDLER_H_

#include <cph.h>
#include "modem.h"

typedef enum {
    SCK_H_INIT = 0,
    SCK_H_REGISTER,
    SCK_H_IDLE,
    SCK_H_LISTEN,
    SCK_H_RESET,
    SCK_H_SEND,
    SCK_H_SUSPEND,
    SCK_H_CLOSE,
    SCK_H_NEXTSOCKET
} socket_handler_state_t;

typedef enum {
    SUBSTATE_INVOKE = 0,
    SUBSTATE_WAITREPLY
} handler_substate_t;

socket_handler_state_t socket_handler_tick(void);
void socket_handler_init(void);

sys_result sck_init(void);

#endif