#pragma once

#include "gridengine/gridengine_scene.h"
#include "engine/engine_rayengine.h"
#include <stdbool.h>

extern int render_grid_scene(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    bool drawGrid
);

extern int render_grid_player(
    const rayengine* const engine,
    const map_pos* const mapPosition,
    const player_obj* const player
);

extern int render_grid_sprites(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition
);

extern int render_grid_rays(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    const player_obj* const player
);

extern int render_grid_verts(
    const rayengine* const engine,
    const grid_scene* const scene
);

extern int render_vertical_strip16(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side
);

extern int render_vertical_strip32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side
);

extern int render_sprites32(
    const rayengine* const engine,
    const grid_scene* const scene
);

extern int render_sprite32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const sprite_obj* const sprite,
    int x,
    int height,
    float distanceToSprite
);
