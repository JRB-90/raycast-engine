#pragma once

#include "engine_color.h"

#define SCENE_WIDTH 64
#define SCENE_HEIGHT 64

typedef enum {
	GRID_FLOOR,
	GRID_WALL,
	GRID_PSPAWN
} grid_object_type;

typedef struct {
	grid_object_type type;
	int textureID;
} grid_object;

typedef struct {
	grid_object grid[SCENE_WIDTH][SCENE_HEIGHT];
} world_grid;

typedef struct {
	color floorCol;
	color ceilingCol;
	color wallCol;
	color pSpawnCol;
} scene_cols;

typedef struct {
	char name[64];
	world_grid world;
	scene_cols colors;
} grid_scene;

typedef struct {
	int x;
	int y;
	int scale;
} map_pos;

grid_scene* create_scene(const char *const name);
void destroy_scene(grid_scene* scene);