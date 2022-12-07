#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_logic.h"
#include "engine/engine_screen.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "gridengine/gridengine_scene.h"
#include "time/time_helper.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;
const int SSIZE = 5;
const int GRID_MIN_SIZE = 4;
const int GRID_MAX_SIZE = 20;

typedef struct {
    int x;
    int y;
    int scale;
} map_pos;

color wallColor;
color pspawnColor;

rayengine* engine;
grid_scene* scene;
map_pos mapPosition;

bool drawGrid = true;
bool shouldRender = true;

void sig_handler(int signum);
void cleanup(int status);
void build_test_scene();
void move_map();
void render_grid_scene();
void render_tile(
    const int row,
    const int col,
    const color const* color
);

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    signal(SIGINT, sig_handler);

    wallColor = to_col(255, 32, 32, 56);
    pspawnColor = to_col(255, 32, 128, 32);

    mapPosition = (map_pos)
    {
        .x = (-SSIZE * (SCENE_WIDTH / 2)) + (SWIDTH / 2),
        .y = (-SSIZE * (SCENE_HEIGHT / 2)) + (SHEIGHT / 2),
        .scale = SSIZE,
    };

    scene = create_scene("Test Grid Scene");
    build_test_scene();

    engine_config config =
    {
        .type = ENGINE_GRID,
        .format = (screen_format)
        {
            .format = SFORMAT,
            .width = SWIDTH,
            .height = SHEIGHT
        }
    };

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
    }

    render_grid_scene();

    input_state input = blank_input_state();

    while (!engine->input.quit)
    {
        update_engine(engine);
        move_map();
        
        if (shouldRender)
        {
            clktimer timer;
            start_timer(&timer);
            render_grid_scene();
            deltatime delta = elapsed_millis(&timer);
            printf("Map render took %.3fms\n", delta);
        }

        shouldRender = false;
        sleep_millis(1);
    }

    cleanup(EXIT_SUCCESS);
}

void sig_handler(int signum)
{
    if (signum == SIGTERM ||
        signum == SIGABRT ||
        signum == SIGINT)
    {
        cleanup(EXIT_SUCCESS);
    }
}

void cleanup(int status)
{
    if (engine != NULL)
    {
        destroy_engine(engine);
    }

    if (scene != NULL)
    {
        destroy_scene(scene);
    }

    exit(status);
}

void build_test_scene()
{
    scene->world.grid[32][32].type = GRID_PSPAWN;

    scene->world.grid[30][28].type = GRID_WALL;
    scene->world.grid[31][28].type = GRID_WALL;
    scene->world.grid[32][28].type = GRID_WALL;
    scene->world.grid[33][28].type = GRID_WALL;
    scene->world.grid[34][28].type = GRID_WALL;
}

void move_map()
{
    if (engine->input.rotLeft)
    {
        mapPosition.x--;
        shouldRender = true;
    }

    if (engine->input.rotRight)
    {
        mapPosition.x++;
        shouldRender = true;
    }

    if (engine->input.forwards)
    {
        mapPosition.y--;
        shouldRender = true;
    }

    if (engine->input.backwards)
    {
        mapPosition.y++;
        shouldRender = true;
    }

    if (engine->input.toggleDebug)
    {
        shouldRender = true;
        mapPosition.scale--;
        
        if (mapPosition.scale < GRID_MIN_SIZE)
        {
            mapPosition.scale = GRID_MIN_SIZE;
        }
        else
        {
            mapPosition.x += SCENE_WIDTH / 2;
            mapPosition.y += SCENE_WIDTH / 2;
        }
    }

    if (engine->input.toggleRenderMode)
    {
        shouldRender = true;
        mapPosition.scale++;
        
        if (mapPosition.scale > GRID_MAX_SIZE)
        {
            mapPosition.scale = GRID_MAX_SIZE;
        }
        else
        {
            mapPosition.x -= SCENE_WIDTH / 2;
            mapPosition.y -= SCENE_WIDTH / 2;
        }
    }
}

void render_grid_scene()
{
    draw_clear_screen32(&engine->screen, to_argb(&scene->floorCol));

    for (int j = 0; j < SCENE_HEIGHT; j++)
    {
        for (int i = 0; i < SCENE_WIDTH; i++)
        {
            if (scene->world.grid[i][j].type == GRID_WALL)
            {
                render_tile(i, j, &wallColor);
            }
            else if (scene->world.grid[i][j].type == GRID_PSPAWN)
            {
                render_tile(i, j, &pspawnColor);
            }
        }
    }

    if (drawGrid)
    {
        draw_grid32(
            &engine->screen,
            0xFF565656,
            mapPosition.x,
            mapPosition.y,
            mapPosition.scale,
            SCENE_WIDTH,
            SCENE_HEIGHT
        );
    }

    render_engine(engine);
}

void render_tile(
    const int col, 
    const int row,
    const color const* color)
{
    int posX = mapPosition.x + (col * mapPosition.scale);
    int posY = mapPosition.y + (row * mapPosition.scale);

    if (posX < 0 ||
        posX >= SWIDTH - mapPosition.scale)
    {
        return;
    }

    if (posY < 0 ||
        posY >= SHEIGHT - mapPosition.scale)
    {
        return;
    }

    draw_filled_rect32(
        &engine->screen,
        to_argb(color),
        posX,
        posY,
        mapPosition.scale,
        mapPosition.scale
    );
}