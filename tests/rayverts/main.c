#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_screen.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_render.h"
#include "time/time_helper.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;
const int SSIZE = 30;
const int GRID_MIN_SIZE = 4;
const int GRID_MAX_SIZE = 30;
const float TRANS_AMT = 0.025f;
const float ROT_AMT = 0.005f;

const vec2d WORLD_FWD =
{
    .x = 0.0,
    .y = 1.0
};

const vec2d WORLD_LEFT =
{
    .x = 1.0,
    .y = 0.0
};

rayengine* engine;
grid_scene* scene;

bool shouldRender = true;

void sig_handler(int signum);
void cleanup(int status);
void build_test_scene();
void move_map();
void render_scene();

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    signal(SIGINT, sig_handler);

    scene = create_scene("Vert drawing test scene");
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
        exit(EXIT_FAILURE);
    }

    render_scene();

    clktimer timer;
    deltatime totalTime = (deltatime)0.0;
    int renderCount = 0;

    while (!engine->input.quit)
    {
        update_engine(engine);
        move_map();

        if (shouldRender)
        {
            start_timer(&timer);

            render_scene();

            deltatime delta = elapsed_millis(&timer);
            printf("Map render took %.3fms\n", delta);
            totalTime += delta;
            renderCount++;

            render_engine(engine);
        }

        shouldRender = false;
        sleep_millis(1);
    }

    destroy_engine(engine);
    destroy_scene(scene);

    sleep_millis(500);
    float aveRenderTime = totalTime / (deltatime)renderCount;
    printf("\nAverage render time: %.3f\n", aveRenderTime);
    int c = getchar();

    exit(EXIT_SUCCESS);
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

    scene->world.grid[35][25].type = GRID_WALL;
    scene->world.grid[36][25].type = GRID_WALL;
    scene->world.grid[37][25].type = GRID_WALL;

    scene->world.grid[36][28].type = GRID_WALL;
    scene->world.grid[36][29].type = GRID_WALL;
    scene->world.grid[36][31].type = GRID_WALL;
}

void move_map()
{
    if (engine->input.forwards)
    {
        vec2d travelDir = calc_forwards(&scene->player.position, &WORLD_FWD);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        scene->player.position.x += transVec.x;
        scene->player.position.y += transVec.y;
        shouldRender = true;
    }

    if (engine->input.backwards)
    {
        vec2d travelDir = calc_forwards(&scene->player.position, &WORLD_FWD);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        scene->player.position.x -= transVec.x;
        scene->player.position.y -= transVec.y;
        shouldRender = true;
    }

    if (engine->input.left)
    {
        vec2d travelDir = calc_forwards(&scene->player.position, &WORLD_LEFT);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        scene->player.position.x += transVec.x;
        scene->player.position.y += transVec.y;
        shouldRender = true;
    }

    if (engine->input.right)
    {
        vec2d travelDir = calc_forwards(&scene->player.position, &WORLD_LEFT);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        scene->player.position.x -= transVec.x;
        scene->player.position.y -= transVec.y;
        shouldRender = true;
    }

    if (engine->input.rotRight)
    {
        scene->player.position.theta += ROT_AMT;
        shouldRender = true;
    }

    if (engine->input.rotLeft)
    {
        scene->player.position.theta -= ROT_AMT;
        shouldRender = true;
    }
}

void render_scene()
{
    draw_ceiling_floor32(
        &engine->screen,
        to_argb(&scene->colors.ceilingCol),
        to_argb(&scene->colors.floorCol)
    );

    render_grid_verts(
        engine,
        scene
    );

    render_engine(engine);
}