#include "gridengine_scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
