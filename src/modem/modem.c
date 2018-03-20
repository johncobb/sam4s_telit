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


modem_event_list_t modem_events[] = {
    {MODEM_TOKEN_OK, EVT_OK},
    {MODEM_TOKEN_ERROR, EVT_ERROR},
    {MODEM_TOKEN_CONNECT, EVT_CONNECT},
    {MODEM_TOKEN_NOCARRIER, EVT_NOCARRIER},
    {MODEM_TOKEN_PROMPT, EVT_PROMPT},
    {MODEM_TOKEN_CMGS, EVT_CMGS},
    {MODEM_TOKEN_CREG, EVT_CREG},
    {MODEM_TOKEN_CSQ, EVT_CSQ},
    {MODEM_TOKEN_MONI, EVT_MONI},
    {MODEM_TOKEN_GPRS_ACT, EVT_GPRS_ACT},
    {MODEM_TOKEN_SGACT, EVT_SGACT},
    {MODEM_TOKEN_ACK, EVT_ACK},
    {MODEM_TOKEN_CMGL, EVT_CMGL},
    {MODEM_TOKEN_CMGR, EVT_CMGR},
    {MODEM_TOKEN_SOCKETSTATUS, EVT_SOCKETSTATUS},
    {MODEM_TOKEN_SOCKETSTATUS_ID, EVT_SOCKETTATUS_ID},
    {MODEM_TOKEN_REMOTECMD, EVT_REMOTECMD}
};

uint8_t modem_get_eventlist_size(void)
{
    return sizeof(modem_events);
}

static usart_ring_buffer_t modem_ring_buffer = {{0}, 0, 0};

modem_event_handler_t modem_evt;



void hw_init(void);
void modem_handle_data(void);

void modem_rx_cb(uint8_t c)
{
    LOGT("data received: %c\r\n", c);
    usart_put_char(&modem_ring_buffer, c);
}

void modem_set_ondatareceive_func(modem_ondatareceive_func_t func)
{
    modem_evt.on_datareceive = func;
}

/*
 * The onreceive method is used to handle modem events sent from 
 * the modem. Example: CONNECT, NO CARRIER, BUSY, NO DIALTONE
 */
void modem_set_oneventreceive_func(modem_oneventreceive_func_t func)
{
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
                modem_evt.on_datareceive(buffer, (i+1));
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
    /*
     * Loop through the modem_events array to determine the modem event.
     * Example: response "OK" -> EVT_OK
     */
    for (int i=0; i<sizeof(modem_events)-1; i++) {

        uint8_t *ptr = NULL;

        if (modem_parse_event(modem_events[i].token, buffer, &ptr) > 0) {
            return modem_events[i].event;
            break;
        }
    }
    /* Well we didn't see this coming */
    return EVT_UNKNOWN;
}



sys_result modem_data_handler2(uint8_t *buffer, uint32_t len) {

    char *ptr = NULL;

    if ((ptr = strstr(buffer, "OK"))) {
        return SYS_AT_OK;
    } else if ((ptr = strstr(buffer, "CONNECT"))) {
        return SYS_AT_CONNECT;
    } else if ((ptr = strstr(buffer, "ERROR"))) {
        LOGE("error: %s\r\n", buffer);
        return SYS_MODEM_ERROR;
    } else if ((ptr = strstr(buffer, "NO CARRIER"))) {
        return SYS_MODEM_NOCARRIER;
    } else {
        return SYS_OK;
    }
}

// void modem_event_t modem_identify_event(uint8_t *buffer)
// {
//     if ((ptr = strstr(buffer, "OK"))) {
//         return SYS_AT_OK;
//     } else if ((ptr = strstr(buffer, "CONNECT"))) {
//         return SYS_AT_CONNECT;
//     } else if ((ptr = strstr(buffer, "ERROR"))) {
//         LOGE("error: %s\r\n", buffer);
//         return SYS_MODEM_ERROR;
//     } else if ((ptr = strstr(buffer, "NO CARRIER"))) {
//         return SYS_MODEM_NOCARRIER;
//     } else {
//         return SYS_OK;
//     }   
// }

// sys_result modem_data_handler(char *data){

//     char *ptr = NULL;

//     if ((ptr = strstr(data, "OK"))) {
//         return SYS_AT_OK;
//     } else if ((ptr = strstr(data, "CONNECT"))) {
//         return SYS_AT_CONNECT;
//     } else if ((ptr = strstr(data, "ERROR"))) {
//         printf("error: %s\r\n", data);
//         return SYS_MODEM_ERROR;
//     } else if ((ptr = strstr(data, "NO CARRIER"))) {
//         return SYS_MODEM_NOCARRIER;
//     } else {
//         return SYS_OK;
//     }
// }


// sys_result handle_result(char *token, char **ptr_out)
// {
// 	if ((ptr = strstr(modem_rx_buffer, token))) {
// 		if(ptr_out != NULL) {
// 			*ptr_out = ptr;
// 		}
// 		printf("SYS_AT_OK\r\n");
// 		return SYS_AT_OK;
// 	} else if ((ptr = strstr(modem_rx_buffer, MODEM_TOKEN_ERROR))) {
// 		if(ptr_out != NULL) {
// 			*ptr_out = ptr;
// 		}
// 		printf("SYS_MODEM_ERROR\r\n");
// 		return SYS_MODEM_ERROR;
// 	} else if ((ptr = strstr(modem_rx_buffer, MODEM_TOKEN_NOCARRIER))) {
// 		if(ptr_out != NULL) {
// 			*ptr_out = ptr;
// 		}
// 		printf("SYS_MODEM_NOCARRIER\r\n");
// 		return SYS_MODEM_NOCARRIER;
// 	}

// 	// set ptr_out to the rx_buffer for troubleshooting
// 	if (ptr_out != NULL) {
// 		*ptr_out = modem_rx_buffer;
//     }
    
//     return SYS_OK;
// }