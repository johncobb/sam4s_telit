#ifndef CPH_CLOCK_
#define CPH_CLOCK_

#include <cph.h>


void _debug_stringz( char* data );
void _debug_byte( unsigned char byte );
void _debug_byte_as_hex( unsigned char byte );
// void _debug_set_rx_cb(debug_rx_cb_t);
// void _debug_set_out_cb(debug_out_cb_t);
void _debug_hex_dump( uint8_t * data, int length);
void _debug_log_float(float f);
void _debug_log(const char * prefix, const char * fmt, ...);


#define debug_stringz(x) _debug_stringz(x);
#define debug_byte(x) _debug_byte(x);
#define debug_byte_as_hex(x) _debug_byte_as_hex(x);
// #define debug_set_rx_cb(x) _debug_set_rx_cb(x);
// #define debug_set_out_cb(x) _debug_set_out_cb(x);
#define debug_hex_dump(x,y) _debug_hex_dump(x,y);
#define debug_log_float(x) _debug_log_float(x);
#define debug_log(x,y,...) _debug_log(x,y,##__VA_ARGS__);

#ifdef __APPLE__
#define LOG(fmt, ...) { debug_log(_tag, fmt, ##__VA_ARGS__); }
#else
#define LOG(fmt, ...) { debug_log(_tag, PSTR(fmt), ##__VA_ARGS__); }
#endif
#endif