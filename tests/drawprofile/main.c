#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine_logic.h"
#include "engine_screen.h"
#include "engine_draw.h"
#include "engine_color.h"
#include "time_helper.h"

const int SWIDTH = 640;
const int SHEIGHT = 480;
const int CLEAR_ITR = 1000;

void sig_handler(int signum);
void cleanup(int status);
void run_basic_tests(engine_config config);

rayengine* engine;

int main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    engine = NULL;

    engine_config config16 =
    {
        .type = ENGINE_GRID,
        .format = (screen_format)
        {
            .format = CF_RGB565,
            .width = SWIDTH,
            .height = SHEIGHT
        }
    };

    engine_config config32 =
    {
        .type = ENGINE_GRID,
        .format = (screen_format)
        {
            .format = CF_ARGB,
            .width = SWIDTH,
            .height = SHEIGHT
        }
    };

    printf("\nStarting 16 bpp tests\n");
    run_basic_tests(config16);

    printf("\nStarting 32 bpp tests\n");
    run_basic_tests(config32);

    printf("\n====== Tests complete ======\n");
    getchar();
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

void run_basic_tests(engine_config config)
{
    printf("Initialising engine...\n");

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
    }

    printf("Initialised\n");

    clktimer timer;

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen16(&engine->screen, 0xFFFF);
    }
    deltatime delta16 = elapsed_millis(&timer);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
    }
    deltatime delta32 = elapsed_millis(&timer);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen64(&engine->screen, 0xFFFFFFFFFFFFFFFF);
    }
    deltatime delta64 = elapsed_millis(&timer);
    render_engine(engine);

    printf("Shutting down engine...\n");
    destroy_engine(engine);
    printf("Shutdown\n");

    printf(
        "16 bit test took %.3fms %i iterations, ave: %.3f\n",
        delta16, 
        CLEAR_ITR,
        delta16 / (deltatime)CLEAR_ITR
    );

    printf(
        "32 bit test took %.3fms %i iterations, ave: %.3fms\n",
        delta32,
        CLEAR_ITR,
        delta32 / (deltatime)CLEAR_ITR
    );

    printf(
        "64 bit test took %.3fms %i iterations, ave: %.3f\n",
        delta64,
        CLEAR_ITR,
        delta64 / (deltatime)CLEAR_ITR
    );
}
