#include "cph_clock.h"

struct timespec ts;

void _cph_delay_ms(uint32_t millis)
{
    // millis * micros
    usleep(millis*1000);
}

/*
 * Time:
 * https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux
 * https://gist.github.com/jbenet/1087739
 * http://www.raspberry-projects.com/pi/programming-in-c/timing/clock_gettime-for-acurate-timing
 */
void current_utc_time(struct timespec *ts)
{
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
}

clock_time_t _cph_get_millis()
{

    current_utc_time(&ts);

    // printf("s:  %lu\n", ts.tv_sec);
    // printf("ns: %lu\n", ts.tv_nsec);

    // return ts.tv_nsec;
    return round(ts.tv_nsec /1.0e6f);

}

