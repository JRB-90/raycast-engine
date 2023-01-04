#pragma once

#include "gridengine_scene.h"
#include "engine/engine_screen.h"

extern grid_scene* create_test_scene1(
    const char* const name,
    const screen_format* const screenFormat
);

extern void destroy_test_scene(grid_scene* scene);
extern void add_wall(grid_scene* const scene, int x, int y, int textureID);
extern int load_test_textures(
    grid_scene* const scene,
    colformat format
);
extern void destroy_test_textures(grid_scene* const scene);
extern int load_test_sprites(grid_scene* const scene);
