#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_subsystems.h"
#include "time_helper.h"

int main(int argc, char** argv)
{
	printf("Starting render subsystem test...\n");

	screen_format sformat =
	{
		.width = 640,
		.height = 480,
		.format = CF_ARGB
	};

	screen_buffer screen = default_screen();

	if (init_render_subsystem(&sformat, &screen))
	{
		fprintf(stderr, "Failed to init render subsystem, exiting...");
		exit(EXIT_FAILURE);
	}

	printf("Render subsystem initialised\n");

	clktimer timer;

	for (int i = 0; i < 5; i++)
	{
		uint32_t* pixels = (uint32_t*)screen.pixels;

		resart_timer(&timer);
		for (int j = 0; j < screen.sizeInPixels; j++)
		{
			pixels[j] = 0xFF00FF00;
		}
		print_elapsed_millis(&timer);

		resart_timer(&timer);
		render_screen(&screen);
		print_elapsed_millis(&timer);

		sleep_secs(1);
		
		for (int j = 0; j < screen.sizeInPixels; j++)
		{
			pixels[j] = 0xFF0000FF;
		}

		render_screen(&screen);
		sleep_secs(1);
	}

	if (destroy_render_subsystem(&screen))
	{
		fprintf(stderr, "Failed to destroy render subsystem, exiting...");
		exit(EXIT_FAILURE);
	}

	printf("Render subsystem destroyed\n");
	printf("Test complete\n");
	getchar();
}