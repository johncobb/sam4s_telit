#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "socket.h"
#include "app_listener.h"


#if defined(__arm__)
static const char _tag[] PROGMEM = "app_listener: "; /* used in embedded gcc */
#else
static const char _tag[] = "app_listener: ";
#endif

modem_event_t socket_event = EVT_OK;

typedef struct
{
    uint8_t *buffer;
    uint32_t len;
} app_listener_data_t;

app_listener_data_t _listener_data;


/*
 * modem event handler for receiving bulk data from servers.
 */
void app_listener_ondatareceive_func(uint8_t *buffer, uint32_t len)
{
    // _listener_data.buffer = buffer;
    // _listener_data.len = len;
    // LOG("app_listener_ondatareceive_func: event_id: %d len:%d buffer: %s\r\n", socket_event, len, buffer);
    LOG("app_listener_ondatareceive_func: len: %d buffer: %s\r\n", len, buffer);
}

/*
 * modem event handler for receiving modem events
 * Example: \r\nOK\r\n \r\nCONNECT\r\n
 */
void app_listener_oneventreceive_func(uint8_t *buffer, uint32_t len)
{
    socket_event = modem_identify_event(buffer);

    LOG("app_listener_oneventreceive_func: event_id: %d len:%d buffer: %s\r\n", socket_event, len, buffer);

    // LOG("app_listener_oneventreceive_func: len: %d buffer: %s\r\n", len, buffer);

}

/*
 * Create mode socket.
 */
modem_socket_t modem_socket = {
    .connection_id = 0,
    .protocol = TCPIP,
    .port = 1337,
    .address = NULL
};

/*
 * Create socket config.
 */
socket_config_t socket_config = {
    .connection_id = 1,
    .cid = 1,
    .packet_size = 512,
    .max_to = 90,
    .conn_to = 150, /* 600 = 60 sec. */
    .tx_to = 2
};

/*
 * Create firewall entry.
 */
firewall_entry_t entry = {
    .action = FW_ACCEPT,
    .ip_address = "172.18.1.1",
    .net_mask = "255.255.0.0"
};

/*
 * Create socket.
 */
socket_t _listener = {
    .modem_socket = &modem_socket,
    .socket_config = &socket_config,
    .firewall = &entry,
    .state = 0,
    .substate = 0,
    .timeout = 0,
    .event = app_listener_oneventreceive_func,
    .on_datareceive = app_listener_ondatareceive_func
};

void listener_reset_buffer(void)
{
    _listener_data.buffer = NULL;
    _listener_data.len = 0;
}

/*
 * First congifure the socket we will be using
 * Second set firewall entries that are required for listening
 * Third open the configured connection
 */
app_listener_init_status_t app_listener_init(void)
{
    LOG("app_listener_init: \r\n");
    app_listener_init_status_t init_result = APP_LISTENER_INIT_SUCCESS;
  

    LOG("app_modem_set_ondatareceive_func: \r\n");
    modem_set_ondatareceive_func(app_listener_ondatareceive_func);

    LOG("app_listener_oneventreceive_func: \r\n");
    modem_set_oneventreceive_func(app_listener_oneventreceive_func);

    /*
     * Reserve new socket.
     */
    if (socket_new(&_listener) == SCK_SUCCESS) {

        /* Setup socket configuration: */
        LOG("modem_socketconfig: \r\n");
        modem_socketconfig(_listener.socket_config);

        /*
         * Wait to see if we successfully configured socket.
         */
        while(true) {
            /*
             * Process modem response.
             */
            modem_tick();

            if (socket_event > EVT_WAITING) {
                if (socket_event == EVT_OK) {
                    /* reset to EVT_WAITING for next pass */
                    socket_event = EVT_WAITING;
                    break;
                } else if (socket_event == EVT_ERROR) {
                    init_result = APP_LISTENER_INIT_FAILED;
                    LOG("error configuring socket\r\n");
                }
            }  
        }        
    }

    /* Configure socket firewall entries */
    // LOG("modem_firewallcfg: \r\n");
    // modem_firewallcfg(_listener.firewall);

    // while(true) {
    //     /*
    //      * Process modem resposne.
    //      */
    //     modem_tick();

    //     if (socket_event > EVT_WAITING) {
    //         if (socket_event == EVT_OK) {
    //             /* reset to EVT_WAITING for next pass */
    //             socket_event = EVT_WAITING;
    //             break;
    //         } else if (socket_event == EVT_ERROR) {
    //             LOG("error configuring firewall\r\n");
    //             init_result = APP_LISTENER_INIT_FAILED;
    //             break;
    //         }
    //     }  
    // }

    return init_result;

    return APP_LISTENER_INIT_SUCCESS;

}



void app_listener_run(void)
{

    while(true) {
        _cph_delay_ms(1000);
        LOG("app_listener_run: waiting...\r\n");
    }
    /*
     * Modem socket listen workflow
     * Activate Context: "AT#SGACT=1,1\r" 
     * Socket Listen:    "AT#SL=1,1,1337\r" <connId: 1..6>, < listenState: 0-close socket listening, 1-start socket listening>
     * modem-response: SRING: 1
     * Socket Accept: AT#SA=1,0 <connId 1..6>, <connMode 0 -online mode, 1-command mode connection>
     */

    LOG("modem_socketlisten: \r\n");
    modem_socketlisten(_listener.modem_socket);

    while(true) {
        /*
         * Process modem response.
         */        
        modem_tick();

        if (socket_event > EVT_WAITING) {
            if (socket_event == EVT_CONNECT) {
                /* reset to EVT_WAITING for next pass */
                socket_event = EVT_WAITING;
                break;
            } else if (socket_event == EVT_ERROR) {
                LOG("error connecting\r\n");
                break;
            }
        }  
    }    
}

app_socket_state_t app_listener_tick(void)
{
    if (_listener_data.len > 0) {
        LOG("_listener_data: %s\r\n", _listener_data.buffer);
    }
    return APP_INPROC;
}