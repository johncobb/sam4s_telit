
#include <cph.h>

#include "serial_driver.h"
#include "usart.h"

usart_rx_cb_t usart_rx_cb;


void serial_rx_callback(uint8_t c)
{
    // printf("data received: %c\r\n", c);
    // TODO: Following code needs testing
    usart_rx_cb(c);
}
void usart_init(void)
{
    serial_port_init(SERIAL_IO_PATH);
    serial_set_callback(serial_rx_callback);
    
}

void usart_set_callback(usart_rx_cb_t cb)
{
    // TODO: Following code needs testing
    printf("usart_set_callback: \r\n");
    usart_rx_cb = cb;
}

void usart_tick(void)
{
    serial_port_tick();
}

void usart_tx(char *buffer, uint8_t len)
{
    serial_port_write(buffer, len);
}
void usart_rx(char *buffer, uint8_t len)
{
    serial_port_read(buffer, len);
}

void usart_close(void)
{
    serial_port_close();
}