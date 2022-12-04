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

void sig_handler(int signum);
void cleanup(int status);
void render_grid_scene();

rayengine* engine;
grid_scene* scene;

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    signal(SIGINT, sig_handler);

    scene = create_scene("Test Grid Scene");

    scene->world.grid[32][32].type = GRID_WALL;
    scene->world.grid[32][32].textureID = 1;

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

    draw_clear_screen32(&engine->screen, 0x0000);
    render_engine(engine);

    input_state input = blank_input_state();

    while (!engine->input.quit)
    {
        update_engine(engine);
        render_grid_scene();
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

void render_grid_scene()
{
    draw_clear_screen32(&engine->screen, 0x0000);

    // TODO

    render_engine(engine);
}