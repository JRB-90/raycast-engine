#pragma once

#include "engine_screen.h"
#include "engine_color.h"

inline void draw_clear_screen16_inline(
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

inline void draw_clear_screen32_inline(
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

inline void draw_clear_screen64_inline(
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