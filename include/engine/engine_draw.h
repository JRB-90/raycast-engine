#pragma once

#include "engine_screen.h"
#include "engine_color.h"

extern void draw_clear_screen16(
	const screen_buffer* const screen,
	const uint16_t color
);

extern void draw_clear_screen32(
	const screen_buffer* const screen,
	const uint32_t color
);

extern void draw_clear_screen64(
	const screen_buffer* const screen,
	const uint64_t color
);
