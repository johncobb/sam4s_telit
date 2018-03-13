#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "modem_socket.h"

// at_cmd_t at_sck_commands[] = {
//     // fnc_handler, timeout, retries, waitingreply

//     {modem_listen,  1000, 0, false, SYS_OK},
//     {NULL, 0, 0, NULL}
// };

// uint8_t _sck_index = 0;
at_cmd_t *at_cmd;

sys_result socket_result = SYS_OK;

void modem_sck_ondatareceive_func(char *data)
{
    // printf("modem_config.ondatareceive_func: %s\r\n", data);

    socket_result = modem_data_handler(data);
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
void modem_socket_init(void)
{

    /* Setup modem callback handler */
    at_cmd = NULL;
    modem_set_ondatareceive_func(modem_sck_ondatareceive_func);

    /* Setup socket configuration: */
    modem_socketconfig(socket_config);

    while(true) {
        modem_tick();

        if (socket_result > SYS_OK) {
            if (socket_result == SYS_AT_OK) {
                socket_result = SYS_OK; // reset result
                break;
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
                break;
            }
        }  
    }

}

void modem_socket_listen_run(void)
{
    /* Configure socket connection */
    modem_socket_t socket;

    socket.connection_id = 1;
    socket.protocol = UDP;
    socket.port = 1337;
    socket.address = NULL;

    modem_socketlisten(socket);
}

void modem_socket_open_run(void)
{
    /* Configure socket connection */
    modem_socket_t socket;

    socket.connection_id = 1;
    socket.protocol = UDP;
    socket.port = 1337;
    socket.address = "bs.cphandheld.com";

    modem_socketopen(socket);
}

modem_sck_state_t modem_socket_tick(void)
{
    return SCK_INPROC;
}