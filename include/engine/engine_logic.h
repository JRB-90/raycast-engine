#pragma once

#include <stdbool.h>

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

input_state blank_input_state();