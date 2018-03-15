#ifndef APP_LISTENER_H_
#define APP_LISTENER_H_

#include <cph.h>
#include "modem.h"

#ifndef LOG_APP_LISTENER_VERBOSE
#define LOG_APP_LISTENER_VERBOSE
#endif

typedef enum {
    APP_INPROC = 0,
    APP_TIMEOUT,
    APP_ERROR,
    APP_INIT_SUCCESS,
    APP_COMPLETE
} app_socket_state_t;

typedef enum {
    APP_LISTENER_INIT_SUCCESS = 0,
    APP_LISTENER_INIT_FAILED
} app_listener_init_status_t;



app_socket_state_t app_listener_tick(void);
app_listener_init_status_t app_listener_init(void);


#endif