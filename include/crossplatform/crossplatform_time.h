#pragma once

/*
	Collection of cross platform time tools, including sleep and time measurement
	tools with various options for time units.
*/

#include <inttypes.h>

#if defined(_WIN32) || defined(_WIN6 )
typedef unsigned __int64 clktick;
#elif defined(_RPI)
typedef long clktick;
#elif defined(__unix__)
typedef uint64_t clktick;
#else
#error "Only windows/rpi/unix supported in the time library"
#endif

typedef float deltatime;

typedef struct {
	clktick startTime;
} clktimer;

// Sleeps current thread for time requested
void cross_sleep_s(uint32_t secs);
void cross_sleep_ms(uint32_t millis);

// Gets the platforms highest resolution tick count, which can be used to 
// calculate time deltas.
// Note, tick count is not transferrable between platforms.
clktick cross_get_ticks();
deltatime cross_delta_s(const clktick elapsedTicks);
deltatime cross_delta_ms(const clktick elapsedTicks);
deltatime cross_delta_us(const clktick elapsedTicks);

// Timer helper functions
void clktimer_start(clktimer* const timer);
void clktimer_restart(clktimer* const timer);
deltatime clktimer_restart_s(clktimer* const timer);
deltatime clktimer_restart_ms(clktimer* const timer);
deltatime clktimer_elapsed_s(const clktimer* const timer);
deltatime clktimer_elapsed_ms(const clktimer* const timer);
void clktimer_print_elapsed_s(const clktimer* const timer);
void clktimer_print_elapsed_ms(const clktimer* const timer);