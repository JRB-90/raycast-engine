#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_screen.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "engine/engine_resource.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_render.h"
#include "gridengine/gridengine_testscenes.h"
#include "time/time_helper.h"

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

    scene = create_test_scene1("Vert drawing test scene", &config.format);
    if (scene == NULL)
    {
        fprintf(stderr, "Failed to create test scene, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    engine->on_update = &on_update;
    engine->on_render = &on_render;

    int res = run_engine(engine);

    destroy_engine(engine);
    destroy_test_scene(scene);
    int c = getchar();

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
        destroy_engine(engine);
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

int on_update(const input_state* const inputState, const float deltaTimeMS)
{
    printf("Delta: %.3fms\n", deltaTimeMS);
    move_player(
        inputState,
        scene,
        &WORLD_FWD,
        &WORLD_LEFT,
        deltaTimeMS
    );

    return 0;
}

int on_render(screen_buffer* const screen)
{
    if (SFORMAT == CF_RGB565)
    {
        draw_ceiling_floor16(
            &engine->screen,
            to_rgb565(&scene->colors.ceilingCol),
            to_rgb565(&scene->colors.floorCol)
        );
    }
    else
    {
        draw_ceiling_floor32(
            &engine->screen,
            to_argb(&scene->colors.ceilingCol),
            to_argb(&scene->colors.floorCol)
        );
    }

    render_grid_verts(
        engine,
        scene
    );

    if (SFORMAT == CF_RGB565)
    {

    }
    else
    {
        render_sprites32(
            engine,
            scene
        );
    }

    return 0;
}
