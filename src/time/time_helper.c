#include "time_helper.h"

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
