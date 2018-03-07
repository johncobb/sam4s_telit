#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <cph.h>

#define RING_BUFFER_SIZE    128

typedef struct
{
	unsigned char buffer[RING_BUFFER_SIZE];
	int head;
	int tail;
} usart_ring_buffer_t;


void usart_put_char(usart_ring_buffer_t *usart_buffer, uint8_t c);
uint8_t usart_clear_buffer(usart_ring_buffer_t *usart_buffer);
uint8_t usart_data_available(usart_ring_buffer_t *usart_buffer);
uint8_t usart_data_read(usart_ring_buffer_t *usart_buffer);

#endif
