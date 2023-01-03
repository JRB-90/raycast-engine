#include "gridengine/gridengine_render.h"

#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>

const float WALL_SHADOW = 0.5f;

const static vec2d WORLD_FWD =
{
    .x = 0.0f,
    .y = 1.0f
};

int render_tile(
    const rayengine* const engine,
    const map_pos* const mapPosition,
    const int col,
    const int row,
    const color* const color
);

int render_grid_scene(
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

    return 0;
}

int render_tile(
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

    return 0;
}

int render_grid_player(
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

    frame2d playerPosLeft =
    {
        .x = player->position.x,
        .y = player->position.y,
        .theta = player->position.theta - (player->fov / 2.0f)
    };

    vec2d rayLeft =
        calc_forwards(
            &playerPosLeft,
            &WORLD_FWD
        );

    rayLeft = mul_vec(&rayLeft, 1000.0f);

    frame2d playerPosRight =
    {
        .x = player->position.x,
        .y = player->position.y,
        .theta = player->position.theta + (player->fov / 2.0f)
    };

    vec2d rayRight =
        calc_forwards(
            &playerPosRight,
            &WORLD_FWD
        );

    rayRight = mul_vec(&rayRight, 1000.0f);

    draw_line32_safe(
        &engine->screen,
        0xFFFFFFFF,
        posX,
        posY,
        posX + rayLeft.x,
        posY + rayLeft.y
    );

    draw_line32_safe(
        &engine->screen,
        0xFFFFFFFF,
        posX,
        posY,
        posX + rayRight.x,
        posY + rayRight.y
    );

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

    return 0;
}

traverse_result results[2048]; // Note, this will break if screen width is > 2048

int render_grid_rays(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    const player_obj* const player)
{
    reset_draw_state(&scene->drawState);

    int steps = engine->screen.width;
    float step = player->fov / (float)steps;

    frame2d playerPos =
    {
        .x = player->position.x,
        .y = player->position.y,
        .theta = player->position.theta - (player->fov / 2.0f)
    };
    
    for (int i = 0; i < steps; i++)
    {
        traverse_result result;
        float angle = scene->player.position.theta - playerPos.theta;

        int err =
            project_grid_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                angle,
                &result
            );

        if (err == 0)
        {
            results[i] = result;
        }
        else
        {
            results[i] = (traverse_result)
            {
                .intersectedObject = NULL,
                .intersectPoint = (vec2d) { 0.0f, 0.0f },
                .wallDistance = -1.0f,
                .side = 0,
            };
        }

        playerPos.theta += step;
    }

    for (int j = 0; j < SCENE_HEIGHT; j++)
    {
        for (int i = 0; i < SCENE_WIDTH; i++)
        {
            if (scene->drawState.visibleTiles[i][j])
            {
                color visColor;

                if (scene->world.grid[i][j].type == GRID_WALL)
                {
                    visColor =
                        to_col(
                            255,
                            (0.5 * 128) + (0.5 * scene->colors.wallCol.r),
                            (0.5 * 128) + (0.5 * scene->colors.wallCol.g),
                            (0.5 * 128) + (0.5 * scene->colors.wallCol.b)
                        );
                }
                else if (scene->world.grid[i][j].type == GRID_PSPAWN)
                {
                    visColor =
                        to_col(
                            255,
                            (0.5 * 128) + (0.5 * scene->colors.pSpawnCol.r),
                            (0.5 * 128) + (0.5 * scene->colors.pSpawnCol.g),
                            (0.5 * 128) + (0.5 * scene->colors.pSpawnCol.b)
                        );
                }
                else
                {
                    visColor =
                        to_col(
                            255,
                            (0.5 * 128) + (0.5 * scene->colors.floorCol.r),
                            (0.5 * 128) + (0.5 * scene->colors.floorCol.g),
                            (0.5 * 128) + (0.5 * scene->colors.floorCol.b)
                        );
                }

                render_tile(
                    engine,
                    mapPosition,
                    i,
                    j,
                    &visColor
                );
            }
        }
    }

    for (int k = 0; k < engine->screen.width; k++)
    {
        if (results[k].intersectedObject != NULL &&
            results[k].wallDistance >= 0.0f)
        {
            draw_filled_rect32_safe(
                &engine->screen,
                to_argb(&scene->colors.intersectCol),
                (mapPosition->x + (results[k].intersectPoint.x * mapPosition->scale)),
                (mapPosition->y + (results[k].intersectPoint.y * mapPosition->scale)),
                2,
                2
            );
        }
    }

    return 0;
}

