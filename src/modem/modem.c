#include <cph.h>
#include "ring_buffer.h"
#include "usart.h"
#include "modem.h"
#include "modem_defs.h"

static usart_ring_buffer_t modem_ring_buffer = {{0}, 0, 0};

modem_event_handler_t modem_evt;

void hw_init(void);
void modem_handle_data(void);

void modem_rx_cb(uint8_t c)
{
    // printf("data received: %c\r\n", c);
    usart_put_char(&modem_ring_buffer, c);
}

void modem_set_ondatareceive_func(modem_ondatareceive_func_t func)
{
    modem_evt.on_datareceive = func;
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
    printf("modem_init: initializing hardware...\r\n");
    hw_init();

    printf("modem_init: configuring usart...\r\n");
    usart_init();   
    usart_set_callback(modem_rx_cb);

    return SYS_OK;
}

void modem_tick(void)
{
    usart_tick();
    modem_handle_data();
}

void modem_handle_data(void)
{
    char buffer[MODEM_RX_BUFFER_SIZE] = {0};
    uint8_t bytes = modem_data_available();

    // printf("modem_data_available: %d\r\n", bytes);
    // printf("modem_data_buffer: ");
    for (int i=0; i<bytes; i++) {

        
        uint8_t c = modem_data_read();
        // printf("%c", c);
        // modem_buffer[modem_buffer_index++] = c;
        buffer[i] = c;

        if (c == ((uint8_t)TOKEN_END)) {

            modem_evt.on_datareceive(buffer);
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
    printf("hw_init: true\r\n");
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
    uint8_t len = strlen((char *) cmd);
    usart_tx(cmd, len);   
}


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