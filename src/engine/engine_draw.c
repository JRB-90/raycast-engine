#include "engine_draw.h"

#include <inttypes.h>

void draw_clear_screen_16(
	const screen_buffer* const screen,
	const col_rgb565* const color)
{
	uint16_t *pix = (uint16_t*)screen->pixels;
}

void draw_clear_screen_32(
	const screen_buffer* const screen, 
	const col_rgb565* const color)
{

}

void draw_clear_screen_64(
	const screen_buffer* const screen, 
	const col_rgb565* const color)
{

}
