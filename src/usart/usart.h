#ifndef MODEM_DEFS_H_
#define MODEM_DEFS_H_

typedef void (*usart_rx_cb_t)(uint8_t);


void usart_set_callback(usart_rx_cb_t cb);
void usart_init(void);
void usart_tick(void);
void usart_tx(char *buffer, uint8_t len);
void usart_rx(char *buffer, uint8_t len);
void usart_close(void);

#endif

