#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/engine_subsystems.h"
#include "crossplatform/crossplatform_time.h"

void render_16bit(screen_buffer* const screen, uint16_t color)
{
	uint16_t* pixels = (uint16_t*)screen->pixels;

	clktimer timer;
	clktimer_restart(&timer);
	for (int j = 0; j < screen->sizeInPixels; j++)
	{
		pixels[j] = color;
	}
	deltatime drawTime = clktimer_elapsed_ms(&timer);

	clktimer_restart(&timer);
	if (engine_render_screen(screen))
	{
		fprintf(stderr, "Failed to render screen, exiting...");
		engine_destroy_render_subsystem(screen);
		getchar();
		exit(EXIT_FAILURE);
	}
	deltatime renderTime = clktimer_elapsed_ms(&timer);

	printf("%.3f\t%.3f\n", drawTime, renderTime);
}

void render_32bit(screen_buffer* const screen, uint32_t color)
{
	uint32_t* pixels = (uint32_t*)screen->pixels;

	clktimer timer;
	clktimer_restart(&timer);
	for (int j = 0; j < screen->sizeInPixels; j++)
	{
		pixels[j] = color;
	}
	deltatime drawTime = clktimer_elapsed_ms(&timer);

	clktimer_restart(&timer);
	if (engine_render_screen(screen))
	{
		fprintf(stderr, "Failed to render screen, exiting...");
		engine_destroy_render_subsystem(screen);
		getchar();
		exit(EXIT_FAILURE);
	}
	deltatime renderTime = clktimer_elapsed_ms(&timer);

	printf("%.3f\t%.3f\n", drawTime, renderTime);
}

void run_format_test(colformat format)
{
	screen_format sformat =
	{
		.width = 640,
		.height = 480,
		.format = format
	};

	screen_buffer screen = engine_get_default_screen();

	if (engine_init_render_subsystem(&sformat, &screen))
	{
		fprintf(stderr, "Failed to init render subsystem, exiting...");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("Render subsystem initialised\n");

	if (format == CF_ARGB)
	{
		render_32bit(&screen, 0xFFFF0000);
		cross_sleep_s(1);
		render_32bit(&screen, 0xFF00FF00);
		cross_sleep_s(1);
		render_32bit(&screen, 0xFF0000FF);
		cross_sleep_s(1);
		printf("\n");
	}
	else if (format == CF_RGB565)
	{
		render_16bit(&screen, 0b1111100000000000);
		cross_sleep_s(1);
		render_16bit(&screen, 0b0000011111100000);
		cross_sleep_s(1);
		render_16bit(&screen, 0b0000000000011111);
		cross_sleep_s(1);
		printf("\n");
	}

	if (engine_destroy_render_subsystem(&screen))
	{
		fprintf(stderr, "Failed to destroy render subsystem, exiting...");
		getchar();
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
	printf("Starting render subsystem test...\n");

	printf("Setting CF ARGB\n");
	run_format_test(CF_ARGB);

	printf("Setting CF RGB565\n");
	run_format_test(CF_RGB565);

	printf("Render subsystem destroyed\n");
	printf("Test complete\n");
	getchar();
}