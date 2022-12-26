#include "engine_draw.h"

void draw_filled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	int pixelIndex = (screen->width * y) + x;
	int offset = screen->width - w;

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex++;
		}

		pixelIndex += offset;
	}
}

void draw_filled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int pixelIndex = (screen->width * y) + x;
	int offset = screen->width - w;

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex++;
		}

		pixelIndex += offset;
	}
}

void draw_filled_rect16_safe(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	int startX = x >= 0 ? x : 0;
	int endX = (x + w) < screen->width ? (x + w) : screen->width;
	int newW = endX - startX;
	int startY = y >= 0 ? y : 0;
	int endY = (y + h) < screen->height ? (y + h) : screen->height;
	int newH = endY - startY;

	int pixelIndex = (screen->width * startY) + startX;
	int offset = screen->width - newW;

	for (int j = 0; j < newH; j++)
	{
		for (int i = 0; i < newW; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex++;
		}

		pixelIndex += offset;
	}
}

void draw_filled_rect32_safe(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int startX = x >= 0 ? x : 0;
	int endX = (x + w) < screen->width ? (x + w) : screen->width;
	int newW = endX - startX;
	int startY = y >= 0 ? y : 0;
	int endY = (y + h) < screen->height ? (y + h) : screen->height;
	int newH = endY - startY;

	int pixelIndex = (screen->width * startY) + startX;
	int offset = screen->width - newW;

	for (int j = 0; j < newH; j++)
	{
		for (int i = 0; i < newW; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex++;
		}

		pixelIndex += offset;
	}
}

void draw_unfilled_rect16(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	int pixelIndex = (screen->width * y) + x;
	int offset = h * screen->width;

	for (int i = x; i < x + w; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + offset] = color;
		pixelIndex++;
	}

	pixelIndex = (screen->width * y) + x;

	for (int i = y; i < y + h; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + w] = color;
		pixelIndex += screen->width;
	}
}

void draw_unfilled_rect32(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int pixelIndex = (screen->width * y) + x;
	int offset = h * screen->width;

	for (int i = x; i < x + w; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + offset] = color;
		pixelIndex++;
	}

	pixelIndex = (screen->width * y) + x;

	for (int i = y; i < y + h; i++)
	{
		pix[pixelIndex] = color;
		pix[pixelIndex + w] = color;
		pixelIndex += screen->width;
	}
}

void draw_unfilled_rect16_safe(
	const screen_buffer* const screen,
	const uint16_t color,
	int x, int y,
	int w, int h)
{
	uint16_t* pix = (uint16_t*)screen->pixels;
	int startX = x >= 0 ? x : 0;
	int endX = (x + w) < screen->width ? (x + w) : screen->width;
	int newW = endX - startX;
	int startY = y >= 0 ? y : 0;
	int endY = (y + h) < screen->height ? (y + h) : screen->height;
	int newH = endY - startY;
	int pixelIndex = (screen->width * startY) + startX;
	int offset = newH * screen->width;

	if (startY != y &&
		endX != y + h)
	{
		// Skip drawing horizontals in this case
	}
	else if (startY != y)
	{
		for (int i = startX; i < endX; i++)
		{
			pix[pixelIndex + offset] = color;
			pixelIndex++;
		}
	}
	else if (endY != y + h)
	{
		for (int i = startX; i < endX; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex++;
		}
	}
	else
	{
		for (int i = startX; i < endX; i++)
		{
			pix[pixelIndex] = color;
			pix[pixelIndex + offset] = color;
			pixelIndex++;
		}
	}

	pixelIndex = (screen->width * startY) + startX;

	if (startX != x &&
		endX != x + w)
	{
		// Skip drawing verticals in this case
		return;
	}
	else if (startX != x)
	{
		for (int i = startY; i < endY; i++)
		{
			pix[pixelIndex + newW] = color;
			pixelIndex += screen->width;
		}
	}
	else if (endX != x + w)
	{
		for (int i = startY; i < endY; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex += screen->width;
		}
	}
	else
	{
		for (int i = startY; i < endY; i++)
		{
			pix[pixelIndex] = color;
			pix[pixelIndex + newW] = color;
			pixelIndex += screen->width;
		}
	}
}

