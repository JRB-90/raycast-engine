#include "gridengine/gridengine_render.h"

#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include <math.h>
#include <stdio.h>

const static vec2d WORLD_FWD =
{
    .x = 0.0f,
    .y = 1.0f
};

void render_tile(
    const rayengine* const engine,
    const map_pos* const mapPosition,
    const int col,
    const int row,
    const color* const color
);

void render_grid_scene(
    const rayengine* const engine,
    const grid_scene *const scene,
    const map_pos* const mapPosition,
    bool drawGrid)
{
    draw_clear_screen32(
        &engine->screen, 
        to_argb(&scene->colors.floorCol)
    );

    for (int j = 0; j < SCENE_HEIGHT; j++)
    {
        for (int i = 0; i < SCENE_WIDTH; i++)
        {
            if (scene->world.grid[i][j].type == GRID_WALL)
            {
                render_tile(
                    engine,
                    mapPosition,
                    i,
                    j,
                    &scene->colors.wallCol
                );
            }
            else if (scene->world.grid[i][j].type == GRID_PSPAWN)
            {
                render_tile(
                    engine,
                    mapPosition,
                    i,
                    j,
                    &scene->colors.pSpawnCol
                );
            }
        }
    }

    if (drawGrid)
    {
        draw_grid32(
            &engine->screen,
            0xFF565656,
            mapPosition->x,
            mapPosition->y,
            mapPosition->scale,
            SCENE_WIDTH,
            SCENE_HEIGHT
        );
    }
}

void render_tile(
    const rayengine *const engine,
    const map_pos *const mapPosition,
    const int col,
    const int row,
    const color *const color)
{
    int posX = mapPosition->x + (col * mapPosition->scale);
    int posY = mapPosition->y + (row * mapPosition->scale);

    draw_filled_rect32_safe(
        &engine->screen,
        to_argb(color),
        posX,
        posY,
        mapPosition->scale,
        mapPosition->scale
    );
}

void render_grid_player(
    const rayengine* const engine,
    const map_pos* const mapPosition,
    const player_obj* const player)
{
    int posX = mapPosition->x + (player->position.x * mapPosition->scale);
    int posY = mapPosition->y + (player->position.y * mapPosition->scale);
    int size = mapPosition->scale * 0.3;

    vec2d forward =
        calc_forwards(
            &player->position,
            &WORLD_FWD
        );

    vec2d arrow = mul_vec(&forward, mapPosition->scale * 0.7);

    draw_line32_safe(
        &engine->screen,
        to_argb(&player->playerCol),
        posX,
        posY,
        posX + arrow.x,
        posY + arrow.y
    );

    draw_filled_rect32_safe(
        &engine->screen,
        to_argb(&player->playerCol),
        posX - size + 2,
        posY - size + 2,
        (size * 2) - 3,
        (size * 2) - 3
    );
}

void render_grid_rays(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    const player_obj* const player)
{
    int steps = engine->screen.width;
    //int steps = 9;

    float fovRad = to_rad(player->fov);
    float step = fovRad / (float)steps;

    frame2d playerPos =
    {
        .x = player->position.x,
        .y = player->position.y,
        .theta = player->position.theta - (fovRad / 2.0f)
    };
    
    for (int i = 0; i < steps; i++)
    {
        float wallDistance = -1.0f;
        vec2d intersectPoint = { 0.0f, 0.0f };

        grid_object* intersectObject =
            project_grid_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                &intersectPoint,
                &wallDistance
            );

        if (intersectObject != NULL)
        {
            float startX = (mapPosition->x + (intersectPoint.x * mapPosition->scale));
            float startY = (mapPosition->y + (intersectPoint.y * mapPosition->scale));

            if (startX <= 0 ||
                startX >= engine->screen.width)
            {
                continue;
            }

            if (startY <= 0 ||
                startY >= engine->screen.height)
            {
                continue;
            }

            draw_filled_rect32_safe(
                &engine->screen,
                to_argb(&scene->colors.intersectCol),
                startX,
                startY,
                1,
                1
            );
        }

        playerPos.theta += step;
    }
}