#pragma once

#include "engine_color.h"

typedef enum {
	GRID_FLOOR,
	GRID_WALL
} grid_object_type;

typedef struct {
	grid_object_type type;
	int textureID;
} grid_object;

typedef struct {
	grid_object grid[64][64];
} world_grid;

typedef struct {
	char name[64];
	world_grid world;
	color floorCol;
	color ceilingCol;
} grid_scene;

grid_scene* create_scene(const char const* name);
void destroy_scene(grid_scene* scene);