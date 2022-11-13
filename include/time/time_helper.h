#pragma once

#include <inttypes.h>

#if defined(_WIN32) || defined(_WIN6 )
typedef unsigned __int64 tick_t;
#elif defined(_RPI)
typedef long tick_t;
#elif defined(__unix__)
typedef uint64_t tick_t;
#else
#error "Only windows/rpi/unix supported in the time library"
#endif

typedef float deltatime_t;

typedef struct {
	tick_t startTime;
} clktimer_t;

void sleep_secs(uint32_t secs);
void sleep_millis(uint32_t millis);

tick_t get_ticks();
deltatime_t get_delta_s(const tick_t elapsedTicks);
deltatime_t get_delta_ms(const tick_t elapsedTicks);
deltatime_t get_delta_us(const tick_t elapsedTicks);

void start_timer(clktimer_t* const timer);
void resart_timer(clktimer_t* const timer);
deltatime_t restart_timer_secs(clktimer_t* const timer);
deltatime_t restart_timer_millis(clktimer_t* const timer);
deltatime_t elapsed_secs(const clktimer_t* const timer);
deltatime_t elapsed_millis(const clktimer_t* const timer);
void print_elapsed_secs(const clktimer_t* const timer);
void print_elapsed_millis(const clktimer_t* const timer);