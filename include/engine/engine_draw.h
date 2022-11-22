#pragma once

#include "engine_screen.h"
#include "engine_color.h"

extern void draw_clear_screen_16(
	const screen_buffer* const screen,
	const col_rgb565* const color
);

extern void draw_clear_screen_32(
	const screen_buffer* const screen,
	const col_rgb565* const color
);

extern void draw_clear_screen_64(
	const screen_buffer* const screen,
	const col_rgb565* const color
);
