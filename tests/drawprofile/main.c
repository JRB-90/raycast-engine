#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine_logic.h"
#include "engine_screen.h"
#include "engine_draw.h"
#include "engine_color.h"
#include "time_helper.h"

const int CLEAR_ITR = 1000;

void sig_handler(int signum);
void cleanup(int status);

volatile rayengine* engine;

int main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    engine = NULL;

    engine_config config =
    {
        .type = ENGINE_GRID,
        .format = (screen_format)
        {
            .format = CF_ARGB,
            .width = 640,
            .height = 480
        }
    };

	printf("Initialising...\n");

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
    }

    printf("Initialised\n");

    // TODO - Draw tests here

    clktimer timer;
    deltatime delta;

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_16(&engine->screen, 0xFFFF);
    }
    delta = elapsed_millis(&timer);
    printf("16 bit test took %.3fms %i iterations\n", delta, CLEAR_ITR);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_32(&engine->screen, 0xFF00FF00);
    }
    delta = elapsed_millis(&timer);
    printf("32 bit test took %.3fms %i iterations\n", delta, CLEAR_ITR);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_64(&engine->screen, 0xFF0000FFFF0000FF);
    }
    delta = elapsed_millis(&timer);
    printf("64 bit test took %.3fms %i iterations\n", delta, CLEAR_ITR);
    render_engine(engine);

    while (!engine->input.quit)
    {
        update_engine(engine);
    }

    printf("Shutting down...\n");
    destroy_engine(engine);
    printf("Shutdown\n");
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

    exit(status);
}
