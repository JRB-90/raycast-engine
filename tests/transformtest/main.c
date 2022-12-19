#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "time/time_helper.h"
#include "engine/engine_rayengine.h"
#include "engine/engine_screen.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;

rayengine* engine;

void sig_handler(int signum);
void cleanup(int status);
void render();

int main(int argc, char** argv)
{
    engine = NULL;

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

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    render();

    while (!engine->input.quit)
    {
        update_engine(engine);
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

    exit(status);
}

void render()
{
    draw_clear_screen32(&engine->screen, 0xFF000000);

    // TODO

    render_engine(engine);
}