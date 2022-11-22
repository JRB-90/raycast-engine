#pragma once

#include <stdbool.h>
#include "engine_screen.h"

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

typedef enum {
    ENGINE_GRID,
    // TODO - As different types of engine are developed,
    //        add in here (grid, 2.5d, 3d etc)
} engine_type;

typedef struct {
    engine_type type;
    screen_format format;
} engine_config;

typedef struct {
    engine_config config;
    screen_buffer screen;
} rayengine;

input_state blank_input_state();
rayengine *init_engine(const engine_config const *config);
void destroy_engine(rayengine *engine);