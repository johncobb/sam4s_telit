#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "app_listener.h"

// at_cmd_t at_sck_commands[] = {
//     // fnc_handler, timeout, retries, waitingreply

//     {modem_listen,  1000, 0, false, SYS_OK},
//     {NULL, 0, 0, NULL}
// };

// uint8_t _sck_index = 0;
at_cmd_t *at_cmd;

sys_result socket_result = SYS_OK;



typedef struct
{
    uint8_t *buffer;
    uint32_t len;
} app_listener_data_t;

app_listener_data_t _listener_data;

void listener_reset_buffer(void)
{
    _listener_data.buffer = NULL;
    _listener_data.len = 0;
}

void app_listener_ondatareceive_func(uint32_t *buffer, uint32_t len)
{
    memcpy(_listener_data.buffer, buffer, len);

    #ifdef LOG_MODEM_ONDATARECEIVE
    printf("app_listener.app_listener_ondatareceive_func: len:%d buffer: %s\r\n", len, buffer);
    #endif

    socket_result = modem_data_handler(buffer, len);
}

 socket_config_t socket_config = {
     .connection_id = 1,
     .cid = 1,
     .packet_size = 512,
     .max_to = 90,
     .conn_to = 600,
     .tx_to = 2
    };
    
/*
 * First congifure the socket we will be using
 * Second set firewall entries that are required for listening
 * Third open the configured connection
 */
app_listener_init_status_t app_listener_init(void)
{

    app_listener_init_status_t init_result = APP_LISTENER_INIT_SUCCESS;

    /* Setup modem callback handler */
    at_cmd = NULL;
    modem_set_ondatareceive_func(app_listener_ondatareceive_func);

    /* Setup socket configuration: */
    modem_socketconfig(socket_config);

    while(true) {
        modem_tick();

        if (socket_result > SYS_OK) {
            if (socket_result == SYS_AT_OK) {
                socket_result = SYS_OK; // reset result
                break;
            } else if (socket_result == SYS_MODEM_ERROR) {
                init_result = APP_LISTENER_INIT_FAILED;
                printf("error configuring socket\r\n");
            }
        }  
    }

    /* Configure socket firewall entries */
    firewall_entry_t entry;

    entry.action = FW_ACCEPT;
    entry.ip_address = "172.18.1.1";
    entry.net_mask = "255.255.0.0";

    modem_firewallcfg(entry);

    while(true) {
        modem_tick();

        if (socket_result > SYS_OK) {
            if (socket_result == SYS_AT_OK) {
                socket_result = SYS_OK; // reset result
                break;
            } else if (socket_result == SYS_MODEM_ERROR) {
                printf("error configuring firewall\r\n");
                init_result = APP_LISTENER_INIT_FAILED;
                break;
            }
        }  
    }

    return init_result;

}

void app_listener_run(void)
{
    /* Configure socket connection */
    modem_socket_t socket;

    socket.connection_id = 1;
    socket.protocol = UDP;
    socket.port = 1337;
    socket.address = NULL;

    modem_socketlisten(socket);
}

// void app_open_run(void)
// {
//     /* Configure socket connection */
//     modem_socket_t socket;

//     socket.connection_id = 1;
//     socket.protocol = UDP;
//     socket.port = 1337;
//     socket.address = "bs.cphandheld.com";

//     modem_socketopen(socket);
// }

app_socket_state_t app_listener_tick(void)
{
    if (_listener_data.len > 0) {
        printf("_listener_data: %s\r\n", _listener_data.buffer);
    }
    return APP_INPROC;
}