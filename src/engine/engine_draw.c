#include "engine_draw.h"

void draw_line16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x1,
	int y1,
	int x2,
	int y2)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	int pixelIndex = (screen->width * y1) + x1;

	int dx = x2 - x1;
	int dy = y2 - y1;
	int incX = Sign(dx);
	int incY = Sign(dy);
	dx = abs(dx);
	dy = abs(dy);

	if (dy == 0)
	{
		// Horizontal line
		for (int x = x1; x != x2 + incX; x += incX)
		{
			pix[pixelIndex] = color;
			pixelIndex += incX;
		}
	}
	else if (dx == 0)
	{
		// Vertical line
		for (int y = y1; y != y2 + incY; y += incY)
		{
			pix[pixelIndex] = color;
			pixelIndex += screen->width * incY;
		}
	}
	else if (dx >= dy)
	{
		// Horizontal bias line
		int slope = 2 * dy;
		int error = -dx;
		int errorInc = -2 * dx;
		int y = y1;

		for (int x = x1; x != x2 + incX; x += incX)
		{
			pix[pixelIndex] = color;

			error += slope;
			if (error >= 0)
			{
				y += incY;
				error += errorInc;
				pixelIndex += (screen->width * incY) + incX;
			}
			else
			{
				pixelIndex += incX;
			}
		}
	}
	else
	{
		// Vertical bias line
		int slope = 2 * dx;
		int error = -dy;
		int errorInc = -2 * dy;
		int x = x1;

		for (int y = y1; y != y2 + incY; y += incY)
		{
			pix[pixelIndex] = color;

			error += slope;
			if (error >= 0)
			{
				x += incX;
				error += errorInc;
				pixelIndex += (screen->width * incY) + incX;
			}
			else
			{
				pixelIndex += screen->width * incY;
			}
		}
	}
}

void draw_line32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x1,
	int y1,
	int x2,
	int y2)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int pixelIndex = (screen->width * y1) + x1;

	int dx = x2 - x1;
	int dy = y2 - y1;
	int incX = Sign(dx);
	int incY = Sign(dy);
	dx = abs(dx);
	dy = abs(dy);

	if (dy == 0)
	{
		// Horizontal line
		for (int x = x1; x != x2 + incX; x += incX)
		{
			pix[pixelIndex] = color;
			pixelIndex += incX;
		}
	}
	else if (dx == 0)
	{
		// Vertical line
		for (int y = y1; y != y2 + incY; y += incY)
		{
			pix[pixelIndex] = color;
			pixelIndex += screen->width * incY;
		}
	}
	else if (dx >= dy)
	{
		// Horizontal bias line
		int slope = 2 * dy;
		int error = -dx;
		int errorInc = -2 * dx;
		int y = y1;

		for (int x = x1; x != x2 + incX; x += incX)
		{
			pix[pixelIndex] = color;

			error += slope;
			if (error >= 0)
			{
				y += incY;
				error += errorInc;
				pixelIndex += (screen->width * incY) + incX;
			}
			else
			{
				pixelIndex += incX;
			}
		}
	}
	else
	{
		// Vertical bias line
		int slope = 2 * dx;
		int error = -dy;
		int errorInc = -2 * dy;
		int x = x1;

		for (int y = y1; y != y2 + incY; y += incY)
		{
			pix[pixelIndex] = color;

			error += slope;
			if (error >= 0)
			{
				x += incX;
				error += errorInc;
				pixelIndex += (screen->width * incY) + incX;
			}
			else
			{
				pixelIndex += screen->width * incY;
			}
		}
	}
}

void draw_grid32(
	const screen_buffer* const screen,
	const uint32_t color,
	int startX,
	int startY,
	int gridSize,
	int cols,
	int rows)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int endX = startX + (gridSize * cols);
	int endY = startY + (gridSize * rows);
	int pixelIndex = 0;
	int lineCount = 0;

	for (int j = startY; j <= endY; j++)
	{
		if (j >= 0 && j < screen->height)
		{
			pixelIndex = (screen->width * j) + startX;

			if (lineCount % gridSize == 0)
			{
				for (int i = startX; i <= endX; i++)
				{
					if (i >= 0 && i < screen->width)
					{
						pix[pixelIndex] = color;
					}
					pixelIndex++;
				}
			}
			else
			{
				for (int i = startX; i <= endX; i += gridSize)
				{
					if (i >= 0 && i < screen->width)
					{
						pix[pixelIndex] = color;
					}
					pixelIndex += gridSize;
				}
			}
		}

		lineCount++;
	}
}