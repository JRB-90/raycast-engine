#pragma once

#include "engine_screen.h"
#include "engine_logic.h"

/*
	Functions defined here need to have implementations defined
	for the specific subsystem backend required
*/

extern int init_render_subsystem(
	const screen_format* const format,
	screen_buffer* const screen
);
extern int destroy_render_subsystem(screen_buffer* const screen);
extern int render_screen(screen_buffer* const screen);

extern int init_input_subsystem();
extern int destroy_input_subsystem();
extern int update_input_state(input_state* const state);