int render_grid_verts(
    const rayengine* const engine,
    const grid_scene* const scene)
{
    // Ensure we reset the draw state tracker before we render a new frame
    reset_draw_state(&scene->drawState);

    // Find the steps to take in the ray sweep based on the screen width,
    // one ray per screen pixel column
    int steps = engine->screen.width;
    float step = scene->player.fov / (float)steps;

    frame2d playerPos =
    {
        .x = scene->player.position.x,
        .y = scene->player.position.y,
        .theta = scene->player.position.theta - (scene->player.fov / 2.0f)
    };

    // Loop through every ray angle
    for (int i = 0; i < steps; i++)
    {
        traverse_result result;
        float alpha = scene->player.position.theta - playerPos.theta;

        // Find object ray collides with first, if any
        int err =
            project_grid_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                alpha,
                &result
            );

        if (result.wallDistance >= 0.0f)
        {
            scene->drawState.wallDistances[i] = result.wallDistance;
        }
        else
        {
            scene->drawState.wallDistances[i] = FLT_MAX;
        }

        // If the ray has hit a wall, render the pixel column with texturing
        if (err == 0 &&
            result.intersectedObject != NULL &&
            result.wallDistance > 0)
        {
            if (engine->screen.colorFormat == CF_RGB565)
            {
                render_vertical_strip16(
                    engine,
                    scene,
                    result.intersectedObject,
                    &result.intersectPoint,
                    i,
                    result.wallDistance,
                    result.side
                );
            }
            else if (engine->screen.colorFormat == CF_ARGB)
            {
                render_vertical_strip32(
                    engine,
                    scene,
                    result.intersectedObject,
                    &result.intersectPoint,
                    i,
                    result.wallDistance,
                    result.side
                );
            }
        }

        playerPos.theta += step;
    }

    return 0;
}

int render_vertical_strip16(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side)
{
    // Find size of wall based on distance to it,
    // using TOA to find triangle side
    float h = tanf(scene->player.fov) * wallDistance;
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

    uint16_t* texturePixels = (uint16_t*)texture->texture.pixels;

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

    uint16_t* screenPixels = (uint16_t*)engine->screen.pixels;
    int sPixelIndex = (engine->screen.width * startY) + colIndex;

    // Run through every pixel in the strip and render the color
    // from the texture
    for (int j = startY; j < endY; j++)
    {
        tPixelIndex = ((int)textureV * texture->texture.width) + textureU;
        uint16_t wallColor = texturePixels[tPixelIndex];
        textureV += textureStep;

        if (engine->screen.colorFormat == CF_ARGB)
        {
            screenPixels[sPixelIndex] = wallColor;
            sPixelIndex += engine->screen.width;
        }
        else
        {
            screenPixels[sPixelIndex] = wallColor;
            sPixelIndex += engine->screen.width;
        }
    }

    return 0;
}

