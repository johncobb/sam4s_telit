
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


static const char _tag[] = "debug: ";

void _debug_byte(unsigned char byte);


#ifdef __APPLE__
void _debug_hex_dump( uint8_t * data, int length)
{
	for (int i=0;i<length;i++)
	{
        printf("%02x ", (uint8_t) data[i]);
        printf("' '");

	}
}

void _debug_log(const char * prefix, const char * fmt, ...)
{
	if (prefix) {
		printf("%s ", prefix);
	}
	va_list argptr;
	va_start(argptr, fmt);
    vprintf(fmt, argptr);
	va_end(argptr);
}

void _debug_log_v(const char * prefix, const char * fmt, ...)
{

	if (prefix) {
		printf("%s ", prefix);
	}
	va_list argptr;
	va_start(argptr, fmt);
	vprintf(fmt, argptr);
	va_end(argptr);
}

#else
void _debug_byte_as_hex( unsigned char byte )
{
	unsigned char c;

	c = pgm_read_byte(&DEBUG_hex[(byte >> 4)]);
	while ( !( UCSR0A & (1<<UDRE0)));
	UDR0 = c;

	c = pgm_read_byte(&DEBUG_hex[(byte & 0x0f)]);
	while ( !( UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void _debug_hex_dump( uint8_t * data, int length)
{
	for (int i=0;i<length;i++)
	{
		//printf_P(PSTR("%02X "), (uint8_t)data[i]);
		_debug_byte_as_hex((unsigned char)data[i]);
		//_debug_byte((uint8_t)' ');
	}
	//_debug_byte((uint8_t)'\r');
	//_debug_byte((uint8_t)'\n');
	//printf_P(PSTR("\r\n"));
}

void _debug_log(const char * prefix, const char * fmt, ...)
{
	if (prefix)
	{
		//printf_P(PSTR("%lu "), clock_millis);
		//printf_P(prefix);
	}
	va_list argptr;
	va_start(argptr, fmt);
	// Important to use vfprintf_P due to variatic signature
	// Note that there is no vprintf_P - we have to manually direct to stdout
	vfprintf_P(stdout, fmt, argptr);
	va_end(argptr);
}
#endif

