#ifndef CPH_
#define CPH_
/*
 * https://sourceforge.net/p/predef/wiki/Architectures/
 * https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
 * 
 */
#if defined(__linux__)
    #define PLATFORM "linux"
#elif defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
    #include <sys/param.h>
    #if defined(BSD)
        #define PLATFORM "bsd" // BSD, FreeBSD, OpenBSD, NetBSD, , DragonFly 
    #endif
#elif defined(__APPLE__) && defined(__MACH__)
    #if TARGET_OS_MAC == 1
        #define PLATFORM "osx"
    #elif TARGET_OS_IPHONE == 1
        #define PLATFORM "ios"
    #elif TARGET_IPHONE_SIMULATOR == 1
        #define PLATFORM "ios"
    #endif
#elif defined(_WIN32) || defined(_WIN64)
    #define PLATFORM "windows"
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define PLATFORM "windows"
#elif defined(__arm__)
    #define PLATFORM "arm"

#endif


// #ifndef LOG_TRACE
// #define LOG_TRACE
// #endif

#ifndef LOG_ERROR
#define LOG_ERROR
#endif





#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>



#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


#include "log.h"


#endif