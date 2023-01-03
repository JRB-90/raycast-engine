#pragma once

#include "engine/engine_color.h"
#include "engine/engine_math.h"
#include "engine/engine_resource.h"
#include <stdbool.h>

#define SCENE_WIDTH			64
#define SCENE_HEIGHT		64
#define MAX_SPRITES	64

typedef enum {
	GRID_FLOOR,
	GRID_WALL,
	GRID_PSPAWN
} grid_object_type;

typedef struct {
	frame2d position;
	float fov;
	color playerCol;
} player_obj;

typedef struct {
	int spriteID;
	vec2d position;
	int textureID;
	float spriteHeight;
} sprite_obj;

typedef struct {
	grid_object_type type;
	int textureID;
} grid_object;

typedef struct {
	grid_object grid[SCENE_WIDTH][SCENE_HEIGHT];
	sprite_obj sprites[MAX_SPRITES];
	float wallHeight;
} world_grid;

typedef struct {
	bool visibleTiles[SCENE_WIDTH][SCENE_HEIGHT];
	int numberCols;
	float* wallDistances;
} draw_state;

typedef struct {
	color floorCol;
	color ceilingCol;
	color wallCol;
	color pSpawnCol;
	color rayCol;
	color intersectCol;
} scene_cols;

typedef struct {
	char name[64];
	world_grid world;
	draw_state drawState;
	player_obj player;
	scene_resources resources;
	scene_cols colors;
} grid_scene;

typedef struct {
	int x;
	int y;
	int scale;
} map_pos;

typedef struct {
	grid_object* intersectedObject;
	vec2d intersectPoint;
	float wallDistance;
	int side;
} traverse_result;

extern grid_scene* create_scene(
	const char *const name,
	int screenWidth
);
extern void destroy_scene(grid_scene* scene);
extern void reset_draw_state(draw_state* const state);

extern int add_sprite(
	grid_scene* const scene,
	const vec2d position,
	int spriteID,
	int textureID,
	float spriteHeight
);

extern int project_grid_ray(
	grid_scene* const scene,
	const frame2d* const playerPos,
	const vec2d* const worldForward,
	float alpha,
	traverse_result* const result
);
