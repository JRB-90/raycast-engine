#pragma once

#include "gridengine_scene.h"
#include "engine/engine_rayengine.h"

extern grid_scene* create_test_scene1(
    const char* const name,
    const screen_format* const screenFormat
);

extern grid_scene* create_test_scene2(
    const char* const name,
    const screen_format* const screenFormat
);

extern void destroy_test_scene(grid_scene* scene);