void draw_unfilled_rect32_safe(
	const screen_buffer* const screen,
	const uint32_t color,
	int x, int y,
	int w, int h)
{
	uint32_t* pix = (uint32_t*)screen->pixels;
	int startX = x >= 0 ? x : 0;
	int endX = (x + w) < screen->width ? (x + w) : screen->width;
	int newW = endX - startX;
	int startY = y >= 0 ? y : 0;
	int endY = (y + h) < screen->height ? (y + h) : screen->height;
	int newH = endY - startY;
	int pixelIndex = (screen->width * startY) + startX;
	int offset = newH * screen->width;

	if (startY != y &&
		endX != y + h)
	{
		// Skip drawing horizontals in this case
	}
	else if (startY != y)
	{
		for (int i = startX; i < endX; i++)
		{
			pix[pixelIndex + offset] = color;
			pixelIndex++;
		}
	}
	else if (endY != y + h)
	{
		for (int i = startX; i < endX; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex++;
		}
	}
	else
	{
		for (int i = startX; i < endX; i++)
		{
			pix[pixelIndex] = color;
			pix[pixelIndex + offset] = color;
			pixelIndex++;
		}
	}

	pixelIndex = (screen->width * startY) + startX;

	if (startX != x &&
		endX != x + w)
	{
		// Skip drawing verticals in this case
		return;
	}
	else if (startX != x)
	{
		for (int i = startY; i < endY; i++)
		{
			pix[pixelIndex + newW] = color;
			pixelIndex += screen->width;
		}
	}
	else if (endX != x + w)
	{
		for (int i = startY; i < endY; i++)
		{
			pix[pixelIndex] = color;
			pixelIndex += screen->width;
		}
	}
	else
	{
		for (int i = startY; i < endY; i++)
		{
			pix[pixelIndex] = color;
			pix[pixelIndex + newW] = color;
			pixelIndex += screen->width;
		}
	}
}

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
	int incX = get_sign(dx);
	int incY = get_sign(dy);
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
	int incX = get_sign(dx);
	int incY = get_sign(dy);
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

void draw_line16_safe(
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
	int incX = get_sign(dx);
	int incY = get_sign(dy);
	dx = abs(dx);
	dy = abs(dy);

	if (dy == 0)
	{
		// Horizontal line
		for (int x = x1; x != x2 + incX; x += incX)
		{
			if (x >= 0 &&
				x < screen->width)
			{
				pix[pixelIndex] = color;
			}
			pixelIndex += incX;
		}
	}
	else if (dx == 0)
	{
		// Vertical line
		for (int y = y1; y != y2 + incY; y += incY)
		{
			if (y >= 0 &&
				y < screen->height)
			{
				pix[pixelIndex] = color;
			}
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
			if (x >= 0 &&
				y >= 0 &&
				x < screen->width &&
				y < screen->height)
			{
				pix[pixelIndex] = color;
			}

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
			if (x >= 0 &&
				y >= 0 &&
				x < screen->width &&
				y < screen->height)
			{
				pix[pixelIndex] = color;
			}

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

void draw_line32_safe(
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
	int incX = get_sign(dx);
	int incY = get_sign(dy);
	dx = abs(dx);
	dy = abs(dy);

	if (dy == 0)
	{
		// Horizontal line
		for (int x = x1; x != x2 + incX; x += incX)
		{
			if (x >= 0 &&
				y1 >= 0 &&
				x < screen->width &&
				y1 < screen->height)
			{
				pix[pixelIndex] = color;
			}
			pixelIndex += incX;
		}
	}
	else if (dx == 0)
	{
		// Vertical line
		for (int y = y1; y != y2 + incY; y += incY)
		{
			if (x1 >= 0 && 
				y >= 0 &&
				x1 < screen->width &&
				y < screen->height)
			{
				pix[pixelIndex] = color;
			}
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
			if (x >= 0 &&
				y >= 0 &&
				x < screen->width &&
				y < screen->height)
			{
				pix[pixelIndex] = color;
			}

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
			if (x >= 0 &&
				y >= 0 &&
				x < screen->width &&
				y < screen->height)
			{
				pix[pixelIndex] = color;
			}

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
