#pragma once

#include "engine_screen.h"
#include "engine_color.h"
#include "engine_math.h"
#include <inttypes.h>

inline void draw_clear_screen16(
	const screen_buffer* const screen,
	const uint16_t color)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	const int pixCount = screen->sizeInBytes >> 1;

	for (int i = 0; i < pixCount; i++)
	{
		pix[i] = color;
	}
}

inline void draw_clear_screen32(
	const screen_buffer* const screen,
	const uint32_t color)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	const int pixCount = screen->sizeInBytes >> 2;

	for (int i = 0; i < pixCount; i++)
	{
		pix[i] = color;
	}
}

inline void draw_clear_screen64(
	const screen_buffer* const screen,
	const uint64_t color)
{
	uint64_t* pix = (uint64_t*)screen->pixels;
	const int pixCount = screen->sizeInBytes >> 3;

	for (int i = 0; i < pixCount; i++)
	{
		pix[i] = color;
	}
}

extern void draw_filled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h
);

extern void draw_filled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h
);

extern void draw_filled_rect16_safe(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h
);

extern void draw_filled_rect32_safe(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h
);

extern void draw_unfilled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h
);

extern void draw_unfilled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h
);

extern void draw_line16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x1,
	int y1,
	int x2,
	int y2
);

extern void draw_line32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x1,
	int y1,
	int x2,
	int y2
);

extern void draw_grid32(
	const screen_buffer* const screen,
	const uint32_t color,
	int startX,
	int startY,
	int gridSize,
	int cols,
	int rows
);
