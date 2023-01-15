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

void cross_sleep_s(uint32_t secs)
{
#if TIMER_PLATFORM_WINDOWS

	Sleep(secs * 1000);

#elif TIMER_PLATFORM_RPI

	sleep(secs);

#elif TIMER_PLATFORM_UNIX

	sleep(secs);

#endif
}

void cross_sleep_ms(uint32_t millis)
{
#if TIMER_PLATFORM_WINDOWS

	Sleep(millis);

#elif TIMER_PLATFORM_RPI

	usleep(millis * 1000);

#elif TIMER_PLATFORM_UNIX

	usleep(millis * 1000);

#endif
}

clktick cross_get_ticks()
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

deltatime cross_delta_s(const clktick elapsedTicks)
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

deltatime cross_delta_ms(const clktick elapsedTicks)
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

deltatime cross_delta_us(const clktick elapsedTicks)
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

void clktimer_start(clktimer* const timer)
{
	timer->startTime = cross_get_ticks();
}

void clktimer_restart(clktimer* const timer)
{
	timer->startTime = cross_get_ticks();
}

deltatime clktimer_restart_s(clktimer* const timer)
{
	deltatime delta = clktimer_elapsed_s(timer);
	clktimer_restart(timer);

	return delta;
}

deltatime clktimer_restart_ms(clktimer* const timer)
{
	deltatime delta = clktimer_elapsed_ms(timer);
	clktimer_restart(timer);

	return delta;
}

deltatime clktimer_elapsed_s(const clktimer* const timer)
{
	return cross_delta_s(cross_get_ticks() - timer->startTime);
}

deltatime clktimer_elapsed_ms(const clktimer* const timer)
{
	return cross_delta_ms(cross_get_ticks() - timer->startTime);
}

void clktimer_print_elapsed_s(const clktimer* const timer)
{
	printf("%f s\n", clktimer_elapsed_s(timer));
}

void clktimer_print_elapsed_ms(const clktimer* const timer)
{
	printf("%f ms\n", clktimer_elapsed_ms(timer));
}
