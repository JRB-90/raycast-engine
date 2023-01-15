#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_draw.h"
#include "engine/engine_color.h"
#include "engine/engine_subsystems.h"
#include "crossplatform/crossplatform_time.h"

const int SWIDTH = 640;
const int SHEIGHT = 480;
const int NUM_RECTS = 10000;
const int SLOW_SLEEP_TIME = 50;
const bool isFilled = true;
const bool isSlow = false;

void sig_handler(int signum);
void cleanup(int status);

rayengine* engine;

int main(int argc, char** argv)
{
    engine = NULL;
    signal(SIGINT, sig_handler);
    srand((unsigned int)time(NULL));

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

    printf("16 BPP test\n");

    engine = engine_create_new_rayengine(&config16);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    draw_clear_screen16(&engine->screen, 0x00);
    engine_render_screen(&engine->screen);

    for (int i = 0; i < NUM_RECTS; i++)
    {
        color c =
        {
            .a = 255,
            .r = (uint8_t)rand(),
            .g = (uint8_t)rand(),
            .b = (uint8_t)rand()
        };

        int x = rand() % SWIDTH;
        int w = rand() % (SWIDTH - x);
        int y = rand() % SHEIGHT;
        int h = rand() % (SHEIGHT - y);

        if (isFilled)
        {
            draw_filled_rect16(
                &engine->screen,
                color_to_rgb565(&c),
                x,
                y,
                w,
                h
            );
        }
        else
        {
            draw_unfilled_rect16(
                &engine->screen,
                color_to_rgb565(&c),
                x,
                y,
                w,
                h
            );
        }

        engine_render_screen(&engine->screen);

        if (isSlow)
        {
            cross_sleep_ms(SLOW_SLEEP_TIME);
        }
    }

    engine_destroy_rayengine(engine);

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

    printf("32 BPP test\n");

    engine = engine_create_new_rayengine(&config32);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    draw_clear_screen32(&engine->screen, 0x0000);
    engine_render_screen(&engine->screen);

    for (int i = 0; i < NUM_RECTS; i++)
    {
        color c =
        {
            .a = 255,
            .r = (uint8_t)rand(),
            .g = (uint8_t)rand(),
            .b = (uint8_t)rand()
        };

        int x = rand() % SWIDTH;
        int w = rand() % (SWIDTH - x);
        int y = rand() % SHEIGHT;
        int h = rand() % (SHEIGHT - y);

        if (isFilled)
        {
            draw_filled_rect32(
                &engine->screen,
                color_to_argb(&c),
                x,
                y,
                w,
                h
            );
        }
        else
        {
            draw_unfilled_rect32(
                &engine->screen,
                color_to_argb(&c),
                x,
                y,
                w,
                h
            );
        }

        engine_render_screen(&engine->screen);

        if (isSlow)
        {
            cross_sleep_ms(SLOW_SLEEP_TIME);
        }
    }

    engine_destroy_rayengine(engine);

    printf("Demo complete\n");
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
        engine_destroy_rayengine(engine);
    }

    exit(status);
}
