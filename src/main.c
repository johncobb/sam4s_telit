
#include <cph.h>


#include "modem.h"
#include "modem_config.h"
#include "socket.h"

// NO DIALTONE
// BUSY
// NO CARRIER
// CONNECT
// CONNECT 115200

extern void _cph_delay_ms(uint32_t millis)
{
    // millis * micros
    usleep(millis*1000);
}

void ondatareceive_func(char *data)
{
    printf("modem_rx_func: %s\r\n", data);
}

void onsocketdatareceive_func(uint8_t *data, uint32_t len)
{
    printf("onsocketdatareceive_func: \r\n");
}

socket_t _listener;

void socket_init_loop(void)
{
    modem_socket_t modem_socket = {
        .connection_id = 0,
        .protocol = TCPIP,
        .port = 1337,
        .address = NULL
    };

    socket_config_t socket_config = {
        .connection_id = 1,
        .cid = 1,
        .packet_size = 512,
        .max_to = 90,
        .conn_to = 600,
        .tx_to = 2
    };

    firewall_entry_t entry = {
        .action = FW_ACCEPT,
        .ip_address = "172.18.1.1",
        .net_mask = "255.255.0.0"
    };
    
    socket_t _listener = {
        .modem_socket = &modem_socket,
        .socket_config = &socket_config,
        .firewall = &entry,
        .state = 0,
        .substate = 0,
        .timeout = 0,
        .event = ondatareceive_func,
        .on_datareceive = onsocketdatareceive_func
    };

    /*
     * The following tests the reservation of the maximum amount of sockets.
     */

    int loop_count = 0;
    while (true) {
        if (socket_new(&_listener) == SCK_SUCCESS) {
            log_socket_settings(socket_pool[loop_count]);
            loop_count++;
        } else {
            break;
        } 
    }

}

void modem_loop(void)
{
    modem_init();
    modem_set_ondatareceive_func(ondatareceive_func);

    modem_write("ATE0\r\n");

    while (true) {
        modem_write("AT\r\n");
        modem_tick();

        _cph_delay_ms(1000);
    }
}

void config_loop(void)
{
    modem_init();
    modem_config_init();

    while (true) {
        modem_tick();
        if (modem_config_tick() == CFG_COMPLETE){
            break;
        }
        // _cph_delay_ms(10);
    }
}

int main(void)
{
    socket_init_loop();
    // modem_loop();
    // config_loop();

    while(true) {
        printf("just hanging out in the loop\r\n");
        _cph_delay_ms(1000);
    }
}
