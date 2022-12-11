#pragma once

#include "gridengine_scene.h"
#include "engine_rayengine.h"
#include <stdbool.h>

void render_grid_scene(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    bool drawGrid
);

void render_grid_player(
    const rayengine* const engine,
    const player_obj* const player,
    const map_pos* const mapPosition
);