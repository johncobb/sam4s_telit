#include "ring_buffer.h"


void usart_put_char(usart_ring_buffer_t *buffer, uint8_t c)
{
	int i = (unsigned int)(buffer->head + 1) % RING_BUFFER_SIZE;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != buffer->tail) {
		buffer->buffer[buffer->head] = c;
		buffer->head = i;
	}
}

uint8_t usart_clear_buffer(usart_ring_buffer_t *buffer)
{
	return (uint8_t)(RING_BUFFER_SIZE + buffer->head - buffer->tail) % RING_BUFFER_SIZE;
}

uint8_t usart_data_available(usart_ring_buffer_t *buffer)
{
	return (uint8_t)(RING_BUFFER_SIZE + buffer->head - buffer->tail) % RING_BUFFER_SIZE;
}

uint8_t usart_data_read(usart_ring_buffer_t *buffer)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (buffer->head == buffer->tail) {
		return -1;
	} else {
		uint8_t c = buffer->buffer[buffer->tail];
		buffer->tail = (unsigned int)(buffer->tail + 1) % RING_BUFFER_SIZE;
		return c;
	}
}