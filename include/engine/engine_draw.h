#pragma once

/*
	Collection of drawing routines that can be used to perform common drawing
	tasks such as clearing the screen and drawing primitives.
	Internally, each drawing routine contains different variants for both 32
	and 16 bit screen buffer colors.
*/

#include "engine_color.h"
#include "engine_math.h"
#include "engine_resource.h"
#include "engine_rayengine.h"
#include <inttypes.h>

/// <summary>
/// Clears the screen with the provided color.
/// </summary>
/// <param name="screen">Screen buffer to clear.</param>
/// <param name="color">Color to fill the screen buffer with.</param>
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

/// <summary>
/// Clears the screen with the provided color.
/// </summary>
/// <param name="screen">Screen buffer to clear.</param>
/// <param name="color">Color to fill the screen buffer with.</param>
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

/// <summary>
/// Draw the ceiling and floor into the screen buffer.
/// Celing covers the top half of the screen, the floor,
/// the bottom half.
/// </summary>
/// <param name="screen">Screen buffer to </param>
/// <param name="ceilingColor">Color to draw the ceiling.</param>
/// <param name="floorColor">Color to draw the floor.</param>
inline void draw_ceiling_floor16(
	const screen_buffer* const screen,
	const uint16_t ceilingColor,
	const uint16_t floorColor)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	const int pixCount = screen->sizeInBytes >> 1;
	const int halfPixCount = pixCount >> 1;

	for (int i = 0; i < halfPixCount; i++)
	{
		pix[i] = ceilingColor;
	}
	for (int i = halfPixCount; i < pixCount; i++)
	{
		pix[i] = floorColor;
	}
}

/// <summary>
/// Draw the ceiling and floor into the screen buffer.
/// Celing covers the top half of the screen, the floor,
/// the bottom half.
/// </summary>
/// <param name="screen">Screen buffer to </param>
/// <param name="ceilingColor">Color to draw the ceiling.</param>
/// <param name="floorColor">Color to draw the floor.</param>
inline void draw_ceiling_floor32(
	const screen_buffer* const screen,
	const uint32_t ceilingColor,
	const uint32_t floorColor)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	const int pixCount = screen->sizeInBytes >> 2;
	const int halfPixCount = pixCount >> 1;

	for (int i = 0; i < halfPixCount; i++)
	{
		pix[i] = ceilingColor;
	}
	for (int i = halfPixCount; i < pixCount; i++)
	{
		pix[i] = floorColor;
	}
}

/// <summary>
/// Draw a filled rectangle on the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the rectangle.</param>
/// <param name="x">Start X position.</param>
/// <param name="y">Start Y position.</param>
/// <param name="w">Rectangle width.</param>
/// <param name="h">Rectangle height.</param>
extern void draw_filled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h
);

/// <summary>
/// Draw a filled rectangle on the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the rectangle.</param>
/// <param name="x">Start X position.</param>
/// <param name="y">Start Y position.</param>
/// <param name="w">Rectangle width.</param>
/// <param name="h">Rectangle height.</param>
extern void draw_filled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h
);

/// <summary>
/// Draw an unfilled rectangle on the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the rectangle.</param>
/// <param name="x">Start X position.</param>
/// <param name="y">Start Y position.</param>
/// <param name="w">Rectangle width.</param>
/// <param name="h">Rectangle height.</param>
extern void draw_unfilled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h
);

/// <summary>
/// Draw an unfilled rectangle on the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the rectangle.</param>
/// <param name="x">Start X position.</param>
/// <param name="y">Start Y position.</param>
/// <param name="w">Rectangle width.</param>
/// <param name="h">Rectangle height.</param>
extern void draw_unfilled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h
);

/// <summary>
/// Draw a line on the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the line.</param>
/// <param name="x1">Start X position.</param>
/// <param name="y1">Start Y position.</param>
/// <param name="x2">End X position.</param>
/// <param name="y2">End Y position.</param>
extern void draw_line16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x1,
	int y1,
	int x2,
	int y2
);

/// <summary>
/// Draw a line on the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the line.</param>
/// <param name="x1">Start X position.</param>
/// <param name="y1">Start Y position.</param>
/// <param name="x2">End X position.</param>
/// <param name="y2">End Y position.</param>
extern void draw_line32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x1,
	int y1,
	int x2,
	int y2
);

/// <summary>
/// Draw a square grid onto the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the grid.</param>
/// <param name="startX">Starting X position of the grid.</param>
/// <param name="startY">Starting Y position of the grid.</param>
/// <param name="gridSize">Size of each grid cell.</param>
/// <param name="cols">Number of columns the grid has.</param>
/// <param name="rows">Number of rows the grid has.</param>
extern void draw_grid16(
	const screen_buffer* const screen,
	const uint16_t color,
	int startX,
	int startY,
	int gridSize,
	int cols,
	int rows
);

/// <summary>
/// Draw a square grid onto the screen.
/// </summary>
/// <param name="screen">Screen to draw onto.</param>
/// <param name="color">Color to draw the grid.</param>
/// <param name="startX">Starting X position of the grid.</param>
/// <param name="startY">Starting Y position of the grid.</param>
/// <param name="gridSize">Size of each grid cell.</param>
/// <param name="cols">Number of columns the grid has.</param>
/// <param name="rows">Number of rows the grid has.</param>
extern void draw_grid32(
	const screen_buffer* const screen,
	const uint32_t color,
	int startX,
	int startY,
	int gridSize,
	int cols,
	int rows
);