int render_vertical_strip32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const grid_object* const intersectObject,
    const vec2d* const intersectPoint,
    int colIndex,
    float wallDistance,
    int side)
{
    // Find size of wall based on distance to it,
    // using TOA to find triangle side
    float h = tanf(scene->player.fov) * wallDistance;
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
    float integral;
    int textureU;
    if (side == 0)
    {
        textureU =
            (int)
            (modff(intersectPoint->y, &integral) * 
            (float)texture->texture.width);
    }
    else
    {
        textureU = 
            (int)
            (modff(intersectPoint->x, &integral) * 
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

        if (engine->screen.colorFormat == CF_ARGB)
        {
            screenPixels[sPixelIndex] = wallColor;
            sPixelIndex += engine->screen.width;
        }
        else
        {
            screenPixels[sPixelIndex] = wallColor;
            sPixelIndex += engine->screen.width;
        }
    }

    return 0;
}

int render_static_sprites32(
    const rayengine* const engine,
    const grid_scene* const scene)
{
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        sprite_obj* sprite = &scene->world.sprites[i];

        if (sprite->spriteID < 0)
        {
            continue;
        }

        bool isInVisibleTile =
            scene->drawState.visibleTiles[(int)sprite->position.x][(int)sprite->position.y];

        if (isInVisibleTile == false)
        {
            continue;
        }

        vec2d playerPos =
        {
            scene->player.position.x,
            scene->player.position.y
        };

        vec2d forwards = calc_forwards(&scene->player.position, &WORLD_FWD);
        vec2d vecToSprite = sub_vec(&sprite->position, &playerPos);
        vec2d spriteDir = norm_vec(&vecToSprite);

        float angle = angle_between_vecs(&forwards, &spriteDir);
        if (angle > M_PI / 2.0)
        {
            continue;
        }

        float distanceToSprite = len_vec(&vecToSprite) * cosf(angle);
        float h = tanf(scene->player.fov) * distanceToSprite;
        int spriteHeightPixels = (int)(sprite->spriteHeight / h);

        float distToSpritePlane = 
            (engine->screen.width >> 1) / 
            tanf(scene->player.fov / 2.0f);
        float xDisplacement = tanf(angle) * distToSpritePlane;
        int spriteX = (engine->screen.width >> 1) + xDisplacement;

        int renderResult =
            render_sprite32(
                engine,
                scene,
                sprite,
                spriteX,
                spriteHeightPixels,
                distanceToSprite
            );

        if (renderResult)
        {
            return -1;
        }
    }

    return 0;
}

int render_sprite32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const sprite_obj* const sprite,
    int x,
    int size,
    float distanceToSprite)
{
    texture_resource* texture = scene->resources.textures[sprite->textureID];

    if (texture == NULL)
    {
        return -1;
    }

    int halfHeight = size >> 1;
    int screenWidth = engine->screen.width;
    int screenHeight = engine->screen.height;
    int textureWidth = texture->texture.width;
    int textureHeight = texture->texture.height;

    int screenX = x - halfHeight;
    int screenY = (screenHeight >> 1) - halfHeight;
    int screenPixelIndex = (screenY * screenWidth) + screenX;
    int screenYOffset = screenWidth - size;

    float texturePixelIndex = 0.0f;
    float textureHorInc = (float)textureWidth / size;
    float textureVerInc = (float)textureHeight / size;
    float u = 0.0f;
    float v = 0.0f;

    uint32_t* screenPixels = (uint32_t*)engine->screen.pixels;
    uint32_t* texturePixels = (uint32_t*)texture->texture.pixels;

    for (int j = screenY; j < screenY + size; j++)
    {
        if (j < 0)
        {
            screenPixelIndex += screenWidth;
            u = 0.0f;
            v += textureVerInc;
            continue;
        }

        if (j >= screenHeight)
        {
            return 0;
        }

        for (int i = screenX; i < screenX + size; i++)
        {
            if (i >= 0 &&
                i < screenWidth)
            {
                float wallDist = scene->drawState.wallDistances[i];
                if (wallDist > distanceToSprite)
                {
                    texturePixelIndex = ((int)v * textureWidth) + (int)u;
                    uint32_t color = texturePixels[(int)texturePixelIndex];

                    if (color != 0xFFFF00FF)
                    {
                        screenPixels[screenPixelIndex] = color;
                    }
                }
            }

            screenPixelIndex++;
            u += textureHorInc;
        }

        screenPixelIndex += screenYOffset;
        u = 0.0f;
        v += textureVerInc;
    }

    return 0;
}
