#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "engine/engine_subsystems.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_render.h"
#include "gridengine/gridengine_testscenes.h"
#include "crossplatform/crossplatform_time.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;
const int SSIZE = 30;
const int GRID_MIN_SIZE = 4;
const int GRID_MAX_SIZE = 30;
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
map_pos mapPosition;

bool drawGrid = true;
bool shouldRender = true;

void sig_handler(int signum);
void cleanup(int status);
void render_scene();

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    signal(SIGINT, sig_handler);

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

    scene = create_test_scene2("Map drawing test scene", &config.format);
    if (scene == NULL)
    {
        fprintf(stderr, "Failed to create test scene, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    mapPosition = (map_pos)
    {
        .x = (-scene->player.position.x * SSIZE) + (SWIDTH / 2),
        .y = (-scene->player.position.y * SSIZE) + (SHEIGHT / 2),
        .scale = SSIZE,
    };

    engine = engine_create_new(&config);
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
        engine_update_input_state(&engine->input);

        shouldRender =
            move_player(
                &engine->input,
                scene,
                &WORLD_FWD,
                &WORLD_LEFT,
                3.0f
            );

        if (shouldRender)
        {
            mapPosition.x = (-scene->player.position.x * SSIZE) + (SWIDTH / 2);
            mapPosition.y = (-scene->player.position.y * SSIZE) + (SHEIGHT / 2);

            clktimer_start(&timer);

            render_scene();

            deltatime delta = clktimer_elapsed_ms(&timer);
            printf("Map render took %.3fms\n", delta);
            totalTime += delta;
            renderCount++;

            engine_render_screen(&engine->screen);
        }

        shouldRender = false;
        cross_sleep_ms(1);
    }

    engine_destroy(engine);
    destroy_test_scene(scene);
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
        engine_destroy(engine);
    }

    if (scene != NULL)
    {
        destroy_test_scene(scene);
    }

    if (status != EXIT_SUCCESS)
    {
        getchar();
    }

    exit(status);
}

void render_scene()
{
    render_grid_scene(
        engine,
        scene,
        &mapPosition,
        true
    );

    render_grid_rays(
        engine,
        scene,
        &mapPosition,
        &scene->player
    );

    draw_grid32(
        &engine->screen,
        0xFF565656,
        mapPosition.x,
        mapPosition.y,
        mapPosition.scale,
        SCENE_WIDTH,
        SCENE_HEIGHT
    );

    render_grid_sprites(
        engine,
        scene,
        &mapPosition
    );

    render_grid_player(
        engine,
        &mapPosition,
        &scene->player
    );

    engine_render_screen(&engine->screen);
}
