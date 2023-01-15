#pragma once

#include "engine_screen.h"
#include "engine_rayengine.h"

/*
	Functions defined here need to have implementations defined
	for the specific subsystem backend required
*/

/// <summary>
/// Initialises the platform specific rendering subsytem.
/// </summary>
/// <param name="format">Desired format of the rendering surface.</param>
/// <param name="screen">Screen to initialse.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int engine_init_render_subsystem(
	const screen_format* const format,
	screen_buffer* const screen
);

/// <summary>
/// Destroys the provided screen and shuts down the rendering subsytem.
/// </summary>
/// <param name="screen">Screen to destroy.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int engine_destroy_render_subsystem(screen_buffer* const screen);

/// <summary>
/// Renders the screen buffer to the screen.
/// </summary>
/// <param name="screen">Screen buffer to render.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int engine_render_screen(screen_buffer* const screen);

/// <summary>
/// Initialises the platform specific input subsystem.
/// </summary>
/// <returns>Non-zero if error occurred</returns>
extern int engine_init_input_subsystem();

/// <summary>
/// Destroys and shuts down the input subsytem.
/// </summary>
/// <returns>Non-zero if error occurred</returns>
extern int engine_destroy_input_subsystem();

/// <summary>
/// Polls the input subsytem for the current input state.
/// </summary>
/// <param name="state">State object to fill out with the current input state.</param>
/// <returns>Non-zero if error occurred</returns>
extern int engine_update_input_state(input_state* const state);