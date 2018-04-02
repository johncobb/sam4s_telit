#include <cph.h>
#include "ring_buffer.h"
#include "usart.h"
#include "modem.h"
#include "modem_defs.h"

// NO DIALTONE
// BUSY
// NO CARRIER
// CONNECT
// CONNECT 115200

#if defined(__arm__)
static const char _tag[] PROGMEM = "modem: "; /* used in embedded gcc */
#else
static const char _tag[] = "modem: ";
#endif

// void modem_handle_activatecontext(uint8_t*);

modem_event_list_t modem_events[] = {
    {MODEM_TOKEN_OK, EVT_OK, NULL},
    {MODEM_TOKEN_ERROR, EVT_ERROR, NULL},
    {MODEM_TOKEN_CONNECT, EVT_CONNECT, NULL},
    {MODEM_TOKEN_NOCARRIER, EVT_NOCARRIER, NULL},
    {MODEM_TOKEN_PROMPT, EVT_PROMPT, NULL},
    {MODEM_TOKEN_CMGS, EVT_CMGS, NULL},
    {MODEM_TOKEN_CREG, EVT_CREG, modem_handle_creg},
    {MODEM_TOKEN_CSQ, EVT_CSQ, modem_handle_signalstrength},
    {MODEM_TOKEN_MONI, EVT_MONI, NULL},
    {MODEM_TOKEN_GPRS_ACT, EVT_GPRS_ACT, NULL},
    {MODEM_TOKEN_SGACT, EVT_SGACT, modem_handle_activatecontext},
    {MODEM_TOKEN_ACK, EVT_ACK, NULL},
    {MODEM_TOKEN_CMGL, EVT_CMGL, NULL},
    {MODEM_TOKEN_CMGR, EVT_CMGR, NULL},
    {MODEM_TOKEN_SOCKETSTATUS, EVT_SOCKETSTATUS, NULL},
    {MODEM_TOKEN_REMOTECMD, EVT_REMOTECMD, NULL}
};

// void modem_handle_activatecontext(uint8_t *buffer)
// {
// 	// example result: #SGACT: 1,0
//     buffer+=10;
//     // modem_status.context = ((buffer[0]-'0'));
//     // LOG("creg: %d\r\n", modem_status.creg);
//     LOG("creg: %d\r\n", ((buffer[0]-'0')));
// }



uint8_t modem_get_eventlist_size(void)
{
    return sizeof(modem_events);
}

static usart_ring_buffer_t modem_ring_buffer = {{0}, 0, 0};

modem_event_handler_t modem_evt;

modem_status_t modem_status;


void hw_init(void);
void modem_handle_data(void);

void modem_rx_cb(uint8_t c)
{
    LOGT("data received: %c\r\n", c);
    usart_put_char(&modem_ring_buffer, c);
}

void modem_set_ondatareceive_func(modem_ondatareceive_func_t func)
{
    LOGT("modem_set_ondatareceive_func: \r\n");
    modem_evt.on_datareceive = func;
}

/*
 * The onreceive method is used to handle modem events sent from 
 * the modem. Example: CONNECT, NO CARRIER, BUSY, NO DIALTONE
 */
void modem_set_oneventreceive_func(modem_oneventreceive_func_t func)
{
    LOGT("modem_set_oneventreceive_func: \r\n");
    modem_evt.on_eventreceive = func;
}

uint8_t modem_data_available(void)
{
    return usart_data_available(&modem_ring_buffer);
}

uint8_t modem_data_read(void)
{
    return usart_data_read(&modem_ring_buffer);
}

sys_result modem_init(void)
{
    LOG("modem_init: initializing hardware...\r\n");
    hw_init();

    LOG("modem_init: configuring usart...\r\n");
    usart_init();   
    usart_set_callback(modem_rx_cb);

    return SYS_OK;
}

void modem_tick(void)
{
    usart_tick();
    modem_handle_data();
}

/*
 * modem responseses are wrapped between
 * two CR LF characters. The following
 * function reads the modem resposne and invokes
 * the on_datareceive method when a response is found.
 * Example: "\r\nOK\r\n"
 */
