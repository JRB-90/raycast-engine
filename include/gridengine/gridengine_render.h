#pragma once

#include "gridengine_scene.h"
#include "engine_rayengine.h"
#include <stdbool.h>

typedef struct {
    bool visibleTiles[SCENE_WIDTH][SCENE_HEIGHT];
    int numberCols;
    int* wallHeights;
} draw_state;

extern draw_state* create_draw_state(int screenWidth);
extern void destroy_draw_state(draw_state* state);
extern void reset_draw_state(draw_state* const state);

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

extern void render_vertical_strip16(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side
);

extern void render_vertical_strip32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side
);

extern int render_static_sprites32(
    const rayengine* const engine,
    const grid_scene* const scene
);

extern int render_sprite32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const static_sprite* const sprite,
    int x,
    int height
);
