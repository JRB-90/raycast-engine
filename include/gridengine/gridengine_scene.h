#pragma once

/*
	Scene implementation that uses a flat 2d grid to store wall and sprite
	data in. This is more limited, but allows for faster ray projection
	through the scene and thus, a better performing engine.
*/

#include "engine/engine_color.h"
#include "engine/engine_math.h"
#include "engine/engine_resource.h"
#include "engine/engine_rayengine.h"
#include "utils/utils_list.h"
#include <stdbool.h>

#define SCENE_WIDTH			64
#define SCENE_HEIGHT		64
#define MAX_SPRITES			64

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
	linked_list sprites;
	float wallHeight;
} world_grid;

typedef struct {
	sprite_obj* sprite;
	float distanceToSprite;
	float angle;
} vis_sprite;

typedef struct {
	bool visibleTiles[SCENE_WIDTH][SCENE_HEIGHT];
	int numberCols;
	float* wallDistances;
	vis_sprite visibleSprites[MAX_SPRITES];
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

/// <summary>
/// Creates a new instance of a grid scene.
/// </summary>
/// <param name="name">Name to give the scene.</param>
/// <param name="screenWidth">Width of the screen buffer.</param>
/// <returns>New grid scene instance. NULL if error occurred.</returns>
extern grid_scene* gridengine_create_new_scene(
	const char *const name,
	int screenWidth
);

/// <summary>
/// Destroys a grid scene.
/// </summary>
/// <param name="scene">Scene to destroy.</param>
extern void gridengine_destroy_scene(grid_scene* scene);

/// <summary>
/// Resets the provided draw state.
/// </summary>
/// <param name="state">Draw State strucure to reset.</param>
extern void gridengine_reset_draw_state(draw_state* const state);

/// <summary>
/// Creates a new sprite object and adds it to the scene.
/// </summary>
/// <param name="scene">Scene to add the sprite to.</param>
/// <param name="spriteID">Unique ID to assign the sprite.</param>
/// <param name="textureID">Texture associated with this sprite.</param>
/// <param name="position">Position of the sprite in the world.</param>
/// <param name="spriteHeight">Height of the sprite.</param>
/// <returns>New sprite object instance. NULL if an error occurred.</returns>
extern sprite_obj* gridengine_create_sprite(
	grid_scene* const scene,
	int spriteID,
	int textureID,
	const vec2d position,
	float spriteHeight
);

/// <summary>
/// Destroys a sprite object and removes it from the scene.
/// </summary>
/// <param name="scene">Scene that contains the sprite object.</param>
/// <param name="sprite">Sprite object to destroy.</param>
/// <returns>Non-zero if error occured.</returns>
extern int gridengine_destroy_sprite(
	grid_scene* const scene, 
	sprite_obj* sprite
);

/// <summary>
/// Moves the player within the scene, whilst maintaining collision avoidance.
/// Uses the supplied input_state as the driver.
/// </summary>
/// <param name="inputState">Input state to apply to the player.</param>
/// <param name="scene">Scene containing the player.</param>
/// <param name="worldForwards">Forwards vector of the world.</param>
/// <param name="worldLeft">Left vector of the world.</param>
/// <param name="deltaTimeMS">Time passed since last update in milliseconds.</param>
/// <returns></returns>
extern bool gridengine_move_player(
	const input_state* const inputState,
	grid_scene* const scene,
	const vec2d* const worldForwards,
	const vec2d* const worldLeft,
	const float deltaTimeMS
);

/// <summary>
/// Projects a ray through the grid scene and calculates the traverse result.
/// </summary>
/// <param name="scene">Scene to project the ray through.</param>
/// <param name="playerPos">The position of the player in the grid.</param>
/// <param name="worldForward">The forward vector of the world.</param>
/// <param name="alpha">The angle offset from forwards that the ray is broadcast.</param>
/// <param name="result">The calculated result of the projection.</param>
/// <returns>Non-zero if an error occurred.</returns>
extern int gridengine_project_ray(
	grid_scene* const scene,
	const frame2d* const playerPos,
	const vec2d* const worldForward,
	float alpha,
	traverse_result* const result
);
