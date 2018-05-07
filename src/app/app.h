#ifndef APP_H_
#define APP_H_

#include <cph.h>
#include "modem.h"

#ifndef LOG_APP_VERBOSE
#define LOG_APP_VERBOSE
#endif

typedef enum {
    APP_INPROC = 0,
    APP_TIMEOUT,
    APP_ERROR,
    APP_COMPLETE
} app_state_t;

typedef enum {
    APP_INIT_SUCCESS = 0,
    APP_INIT_FAILED
} app_init_status_t;

typedef struct
{
    uint8_t *buffer;
    uint32_t len;
} app_data_t;


app_init_status_t app_listener_init(void);
app_state_t app_listener_tick(void);


app_init_status_t app_powersave_init(void);
app_state_t app_powersave_tick(void);

app_init_status_t app_sender_init(void);
app_state_t app_sender_tick(void);

#endif
