#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "engine_subsystems.h"
#include "engine_screen.h"
#include "engine_color.h"
#include "time_helper.h"

screen_buffer screen;
int barOffset;
int barWidth;

void clear_screen();
void draw_bars();

int main(int argc, char** argv)
{
	printf("Starting...\n");

	barOffset = 0;
	barWidth = 50;
	clktimer clock;

	screen_format format =
	{
		.width = 640,
		.height = 480,
		.format = CF_RGB565
	};

	init_render_subsystem(&format, &screen);
	
	int numRuns = 200;
	start_timer(&clock);

	for (int i = 0; i < numRuns; i++)
	{
		clear_screen();
		draw_bars();
		render_screen(&screen);
		barOffset++;
	}

	deltatime delta = elapsed_millis(&clock);
	deltatime aveDelta = delta / (deltatime)(numRuns);
	printf("Time taken: %.3fms\n", delta);
	printf("Ave frame:  %.3fms\n", aveDelta);

	destroy_render_subsystem(&screen);

	printf("Done\n");
	exit(EXIT_SUCCESS);
}

void clear_screen()
{
	uint16_t* pixels = (uint16_t*)screen.pixels;
	for (int i = 0; i < screen.sizeInPixels; i++)
	{
		pixels[i] = 0b1111111111111111;
	}
}

void draw_bars()
{
	uint16_t* pixels = (uint16_t*)screen.pixels;
	for (int x = 0; x < 640; x++)
	{
		if (((x + barOffset) % barWidth) > (barWidth / 2))
		{
			for (int y = 0; y < 480; y++)
			{
				int pixelIndex = (y * 640) + x;
				pixels[pixelIndex] = 0b0000000000000000;
			}
		}
	}
}
