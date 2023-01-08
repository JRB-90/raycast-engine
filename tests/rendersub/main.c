#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/engine_subsystems.h"
#include "crossplatform/crossplatform_time.h"

void render_16bit(screen_buffer* const screen, uint16_t color)
{
	uint16_t* pixels = (uint16_t*)screen->pixels;

	clktimer timer;
	resart_timer(&timer);
	for (int j = 0; j < screen->sizeInPixels; j++)
	{
		pixels[j] = color;
	}
	deltatime drawTime = elapsed_millis(&timer);

	resart_timer(&timer);
	if (render_screen(screen))
	{
		fprintf(stderr, "Failed to render screen, exiting...");
		destroy_render_subsystem(screen);
		getchar();
		exit(EXIT_FAILURE);
	}
	deltatime renderTime = elapsed_millis(&timer);

	printf("%.3f\t%.3f\n", drawTime, renderTime);
}

void render_32bit(screen_buffer* const screen, uint32_t color)
{
	uint32_t* pixels = (uint32_t*)screen->pixels;

	clktimer timer;
	resart_timer(&timer);
	for (int j = 0; j < screen->sizeInPixels; j++)
	{
		pixels[j] = color;
	}
	deltatime drawTime = elapsed_millis(&timer);

	resart_timer(&timer);
	if (render_screen(screen))
	{
		fprintf(stderr, "Failed to render screen, exiting...");
		destroy_render_subsystem(screen);
		getchar();
		exit(EXIT_FAILURE);
	}
	deltatime renderTime = elapsed_millis(&timer);

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

	screen_buffer screen = default_screen();

	if (init_render_subsystem(&sformat, &screen))
	{
		fprintf(stderr, "Failed to init render subsystem, exiting...");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("Render subsystem initialised\n");

	if (format == CF_ARGB)
	{
		render_32bit(&screen, 0xFFFF0000);
		sleep_secs(1);
		render_32bit(&screen, 0xFF00FF00);
		sleep_secs(1);
		render_32bit(&screen, 0xFF0000FF);
		sleep_secs(1);
		printf("\n");
	}
	else if (format == CF_RGB565)
	{
		render_16bit(&screen, 0b1111100000000000);
		sleep_secs(1);
		render_16bit(&screen, 0b0000011111100000);
		sleep_secs(1);
		render_16bit(&screen, 0b0000000000011111);
		sleep_secs(1);
		printf("\n");
	}

	if (destroy_render_subsystem(&screen))
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