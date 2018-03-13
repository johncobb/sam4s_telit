#ifndef MODEM_SOCKET_H_
#define MODEM_SOCKET_H_

#include "modem.h"

// typedef void (*modem_config_socket_func_t)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

typedef enum {
    SCK_INPROC = 0,
    SCK_TIMEOUT,
    SCK_ERROR,
    SCK_COMPLETE
} modem_sck_state_t;



modem_sck_state_t modem_socket_tick(void);
void modem_socket_init(void);



#endif