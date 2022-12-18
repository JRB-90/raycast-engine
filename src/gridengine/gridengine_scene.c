#include "gridengine/gridengine_scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

grid_scene* create_scene(const char *const name)
{
	grid_scene* scene = malloc(sizeof(grid_scene));
	
	if (scene == NULL)
	{
		fprintf(stderr, "Failed to malloc grid_scene\n");
		return NULL;
	}

	memcpy(scene->name, name, sizeof(scene->name));

	scene->colors.floorCol = to_col(255, 56, 56, 56);
	scene->colors.ceilingCol = to_col(255, 32, 32, 128);
	scene->colors.wallCol = to_col(255, 32, 32, 56);
	scene->colors.pSpawnCol = to_col(255, 32, 128, 32);
	scene->colors.rayCol = to_col(255, 255, 0, 255);
	scene->colors.intersectCol = to_col(255, 0, 255, 0);

	scene->player.playerCol = to_col(255, 255, 0, 0);
	scene->player.position = 
		to_frame2d(
			(SCENE_WIDTH / 2) + 0.5f, 
			(SCENE_HEIGHT / 2) + 0.5f, 
			to_rad(180.0f)
		);

	for (int j = 0; j < SCENE_HEIGHT; j++)
	{
		for (int i = 0; i < SCENE_WIDTH; i++)
		{
			scene->world.grid[i][j].type = GRID_FLOOR;
			scene->world.grid[i][j].textureID = -1;
		}
	}

	return scene;
}

void destroy_scene(grid_scene* scene)
{
	free(scene);
}

grid_object* project_grid_ray(
    const grid_scene* const scene,
    const player_obj* const player,
    const vec2d* const worldForward,
    vec2d* const intersectPoint,
    float* const wallDistance)
{
	grid_object* intersectObject = NULL;
    *intersectPoint = (vec2d){ 0.0f, 0.0f };
	*wallDistance = -1.0f;
    
	float posX = player->position.x;
	float posY = player->position.y;
	int gridX = (int)posX;
	int gridY = (int)posY;

	float sideDistX;
	float sideDistY;
	int xDir;
	int yDir;
	int side;

	vec2d ray = calc_forwards(&player->position, worldForward);
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
            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            gridY += yDir;
            side = 1;
        }

        if (gridX < 0 || gridX > SCENE_WIDTH)
        {
            break;
        }

        if (gridY < 0 || gridY > SCENE_HEIGHT)
        {
            break;
        }

        intersectObject = &scene->world.grid[gridX][gridY];

        if (intersectObject->type == GRID_WALL)
        {
            if (side == 0)
            {
                *wallDistance = sideDistX - deltaDistX;
            }
            else
            {
                *wallDistance = sideDistY - deltaDistY;
            }

            break;
        }
    }

    if (*wallDistance > 0.0f)
    {
        vec2d playerPos = to_vec2d(player->position.x, player->position.y);
        *intersectPoint = mul_vec(&ray, *wallDistance);
        *intersectPoint = add_vec(&playerPos, intersectPoint);
    }
    else
    {
        return NULL;
    }

	return intersectObject;
}
