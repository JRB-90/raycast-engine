#include "crossplatform/crossplatform_time.h"

#if defined( _WIN32 ) || defined( _WIN64 )
#define TIMER_PLATFORM_WINDOWS 1
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#elif defined( _RPI )
#define TIMER_PLATFORM_RPI 1
#define BCM2708_ST_BASE 0x20003000
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#elif defined(__unix__)
#define TIMER_PLATFORM_UNIX 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#else
#error "Only windows/rpi/unix supported in the time library"
#endif

#if TIMER_PLATFORM_RPI
bool isTimerInitialised = false;
volatile unsigned* timerRegisters;

void init_rpitimer()
{
	int timerFd = open("/dev/mem", O_RDWR);
	if (timerFd < 0)
	{
		fprintf(stderr, "RPI timer init failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

	void* timerMemMap =
		mmap(
			NULL,
			4096,
			PROT_READ,
			MAP_SHARED,
			timerFd,
			BCM2708_ST_BASE
		);

	close(timerFd);

	if (timerMemMap == MAP_FAILED)
	{
		fprintf(stderr, "RPI timer init failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

	timerRegisters = (volatile unsigned*)timerMemMap;

	isTimerInitialised = true;
}
#endif

void sleep_secs(uint32_t secs)
{
#if TIMER_PLATFORM_WINDOWS

	Sleep(secs * 1000);

#elif TIMER_PLATFORM_RPI

	sleep(secs);

#elif TIMER_PLATFORM_UNIX

	sleep(secs);

#endif
}

void sleep_millis(uint32_t millis)
{
#if TIMER_PLATFORM_WINDOWS

	Sleep(millis);

#elif TIMER_PLATFORM_RPI

	usleep(millis * 1000);

#elif TIMER_PLATFORM_UNIX

	usleep(millis * 1000);

#endif
}

clktick get_ticks()
{
#if TIMER_PLATFORM_WINDOWS

	LARGE_INTEGER t;
	if (QueryPerformanceCounter(&t))
	{
		return t.QuadPart;
	}
	else
	{
		fprintf(stderr, "Windows timer failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

#elif TIMER_PLATFORM_RPI

	if (!isTimerInitialised)
	{
		init_rpitimer();
	}

	return (clktick)timerRegisters[1];

#elif TIMER_PLATFORM_UNIX

	struct timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t))
	{
		fprintf(stderr, "Unix timer failed, exiting...\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		return (t.tv_sec * (1000000000)) + (t.tv_nsec);
	}

#endif
}

deltatime get_delta_s(const clktick elapsedTicks)
{
#if TIMER_PLATFORM_WINDOWS

	LARGE_INTEGER f;
	if (QueryPerformanceFrequency(&f))
	{
		return (deltatime)elapsedTicks / ((deltatime)f.QuadPart);
	}
	else
	{
		fprintf(stderr, "Windows timer failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

#elif TIMER_PLATFORM_RPI

	return (deltatime)(elapsedTicks / 1000000);

#elif TIMER_PLATFORM_UNIX

	return (deltatime)(elapsedTicks / 1000000000);

#endif
}

deltatime get_delta_ms(const clktick elapsedTicks)
{
#if TIMER_PLATFORM_WINDOWS

	LARGE_INTEGER f;
	if (QueryPerformanceFrequency(&f))
	{
		return (deltatime)elapsedTicks / ((deltatime)f.QuadPart / 1000);
	}
	else
	{
		fprintf(stderr, "Windows timer failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

#elif TIMER_PLATFORM_RPI

	return (deltatime)(elapsedTicks / 1000);

#elif TIMER_PLATFORM_UNIX

	return (deltatime)(elapsedTicks / 1000000);

#endif
}

deltatime get_delta_us(const clktick elapsedTicks)
{
#if TIMER_PLATFORM_WINDOWS

	LARGE_INTEGER f;
	if (QueryPerformanceFrequency(&f))
	{
		return (deltatime)elapsedTicks / ((deltatime)f.QuadPart / 1000000);
	}
	else
	{
		fprintf(stderr, "Windows timer failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

#elif TIMER_PLATFORM_RPI

	return (deltatime)(elapsedTicks);

#elif TIMER_PLATFORM_UNIX

	return (deltatime)(elapsedTicks / 1000);

#endif
}

void start_timer(clktimer* const timer)
{
	timer->startTime = get_ticks();
}

void resart_timer(clktimer* const timer)
{
	timer->startTime = get_ticks();
}

deltatime restart_timer_secs(clktimer* const timer)
{
	deltatime delta = elapsed_secs(timer);
	resart_timer(timer);

	return delta;
}

deltatime restart_timer_millis(clktimer* const timer)
{
	deltatime delta = elapsed_millis(timer);
	resart_timer(timer);

	return delta;
}

deltatime elapsed_secs(const clktimer* const timer)
{
	return get_delta_s(get_ticks() - timer->startTime);
}

deltatime elapsed_millis(const clktimer* const timer)
{
	return get_delta_ms(get_ticks() - timer->startTime);
}

void print_elapsed_secs(const clktimer* const timer)
{
	printf("%f s\n", elapsed_secs(timer));
}

void print_elapsed_millis(const clktimer* const timer)
{
	printf("%f ms\n", elapsed_millis(timer));
}
