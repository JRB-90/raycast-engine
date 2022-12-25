#pragma once

#include "gridengine_scene.h"
#include "engine_rayengine.h"
#include <stdbool.h>

extern void render_grid_scene(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    bool drawGrid
);

extern void render_grid_player(
    const rayengine* const engine,
    const map_pos* const mapPosition,
    const player_obj* const player
);

extern void render_grid_rays(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    const player_obj* const player
);

extern void render_grid_verts(
    const rayengine* const engine,
    const grid_scene* const scene
);

extern void render_vertical_strip(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side
);
