#pragma once

/*
    Provides a convience wrapped object (rayengine) to store all the data
    associated with an engine and provides helper functions for running
    it in continous render mode.
*/

#include <stdbool.h>
#include "engine_color.h"

typedef struct {
    bool quit;
    bool forwards;
    bool backwards;
    bool rotRight;
    bool rotLeft;
    bool right;
    bool left;
    bool toggleDebug;
    bool toggleRenderMode;
} input_state;

typedef struct {
    void* pixels;
    int width;
    int height;
    int sizeInPixels;
    int stride;
    int sizeInBytes;
    int bitsPP;
    int bytesPP;
    colformat colorFormat;
} screen_buffer;

typedef struct {
    int width;
    int height;
    colformat format;
} screen_format;

typedef enum {
    ENGINE_GRID,
    // TODO - As different types of engine are developed,
    //        add in here (grid, 2.5d, 3d etc)
} engine_type;

typedef struct {
    engine_type type;
    screen_format format;
    int targetFps;
} engine_config;

typedef struct {
    engine_config config;
    screen_buffer screen;
    input_state input;
    int (*on_update)(const input_state* const, const float);
    int (*on_render)(screen_buffer* const screen);
} rayengine;

/// <summary>
/// Returns a blank input state structure.
/// </summary>
extern input_state engine_get_default_input();

/// <summary>
/// Returns a blank screen structure.
/// </summary>
extern screen_buffer engine_get_default_screen();

/// <summary>
/// Creates a new engine instance with the supplied configuration.
/// </summary>
/// <param name="config">Configuration of the engine to create.</param>
/// <returns>Instantiated engine object. NULL if error occured.</returns>
extern rayengine * engine_create_new_rayengine(const engine_config *const config);

/// <summary>
/// Destroys the engine.
/// </summary>
/// <param name="engine">Engine to destroy.</param>
extern void engine_destroy_rayengine(rayengine *engine);

/// <summary>
/// Runs the engine in continous render mode, using the engines configuration
/// until an error occurs or the quit input is raised.
/// </summary>
/// <param name="engine">Engine to run in continous render rmode.</param>
/// <returns>Non-zero if an error occured.</returns>
extern int engine_run_rayengine(rayengine* const engine);
