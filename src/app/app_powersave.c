#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "socket.h"
#include "app.h"


#if defined(__arm__)
static const char _tag[] PROGMEM = "app_powersave: "; /* used in embedded gcc */
#else
static const char _tag[] = "app_powersave: ";
#endif

modem_event_t psm_event = EVT_OK;

typedef struct
{
    uint8_t *buffer;
    uint32_t len;
} app_powersave_data_t;

app_powersave_data_t _powersave_data;


/*
 * modem event handler for receiving bulk data from servers.
 */
void app_powersave_ondatareceive_func(uint8_t *buffer, uint32_t len)
{
    // _listener_data.buffer = buffer;
    // _listener_data.len = len;
    // LOG("app_listener_ondatareceive_func: event_id: %d len:%d buffer: %s\r\n", psm_event, len, buffer);
    LOG("app_powersave_ondatareceive_func: len: %d buffer: %s\r\n", len, buffer);
}

/*
 * modem event handler for receiving modem events
 * Example: \r\nOK\r\n \r\nCONNECT\r\n
 */
void app_powersave_oneventreceive_func(uint8_t *buffer, uint32_t len)
{
    psm_event = modem_identify_event(buffer);

    LOG("app_powersave_oneventreceive_func: event_id: %d len:%d buffer: %s\r\n", psm_event, len, buffer);

    // LOG("app_listener_oneventreceive_func: len: %d buffer: %s\r\n", len, buffer);

}


void powersave_reset_buffer(void)
{
    _powersave_data.buffer = NULL;
    _powersave_data.len = 0;
}

/*
 * First congifure the socket we will be using
 * Second set firewall entries that are required for listening
 * Third open the configured connection
 */
app_init_status_t app_powersave_init(void)
{
    LOG("app_powersave_init: \r\n");
    app_init_status_t init_result = APP_INIT_SUCCESS;
  

    LOG("app_powersave_oneventreceive_func: \r\n");
    modem_set_oneventreceive_func(app_powersave_oneventreceive_func);


    /* Start by disabling power saving mode: */
    // LOG("modem_disablepsm: \r\n");
    // modem_disablepsm();
    LOG("modem_enablepsm: \r\n");
    modem_enablepsm();    

    /*
        * Wait to see if we successfully configured socket.
        */
    while(true) {
        /*
            * Process modem response.
            */
        modem_tick();

        if (psm_event > EVT_WAITING) {
            if (psm_event == EVT_OK) {
                /* reset to EVT_WAITING for next pass */
                psm_event = EVT_WAITING;
                break;
            } else if (psm_event == EVT_ERROR) {
                init_result = APP_INIT_FAILED;
                LOGE("error setting power saving mode!\r\n");
            }
        }  
    }        
    
    return init_result;

    return APP_INIT_SUCCESS;

}



void app_powersave_run(void)
{
    

    while(true) {
        _cph_delay_ms(1000);
        LOG("app_powersave_run: waiting...\r\n");
    }
    /*
     * Modem powersave workflow
     * TBD
     */


    while(true) {
        /*
         * Process modem response.
         */        
        modem_tick();

        if (psm_event > EVT_WAITING) {
            if (psm_event == EVT_CONNECT) {
                /* reset to EVT_WAITING for next pass */
                psm_event = EVT_WAITING;
                break;
            } else if (psm_event == EVT_ERROR) {
                LOG("error connecting\r\n");
                break;
            }
        }  
    }    
}

app_state_t app_powersave_tick(void)
{
    return APP_INPROC;
}