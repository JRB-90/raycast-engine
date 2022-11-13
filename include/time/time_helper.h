#pragma once

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

void sleep_secs(uint32_t secs);
void sleep_millis(uint32_t millis);

clktick get_ticks();
deltatime get_delta_s(const clktick elapsedTicks);
deltatime get_delta_ms(const clktick elapsedTicks);
deltatime get_delta_us(const clktick elapsedTicks);

void start_timer(clktimer* const timer);
void resart_timer(clktimer* const timer);
deltatime restart_timer_secs(clktimer* const timer);
deltatime restart_timer_millis(clktimer* const timer);
deltatime elapsed_secs(const clktimer* const timer);
deltatime elapsed_millis(const clktimer* const timer);
void print_elapsed_secs(const clktimer* const timer);
void print_elapsed_millis(const clktimer* const timer);