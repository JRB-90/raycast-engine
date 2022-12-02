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
const int RECT_ITR = 10000;

void sig_handler(int signum);
void cleanup(int status);
void run_basic_tests(engine_config config);
void run_basic_rect_tests(engine_config config);

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

    /*printf("\nStarting 16 bpp basic tests\n");
    run_basic_tests(config16);

    printf("\nStarting 32 bpp basic tests\n");
    run_basic_tests(config32);*/

    printf("\nStarting 16 bpp rect tests\n");
    run_basic_rect_tests(config16);

    printf("\nStarting 32 bpp rect tests\n");
    run_basic_rect_tests(config32);

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
    deltatime delta16 = 0;
    deltatime delta32 = 0;
    deltatime delta64 = 0;

    for (int i = 0; i < CLEAR_ITR; i++)
    {
        start_timer(&timer);
        draw_clear_screen16(&engine->screen, 0xFFFF);
        delta16 += elapsed_millis(&timer);
        render_engine(engine);
    }
    
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        start_timer(&timer);
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        delta32 += elapsed_millis(&timer);
        render_engine(engine);
    }
    
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        start_timer(&timer);
        draw_clear_screen64(&engine->screen, 0xFFFFFFFFFFFFFFFF);
        delta64 += elapsed_millis(&timer);
        render_engine(engine);
    }

    printf("Shutting down engine...\n");
    destroy_engine(engine);
    printf("Shutdown\n");

    printf(
        "16 bit test took %.3fms %i iterations, ave: %.3fms\n",
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
        "64 bit test took %.3fms %i iterations, ave: %.3fms\n",
        delta64,
        CLEAR_ITR,
        delta64 / (deltatime)CLEAR_ITR
    );
}

void run_basic_rect_tests(engine_config config)
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
    deltatime filledDelta = 0;
    deltatime unfilledDelta = 0;

    if (config.format.format == CF_RGB565)
    {
        draw_clear_screen16(&engine->screen, 0xFFFF);
        render_engine(engine);
        
        for (int i = 0; i < RECT_ITR; i++)
        {
            start_timer(&timer);

            draw_filled_rect16(
                &engine->screen,
                0b1111100000000000,
                50, 50,
                100, 150
            );

            filledDelta += elapsed_millis(&timer);
            render_engine(engine);
        }

        draw_clear_screen16(&engine->screen, 0x0000);
        render_engine(engine);

        for (int i = 0; i < RECT_ITR; i++)
        {
            start_timer(&timer);

            draw_unfilled_rect16(
                &engine->screen,
                0b1111100000000000,
                50, 50,
                100, 150
            );

            unfilledDelta += elapsed_millis(&timer);
            render_engine(engine);
        }
    }
    else if (config.format.format == CF_ARGB)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        render_engine(engine);

        for (int i = 0; i < RECT_ITR; i++)
        {
            start_timer(&timer);

            draw_filled_rect32(
                &engine->screen,
                0xFF00FF00,
                50, 50,
                100, 150
            );

            filledDelta += elapsed_millis(&timer);
            render_engine(engine);
        }

        draw_clear_screen32(&engine->screen, 0x00000000);
        render_engine(engine);

        for (int i = 0; i < RECT_ITR; i++)
        {
            start_timer(&timer);

            draw_unfilled_rect32(
                &engine->screen,
                0xFF00FF00,
                50, 50,
                100, 150
            );

            unfilledDelta += elapsed_millis(&timer);
            render_engine(engine);
        }
    }

    printf("Shutting down engine...\n");
    destroy_engine(engine);
    printf("Shutdown\n");

    printf(
        "Filled rect took %.3fms %i iterations, ave: %.3fms\n",
        filledDelta,
        RECT_ITR,
        filledDelta / (deltatime)RECT_ITR
    );

    printf(
        "Unfilled rect took %.3fms %i iterations, ave: %.3fms\n",
        unfilledDelta,
        RECT_ITR,
        unfilledDelta / (deltatime)RECT_ITR
    );
}