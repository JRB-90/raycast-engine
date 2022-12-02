#pragma once

#include "engine_screen.h"
#include "engine_color.h"

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

inline void draw_filled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y, 
	int w, int h)
{
	uint16_t* pix = (uint16_t*)screen->pixels;

	for (int j = y; j < y + h; j++)
	{
		for (int i = x; i < x + w; i++)
		{
			int pixelIndex = (screen->width * j) + i;
			pix[pixelIndex] = color;
		}
	}
}

inline void draw_filled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h)
{
	uint32_t* pix = (uint32_t*)screen->pixels;

	for (int j = y; j < y + h; j++)
	{
		for (int i = x; i < x + w; i++)
		{
			int pixelIndex = (screen->width * j) + i;
			pix[pixelIndex] = color;
		}
	}
}

inline void draw_unfilled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	int pixelIndex = (screen->width * x) + y;
	int offset = h * screen->width;

	for (int i = x; i < x + w; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + offset] = color;
		pixelIndex++;
	}

	pixelIndex = (screen->width * x) + y;

	for (int i = y; i < y + h; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + w] = color;
		pixelIndex += screen->width;
	}
}

inline void draw_unfilled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int pixelIndex = (screen->width * x) + y;
	int offset = h * screen->width;

	for (int i = x; i < x + w; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + offset] = color;
		pixelIndex++;
	}

	pixelIndex = (screen->width * x) + y;

	for (int i = y; i < y + h; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + w] = color;
		pixelIndex += screen->width;
	}
}
