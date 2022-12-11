#include "gridengine/gridengine_render.h"

#include "engine/engine_draw.h"

const static vec2d WORLD_FWD =
{
    .x = 0.0f,
    .y = -1.0f
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

    if (posX < 0 ||
        posX >= engine->screen.width - mapPosition->scale)
    {
        return;
    }

    if (posY < 0 ||
        posY >= engine->screen.height - mapPosition->scale)
    {
        return;
    }

    draw_filled_rect32(
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
    const player_obj* const player,
    const map_pos* const mapPosition)
{
    int posX = mapPosition->x + (player->position.x * mapPosition->scale);
    int posY = mapPosition->y + (player->position.y * mapPosition->scale);
    int size = mapPosition->scale * 0.3;

    vec2d forward =
        calc_forwards(
            &player->position,
            &WORLD_FWD
        );

    vec2d arrow = mul_vec(&forward, 12);

    draw_line32(
        &engine->screen,
        to_argb(&player->playerCol),
        posX,
        posY,
        posX + arrow.x,
        posY + arrow.y
    );

    draw_filled_rect32(
        &engine->screen,
        to_argb(&player->playerCol),
        posX - size + 1,
        posY - size + 1,
        size * 2,
        size * 2
    );
}