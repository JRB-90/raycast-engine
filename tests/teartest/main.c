#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "engine/engine_subsystems.h"
#include "engine/engine_rayengine.h"
#include "engine/engine_color.h"
#include "crossplatform/crossplatform_time.h"

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
	screen = engine_get_default_screen();

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

	engine_init_render_subsystem(&format, &screen);

	clear_screen();
	draw_bars();
	engine_render_screen(&screen);
	cross_sleep_s(1);

	int numRuns = 300;
	clktimer_start(&clock);

	for (int i = 0; i < numRuns; i++)
	{
		clktimer_start(&clock2);
		clear_screen();
		draw_bars();
		innerDrawDelta += clktimer_elapsed_ms(&clock2);

		clktimer_start(&clock2);
		engine_render_screen(&screen);
		innerFlipDelta += clktimer_elapsed_ms(&clock2);
		
		barOffset++;
		if (barOffset >= BWIDTH)
		{
			barOffset = 0;
		}
	}

	deltatime delta = clktimer_elapsed_ms(&clock);
	deltatime aveDelta = delta / (deltatime)(numRuns);
	printf("Time taken: %.3fms\n", delta);
	printf("Ave frame:  %.3fms\n", aveDelta);
	printf("Inner draw ave:  %.3fms\n", innerDrawDelta / (deltatime)(numRuns));
	printf("Inner flip ave:  %.3fms\n", innerFlipDelta / (deltatime)(numRuns));

	cross_sleep_s(1);

	engine_destroy_render_subsystem(&screen);

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
