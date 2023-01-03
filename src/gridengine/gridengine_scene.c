#include "gridengine/gridengine_scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

grid_scene* create_scene(
    const char *const name,
    int screenWidth)
{
	grid_scene* scene = malloc(sizeof(grid_scene));
	
	if (scene == NULL)
	{
		fprintf(stderr, "Failed to malloc grid_scene\n");
		return NULL;
	}

    scene->drawState.numberCols = screenWidth;
    scene->drawState.wallDistances = malloc(sizeof(float) * screenWidth);
    reset_draw_state(&scene->drawState);

    if (scene->drawState.wallDistances == NULL)
    {
        fprintf(stderr, "Failed to malloc draw state wall height array\n");
        return NULL;
    }

	memcpy(scene->name, name, sizeof(scene->name));

    scene->world.wallHeight = 1000.0f;

	scene->colors.floorCol = to_col(255, 32, 32, 32);
	scene->colors.ceilingCol = to_col(255, 52, 128, 128);
	scene->colors.wallCol = to_col(255, 32, 32, 56);
	scene->colors.pSpawnCol = to_col(255, 32, 128, 32);
	scene->colors.rayCol = to_col(255, 255, 0, 255);
	scene->colors.intersectCol = to_col(255, 0, 255, 0);

	scene->player.position = 
		to_frame2d(
			(SCENE_WIDTH / 2) + 0.5f, 
			(SCENE_HEIGHT / 2) + 0.5f, 
			to_rad(180.0f)
		);

    scene->player.fov = to_rad(45.0f);
    scene->player.playerCol = to_col(255, 255, 0, 0);

	for (int j = 0; j < SCENE_HEIGHT; j++)
	{
		for (int i = 0; i < SCENE_WIDTH; i++)
		{
			scene->world.grid[i][j].type = GRID_FLOOR;
			scene->world.grid[i][j].textureID = -1;
		}
	}

    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        scene->resources.textures[i] = NULL;
        scene->resources.texturesDark[i] = NULL;
        scene->resources.texturesLight[i] = NULL;
    }

    for (int i = 0; i < MAX_SPRITES; i++)
    {
        scene->world.sprites[i].spriteID = -1;
        scene->world.sprites[i].textureID = -1;
        scene->world.sprites[i].spriteHeight = -1.0f;
        scene->world.sprites[i].position = (vec2d)
        {
            .x = 0.0f,
            .y = 0.0f
        };
    }

	return scene;
}

void destroy_scene(grid_scene* scene)
{
    if (scene != NULL)
    {
        if (scene->drawState.wallDistances != NULL)
        {
            free(scene->drawState.wallDistances);
        }
        free(scene);
    }
}

void reset_draw_state(draw_state* const state)
{
    for (int j = 0; j < SCENE_HEIGHT; j++)
    {
        for (int i = 0; i < SCENE_WIDTH; i++)
        {
            state->visibleTiles[i][j] = false;
        }
    }

    for (int k = 0; k < state->numberCols; k++)
    {
        state->wallDistances[k] = -1.0f;
    }
}

int add_sprite(
    grid_scene* const scene,
    const vec2d position,
    int spriteID,
    int textureID,
    float spriteHeight)
{
    if (spriteID < 0 ||
        spriteID >= MAX_SPRITES)
    {
        fprintf(stderr, "Incorrect sprite ID\n");
        return -1;
    }

    scene->world.sprites[spriteID].spriteID = spriteID;
    scene->world.sprites[spriteID].textureID = textureID;
    scene->world.sprites[spriteID].position = position;
    scene->world.sprites[spriteID].spriteHeight = spriteHeight;

    return 0;
}

int project_grid_ray(
    grid_scene* const scene,
    const frame2d* const playerPos,
    const vec2d* const worldForward,
    float alpha,
    traverse_result* const result)
{
	result->intersectedObject = NULL;
    result->intersectPoint = (vec2d){ 0.0f, 0.0f };
	result->wallDistance = -1.0f;
    
	float posX = playerPos->x;
	float posY = playerPos->y;
	int gridX = (int)posX;
	int gridY = (int)posY;

	float sideDistX;
	float sideDistY;
	int xDir;
	int yDir;

	vec2d ray = calc_forwards(playerPos, worldForward);
	float deltaDistX = fabs(1.0 / ray.x);
	float deltaDistY = fabs(1.0 / ray.y);

    if (ray.x < 0)
    {
        xDir = -1;
        sideDistX = (posX - (float)gridX) * deltaDistX;
    }
    else
    {
        xDir = 1;
        sideDistX = ((float)gridX + 1.0 - posX) * deltaDistX;
    }

    if (ray.y < 0)
    {
        yDir = -1;
        sideDistY = (posY - (float)gridY) * deltaDistY;
    }
    else
    {
        yDir = 1;
        sideDistY = ((float)gridY + 1.0 - posY) * deltaDistY;
    }

    while (true)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            gridX += xDir;
            result->side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            gridY += yDir;
            result->side = 1;
        }

        if (gridX < 0 || gridX >= SCENE_WIDTH)
        {
            break;
        }

        if (gridY < 0 || gridY >= SCENE_HEIGHT)
        {
            break;
        }

        result->intersectedObject = &scene->world.grid[gridX][gridY];
        scene->drawState.visibleTiles[gridX][gridY] = true;

        if (result->intersectedObject->type == GRID_WALL)
        {
            if (result->side == 0)
            {
                result->wallDistance = sideDistX - deltaDistX;
            }
            else
            {
                result->wallDistance = sideDistY - deltaDistY;
            }

            break;
        }
    }

    if (result->wallDistance > 0.0f)
    {
        vec2d playerOrigin = to_vec2d(playerPos->x, playerPos->y);
        result->intersectPoint = mul_vec(&ray, result->wallDistance);
        result->intersectPoint = add_vec(&playerOrigin, &result->intersectPoint);

        // Correct for perspective
        float pCorrectDist = (result->wallDistance) * cosf(alpha);
        result->wallDistance = pCorrectDist;
    }

    return 0;
}
