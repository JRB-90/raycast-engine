#pragma once

#include "engine/engine_color.h"
#include "engine/engine_math.h"
#include "engine/engine_resource.h"

#define SCENE_WIDTH			64
#define SCENE_HEIGHT		64
#define MAX_STATIC_SPRITES	64

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
	grid_object_type type;
	int textureID;
} grid_object;

typedef struct {
	int spriteID;
	vec2d position;
	int textureID;
	float spriteHeight;
} static_sprite;

typedef struct {
	grid_object grid[SCENE_WIDTH][SCENE_HEIGHT];
	static_sprite staticSprites[MAX_STATIC_SPRITES];
	float wallHeight;
} world_grid;

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
	player_obj player;
	scene_resources resources;
	scene_cols colors;
} grid_scene;

typedef struct {
	int x;
	int y;
	int scale;
} map_pos;

extern grid_scene* create_scene(const char *const name);
extern void destroy_scene(grid_scene* scene);

extern int add_sprite(
	grid_scene* const scene,
	const vec2d position,
	int spriteID,
	int textureID,
	float spriteHeight
);

extern grid_object* project_grid_ray(
	const grid_scene* const scene,
	const frame2d* const playerPos,
	const vec2d* const worldForward,
	float alpha,
	vec2d* const intersectPoint,
	float* const wallDistance,
	int* const side
);
