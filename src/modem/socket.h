#ifndef MODEM_SOCKET_H_
#define MODEM_SOCKET_H_

#include "modem.h"

#define SOCKET_POOL_LEN     6

typedef enum {
    SCK_SUCCESS = 0,
    SCK_ERR_MAX_POOL
} socket_status_t;

typedef enum {
    SCK_INIT = 0,
    SCK_CONFIG,
    SCK_CONNECT,
    SCK_SUSPEND,
    SCK_RESUME,
    SCK_CLOSE
} socket_state_t;

typedef enum {
    SCK_INVOKE = 1,
    SCK_WAIT
} socket_substate_t;

typedef void (*socket_connect_func_t)(void);
typedef void (*socket_ondisconnect_func_t)(void);
typedef void (*socket_ondatareceive_func_t)(uint8_t *data, uint32_t len);

typedef struct
{
	socket_connect_func_t on_connect;
	socket_ondisconnect_func_t on_disconnect;
	socket_ondatareceive_func_t on_datareceive;

}socket_event_handler_t;

typedef struct {
    modem_socket_t *modem_socket;
    socket_config_t *socket_config;
    firewall_entry_t *firewall;
    socket_state_t state;
    socket_substate_t substate;
    uint32_t timeout;
    modem_ondatareceive_func_t event;
    socket_ondatareceive_func_t on_datareceive;
} socket_t;

extern socket_t *socket_pool[SOCKET_POOL_LEN];

socket_status_t socket_new(socket_t *socket);
socket_state_t socket_init(socket_t *socket);
socket_state_t socket_tick(socket_t *socket);
void log_socket_settings(socket_t *socket);

#endif