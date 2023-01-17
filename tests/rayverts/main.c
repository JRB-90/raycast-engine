#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "engine/engine_resource.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_render.h"
#include "gridengine/gridengine_testscenes.h"
#include "crossplatform/crossplatform_time.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;
const int SSIZE = 30;

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

void sig_handler(int signum);
void cleanup(int status);
int on_update(const input_state* const inputState, const float deltaTimeMS);
int on_render(screen_buffer* const screen);

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    signal(SIGINT, sig_handler);

    engine_config config =
    {
        .type = ENGINE_GRID,
        .targetFps = 10000,
        .format = (screen_format)
        {
            .format = SFORMAT,
            .width = SWIDTH,
            .height = SHEIGHT
        }
    };

    scene = gridengine_create_test_scene2("Vert drawing test scene", &config.format);
    if (scene == NULL)
    {
        fprintf(stderr, "Failed to create test scene, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    engine->on_update = &on_update;
    engine->on_render = &on_render;

    int res = engine_run_rayengine(engine);

    engine_destroy_rayengine(engine);
    gridengine_destroy_test_scene(scene);
    //int c = getchar();

    exit(res == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
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
        engine_destroy_rayengine(engine);
    }

    if (scene != NULL)
    {
        gridengine_destroy_test_scene(scene);
    }

    if (status != EXIT_SUCCESS)
    {
        getchar();
    }
    
    exit(status);
}

int on_update(const input_state* const inputState, const float deltaTimeMS)
{
    printf("Delta: %.3fms\n", deltaTimeMS);
    gridengine_move_player(
        inputState,
        scene,
        &WORLD_FWD,
        &WORLD_LEFT,
        deltaTimeMS
    );

    frame2d_print(&scene->player.position);

    return 0;
}

int on_render(screen_buffer* const screen)
{
    if (SFORMAT == CF_RGB565)
    {
        draw_ceiling_floor16(
            &engine->screen,
            color_to_rgb565(&scene->colors.ceilingCol),
            color_to_rgb565(&scene->colors.floorCol)
        );
    }
    else
    {
        draw_ceiling_floor32(
            &engine->screen,
            color_to_argb(&scene->colors.ceilingCol),
            color_to_argb(&scene->colors.floorCol)
        );
    }

    gridengine_render_firstperson(
        engine,
        scene
    );

    gridengine_render_sprites(
        engine,
        scene,
        &scene->player.position
    );

    return 0;
}
