#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>
#include "engine_logic.h"
#include "engine_screen.h"
#include "engine_draw.h"
#include "engine_color.h"
#include "time_helper.h"

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

    engine = init_engine(&config16);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
    }

    draw_clear_screen16(&engine->screen, 0x00);
    render_engine(engine);

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
                to_rgb565(&c),
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
                to_rgb565(&c),
                x,
                y,
                w,
                h
            );
        }

        render_engine(engine);

        if (isSlow)
        {
            sleep_millis(SLOW_SLEEP_TIME);
        }
    }

    destroy_engine(engine);

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

    engine = init_engine(&config32);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    draw_clear_screen32(&engine->screen, 0x0000);
    render_engine(engine);

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
                to_argb(&c),
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
                to_argb(&c),
                x,
                y,
                w,
                h
            );
        }

        render_engine(engine);

        if (isSlow)
        {
            sleep_millis(SLOW_SLEEP_TIME);
        }
    }

    destroy_engine(engine);

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
        destroy_engine(engine);
    }

    exit(status);
}
