#ifndef MODEM_CONFIG_H_
#define MODEM_CONFIG_H_
#include "modem.h"

typedef enum{
    CFG_INPROC = 0,
    CFG_TIMEOUT,
    CFG_ERROR,
    CFG_COMPLETE
} modem_cfg_state_t;

modem_cfg_state_t modem_config_tick(void);
void modem_config_init(void);

#endif