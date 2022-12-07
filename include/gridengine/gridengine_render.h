#pragma once

#include "gridengine_scene.h"
#include "engine_logic.h"
#include <stdbool.h>

void render_grid_scene(
    rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    bool drawGrid
);