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

/// <summary>
/// Sleeps the current thread for the time requested.
/// </summary>
/// <param name="secs">Seconds to sleep.</param>
void cross_sleep_s(uint32_t secs);

/// <summary>
/// Sleeps the current thread for the time requested.
/// </summary>
/// <param name="secs">Milliseconds to sleep.</param>
void cross_sleep_ms(uint32_t millis);

/// <summary>
/// Gets the platforms highest resolution tick count, which can be used to
/// calculate time deltas.
/// Note, tick count is not transferrable between platforms.
/// </summary>
/// <returns>Tick count.</returns>
clktick cross_get_ticks();

/// <summary>
/// Converts ticks to seconds.
/// </summary>
/// <param name="elapsedTicks">Number of ticks elapsed.</param>
/// <returns>Time elapsed in seconds.</returns>
deltatime cross_delta_s(const clktick elapsedTicks);

/// <summary>
/// Converts ticks to milliseconds.
/// </summary>
/// <param name="elapsedTicks">Number of ticks elapsed.</param>
/// <returns>Time elapsed in milliseconds.</returns>
deltatime cross_delta_ms(const clktick elapsedTicks);

/// <summary>
/// Converts ticks to microseconds.
/// </summary>
/// <param name="elapsedTicks">Number of ticks elapsed.</param>
/// <returns>Time elapsed in microseconds.</returns>
deltatime cross_delta_us(const clktick elapsedTicks);

/// <summary>
/// Starts a timer running.
/// </summary>
/// <param name="timer">Timer to start.</param>
void clktimer_start(clktimer* const timer);
/// <summary>
/// Restarts a timer and sets it running again.
/// </summary>
/// <param name="timer">Timer to restart.</param>
void clktimer_restart(clktimer* const timer);

/// <summary>
/// Extracts the seconds passed since the timer started and then restarts it.
/// </summary>
/// <param name="timer">Timer to restart.</param>
/// <returns>Time passed in seconds since timer was started.</returns>
deltatime clktimer_restart_s(clktimer* const timer);

/// <summary>
/// Extracts the milliseconds passed since the timer started and then restarts it.
/// </summary>
/// <param name="timer">Timer to restart.</param>
/// <returns>Time passed in milliseconds since timer was started.</returns>
deltatime clktimer_restart_ms(clktimer* const timer);

/// <summary>
/// Gets the elapsed time in seconds sincs the timer was started.
/// </summary>
/// <param name="timer">Timer to evaluate.</param>
/// <returns>Time passed in seconds.</returns>
deltatime clktimer_elapsed_s(const clktimer* const timer);

/// <summary>
/// Gets the elapsed time in millisconds sincs the timer was started.
/// </summary>
/// <param name="timer">Timer to evaluate.</param>
/// <returns>Time passed in millisconds.</returns>
deltatime clktimer_elapsed_ms(const clktimer* const timer);

/// <summary>
/// Prints the time elapsed in seconds of the given timer.
/// </summary>
/// <param name="timer">Timer to print.</param>
void clktimer_print_elapsed_s(const clktimer* const timer);

/// <summary>
/// Prints the time elapsed in milliseconds of the given timer.
/// </summary>
/// <param name="timer">Timer to print.</param>
void clktimer_print_elapsed_ms(const clktimer* const timer);