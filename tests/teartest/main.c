#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "engine_subsystems.h"
#include "engine_screen.h"
#include "engine_color.h"
#include "time_helper.h"

const int SWIDTH = 640;
const int SHEIGHT = 480;
int BWIDTH = 50;

screen_buffer screen;
int barOffset;

void clear_screen();
void draw_bars();

int main(int argc, char** argv)
{
	printf("Starting...\n");

	barOffset = 0;
	screen = default_screen();

	clktimer clock;
	clktimer clock2;
	deltatime innerDrawDelta = 0;
	deltatime innerFlipDelta = 0;

	screen_format format =
	{
		.width = SWIDTH,
		.height = SHEIGHT,
		.format = CF_RGB565
	};

	init_render_subsystem(&format, &screen);

	clear_screen();
	draw_bars();
	render_screen(&screen);
	sleep_secs(1);

	int numRuns = 300;
	start_timer(&clock);

	for (int i = 0; i < numRuns; i++)
	{
		start_timer(&clock2);
		clear_screen();
		draw_bars();
		innerDrawDelta += elapsed_millis(&clock2);

		start_timer(&clock2);
		render_screen(&screen);
		innerFlipDelta += elapsed_millis(&clock2);
		
		barOffset++;
		if (barOffset >= BWIDTH)
		{
			barOffset = 0;
		}
	}

	deltatime delta = elapsed_millis(&clock);
	deltatime aveDelta = delta / (deltatime)(numRuns);
	printf("Time taken: %.3fms\n", delta);
	printf("Ave frame:  %.3fms\n", aveDelta);
	printf("Inner draw ave:  %.3fms\n", innerDrawDelta / (deltatime)(numRuns));
	printf("Inner flip ave:  %.3fms\n", innerFlipDelta / (deltatime)(numRuns));

	sleep_secs(1);

	destroy_render_subsystem(&screen);

	printf("Done\n");
	//getchar();
	exit(EXIT_SUCCESS);
}

void clear_screen()
{
	uint16_t *pixels = (uint16_t *)screen.pixels;
	for (int i = 0; i < screen.sizeInPixels; i++)
	{
		pixels[i] = 0b1111111111111111;
	}
}

void draw_bars()
{
	uint16_t* pixels = (uint16_t*)screen.pixels;
	int halfBarWidth = BWIDTH / 2;
	int xPos = barOffset;

	for (int x = 0; x < SWIDTH; x++)
	{
		if (xPos < halfBarWidth)
		{
			for (int pIndex = x; pIndex < screen.sizeInPixels; pIndex += SWIDTH)
			{
				pixels[pIndex] = 0x0000;
			}
		}

		xPos++;
		if (xPos >= BWIDTH)
		{
			xPos = 0;
		}
	}
}
