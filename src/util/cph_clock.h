#ifndef CPH_CLOCK_
#define CPH_CLOCK_

#include <cph.h>


typedef uint32_t clock_time_t;
extern clock_time_t _cph_get_millis();
extern void _cph_delay_ms(uint32_t millis);

#endif
