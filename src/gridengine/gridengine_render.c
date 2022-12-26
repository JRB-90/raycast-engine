#include "gridengine/gridengine_render.h"

#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

const float WALL_SHADOW = 0.5f;

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
        float angle = scene->player.position.theta - playerPos.theta;
        float wallDistance = -1.0f;
        vec2d intersectPoint = { 0.0f, 0.0f };
        int side = 0;

        grid_object* intersectObject =
            project_grid_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                angle,
                &intersectPoint,
                &wallDistance,
                &side
            );

        if (intersectObject != NULL)
        {
            draw_filled_rect32_safe(
                &engine->screen,
                to_argb(&scene->colors.intersectCol),
                (mapPosition->x + (intersectPoint.x * mapPosition->scale)),
                (mapPosition->y + (intersectPoint.y * mapPosition->scale)),
                1,
                1
            );
        }

        playerPos.theta += step;
    }
}

void render_grid_verts(
    const rayengine* const engine,
    const grid_scene* const scene)
{
    // Find the steps to take in the ray sweep based on the screen width,
    // one ray per screen pixel column
    int steps = engine->screen.width;
    float fovRad = to_rad(scene->player.fov);
    float step = fovRad / (float)steps;

    frame2d playerPos =
    {
        .x = scene->player.position.x,
        .y = scene->player.position.y,
        .theta = scene->player.position.theta - (fovRad / 2.0f)
    };

    // Loop through every ray angle
    for (int i = 0; i < steps; i++)
    {
        float alpha = scene->player.position.theta - playerPos.theta;
        float wallDistance = -1.0f;
        vec2d intersectPoint = { 0.0f, 0.0f };
        int side = 0;

        // Find object ray collides with first, if any
        grid_object* intersectObject =
            project_grid_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                alpha,
                &intersectPoint,
                &wallDistance,
                &side
            );

        // If the ray has hit a wall, render the pixel column with texturing
        if (intersectObject != NULL &&
            wallDistance > 0)
        {
            render_vertical_strip(
                engine,
                scene,
                intersectObject,
                &intersectPoint,
                i,
                wallDistance,
                side
            );
        }

        playerPos.theta += step;
    }
}

void render_vertical_strip(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side)
{
    // Find height of wall based on distance to it,
    // using TOA to find triangle side
    float h = tanf(to_rad(scene->player.fov)) * wallDistance;
    int wallHeightPixels = scene->world.wallHeight / h;

    // Find the start and end of the walls in screen Y coords
    int wallYStart = (engine->screen.height >> 1) - (wallHeightPixels >> 1);
    int startY =
        clampi(
            wallYStart,
            0,
            engine->screen.height
        );

    int wallYEnd = startY + wallHeightPixels;
    int endY =
        clampi(
            wallYEnd,
            0,
            engine->screen.height
        );
    
    texture_resource* texture;

    if (side == 0)
    {
        texture = scene->resources.texturesDark[intersectObject->textureID];
    }
    else
    {
        texture = scene->resources.textures[intersectObject->textureID];
    }

    uint32_t* texturePixels = (uint32_t*)texture->texture.pixels;

    // Find the texture U coord information, based on where intersect on
    // the wall was and the side it hit
    double integral;
    int textureU;
    if (side == 0)
    {
        textureU =
            (int)
            (modf(intersectPoint->y, &integral) * 
            (float)texture->texture.width);
    }
    else
    {
        textureU = 
            (int)
            (modf(intersectPoint->x, &integral) * 
            (float)texture->texture.width);
    }

    // Find the texture V coord information:
    // Where it starts and stops, what step does it take per pixel, etc
    float textureStep =
        (float)texture->texture.height / (float)(wallHeightPixels);

    float textureV = 
        ((startY - wallYStart) * (float)texture->texture.height) / 
        (float)wallHeightPixels;

    int tPixelIndex = ((int)textureV * texture->texture.width) + textureU;

    uint32_t* screenPixels = (uint32_t*)engine->screen.pixels;
    int sPixelIndex = (engine->screen.width * startY) + colIndex;
    
    // Run through every pixel in the strip and render the color
    // from the texture
    for (int j = startY; j < endY; j++)
    {
        tPixelIndex = ((int)textureV * texture->texture.width) + textureU;
        uint32_t wallColor = texturePixels[tPixelIndex];
        textureV += textureStep;

        screenPixels[sPixelIndex] = wallColor;
        sPixelIndex += engine->screen.width;
    }
}