uint8_t token_count = 0;

void modem_handle_data(void)
{
    char buffer[MODEM_RX_BUFFER_SIZE] = {0};
    uint8_t bytes = modem_data_available();

    LOGT("modem_data_available: %d\r\n", bytes);
    for (int i=0; i<bytes; i++) {

        uint8_t c = modem_data_read();

        buffer[i] = c;

        if (c == ((uint8_t)TOKEN_END)) {
            if (++token_count == 2) {
                token_count = 0;
                modem_evt.on_eventreceive(buffer, (i+1));
                break;
            }


        }
    }

}

void modem_read(char *buffer, uint8_t len)
{
    usart_rx(buffer, len);
}

void modem_close(void)
{
    usart_close();
}

void hw_init(void)
{
    LOG("hw_init: true\r\n");
	// Sanity check make sure pins are low
	// pio_set_pin_low(MDM_ENABLE_IDX);
	// pio_set_pin_low(MDM_RESET_IDX);
	// pio_set_pin_low(MDM_ONOFF_IDX);
    
    // uint32_t powmon = 0;
    // uint8_t num_tries = 0;

    // while(true) {
    //     if (num_tries++ == 3) break;

    //     powmon = pio_get_pin_value(MDM_POWMON_IDX);

    //     if (powmon == 0) {
    //         printf("powmon: low\r\n");
    //         break;
    //     }
    //     pio_set_pin_low(MDM_ENABLE_IDX);
    //     delay_ms(20);
    //     pio_set_pin_high(MDM_ENABLE_IDX);
    // }

    // while (true) {
    //     printf("toggle modem_onoff: high\r\n");
    //     printf("wait 3 sec... \r\n");
    //     pio_set_pin_high("MDM_ONOFF_IDX");
    //     delay_ms(3000);

    //     printf("toggle modem_onoff: low\r\n");
    //     printf("wait 3 sec... \r\n");
    //     pio_set_pin_low("MDM_ONOFF_IDX");
    //     delay_ms(3000);

    //     powmon = pio_get_pin_value(MDM_POWMON_IDX);

    //     if (powmon == 1) {
    //         printf("powmon: high");
    //         printf("modem powered on!\r\n");
    //         break;
    //     }

    // }
}

void modem_write(char *cmd)
{
    LOG("modem_write: %s\r\n", cmd);
    uint8_t len = strlen((char *) cmd);
    usart_tx(cmd, len);   
}



/* modem_parse_event parses out tokens sent back from modem to identify event */
uint8_t modem_parse_event(uint8_t *token, uint8_t *buffer, uint8_t **ptr_out)
{

    uint8_t *ptr = NULL;
    
	if ((ptr = strstr(buffer, token))) {
		if(ptr_out != NULL) {
			*ptr_out = ptr;
		}
		LOGT("modem_parse_event: event found\r\n");
		return 1;
	} 
	/* set ptr_out to the buffer for troubleshooting */
	if (ptr_out != NULL) {
		*ptr_out = buffer;
    }

    return 0;
}

modem_event_t modem_identify_event(uint8_t *buffer)
{
    // LOG("modem_identify_event: \r\n");
    /*
     * Loop through the modem_events array to determine the modem event.
     * Example: response "OK" -> EVT_OK
     */
    for (int i=0; i<sizeof(modem_events)-1; i++) {

        uint8_t *ptr = NULL;

        if (modem_parse_event(modem_events[i].token, buffer, &ptr) > 0) {
            /*
             * TODO: Research better way to handle results. for example the following modem events
             * command AT#SGACT=1,1 to activate context results in #SGACT: 10.0.0.1
             * command AT#SGACT? to query context results in #SGACT: 1,0
             * bot commands require different handling which cannot be carried out by a single
             * fnc_handler for each modem event!
             */
            // if (modem_events[i].fnc_handler != NULL) {
            //     modem_events[i].fnc_handler(ptr);
            // }
            return modem_events[i].event;
            break;
        }
    }
    /* Well we didn't see this coming */
    return EVT_UNKNOWN;
}