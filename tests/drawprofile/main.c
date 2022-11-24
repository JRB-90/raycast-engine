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
void run_16bit_tests();
void run_32bit_tests();

rayengine* engine;

int main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    engine = NULL;

    run_16bit_tests();
    run_32bit_tests();

    printf("====== Tests complete ======\n");
    //getchar();
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

void run_16bit_tests()
{
    engine_config config =
    {
        .type = ENGINE_GRID,
        .format = (screen_format)
        {
            .format = CF_RGB565,
            .width = 640,
            .height = 480
        }
    };

    printf("Initialising engine for 16 bit color...\n");

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
    }

    printf("Initialised\n");

    draw_clear_screen_16(&engine->screen, 0b1111100000011111);
    render_engine(engine);

    /*clktimer timer;

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_16(&engine->screen, 0b1111100000011111);
    }
    deltatime delta16 = elapsed_millis(&timer);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_32(&engine->screen, 0b11111111111000001111111111100000);
    }
    deltatime delta32 = elapsed_millis(&timer);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_64(&engine->screen, 0b0000011111111111000001111111111100000111111111110000011111111111);
    }
    deltatime delta64 = elapsed_millis(&timer);
    render_engine(engine);*/

    sleep_secs(1);

    printf("Shutting down engine...\n");
    destroy_engine(engine);
    printf("Shutdown\n");

    //printf("\n16 bit test took %.3fms %i iterations\n", delta16, CLEAR_ITR);
    //printf("32 bit test took %.3fms %i iterations\n", delta32, CLEAR_ITR);
    //printf("64 bit test took %.3fms %i iterations\n\n", delta64, CLEAR_ITR);
}

void run_32bit_tests()
{
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

    printf("Initialising engine for 32 bit color...\n");

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
    }

    printf("Initialised\n");

    /*clktimer timer;

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_16(&engine->screen, 0xFFFF);
    }
    deltatime delta16 = elapsed_millis(&timer);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_32(&engine->screen, 0xFF00FF00);
    }
    deltatime delta32 = elapsed_millis(&timer);
    render_engine(engine);

    start_timer(&timer);
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        draw_clear_screen_64(&engine->screen, 0xFF0000FFFF0000FF);
    }
    deltatime delta64 = elapsed_millis(&timer);
    render_engine(engine);*/

    printf("Shutting down engine...\n");
    destroy_engine(engine);
    printf("Shutdown\n");

    //printf("\n16 bit test took %.3fms %i iterations\n", delta16, CLEAR_ITR);
    //printf("32 bit test took %.3fms %i iterations\n", delta32, CLEAR_ITR);
    //printf("64 bit test took %.3fms %i iterations\n\n", delta64, CLEAR_ITR);
}
