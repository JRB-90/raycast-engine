#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_draw.h"
#include "engine/engine_color.h"
#include "engine/engine_subsystems.h"
#include "crossplatform/crossplatform_time.h"

#define LINES_TO_READ 10000
#define RECTS_TO_READ 10000

const int SWIDTH = 640;
const int SHEIGHT = 480;
const int CLEAR_ITR = 1000;
const int LINE_ITR = 10000;
const int RECT_ITR = 10000;
const char* LINE_FILE = "shapefiles/lines.dat";
const char* RECT_FILE = "shapefiles/rects.dat";

typedef enum {
    line = 1,
    rect = 2
} shape_type;

typedef struct {
    int p1;
    int p2;
    int p3;
    int p4;
} param_4;

void sig_handler(int signum);
void cleanup(int status);
void run_basic_tests(engine_config config);
void run_basic_line_tests(engine_config config);
void run_basic_rect_tests(engine_config config);
void run_file_line_tests(engine_config config);
void run_file_rect_tests(engine_config config);

rayengine* engine;
param_4 params[LINES_TO_READ];

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

    printf("\nStarting 16 bpp basic tests\n");
    run_basic_tests(config16);

    printf("\nStarting 32 bpp basic tests\n");
    run_basic_tests(config32);

    printf("\nStarting 16 bpp line basic tests\n");
    run_basic_line_tests(config16);

    printf("\nStarting 32 bpp line basic tests\n");
    run_basic_line_tests(config32);

    printf("\nStarting 16 bpp rect basic tests\n");
    run_basic_rect_tests(config16);

    printf("\nStarting 32 bpp rect basic tests\n");
    run_basic_rect_tests(config32);

    printf("\nStarting 16 bpp line file tests\n");
    run_file_line_tests(config16);

    printf("\nStarting 32 bpp line file tests\n");
    run_file_line_tests(config32);

    printf("\nStarting 16 bpp line file tests\n");
    run_file_rect_tests(config16);

    printf("\nStarting 32 bpp line file tests\n");
    run_file_rect_tests(config32);

    printf("\n====== Tests complete ======\n");
    int c = getchar();
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

void run_basic_tests(engine_config config)
{
    printf("Initialising engine...\n");

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    printf("Initialised\n");

    clktimer timer;
    deltatime delta16 = 0;
    deltatime delta32 = 0;
    deltatime delta64 = 0;

    for (int i = 0; i < CLEAR_ITR; i++)
    {
        clktimer_start(&timer);
        draw_clear_screen16(&engine->screen, 0xFFFF);
        delta16 += clktimer_elapsed_ms(&timer);
        engine_render_screen(&engine->screen);
    }
    
    for (int i = 0; i < CLEAR_ITR; i++)
    {
        clktimer_start(&timer);
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        delta32 += clktimer_elapsed_ms(&timer);
        engine_render_screen(&engine->screen);
    }

    printf("Shutting down engine...\n");
    engine_destroy_rayengine(engine);
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
}

void run_basic_line_tests(engine_config config)
{
    printf("Initialising engine...\n");

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    printf("Initialised\n");

    clktimer timer;
    deltatime delta = 0;

    if (config.format.format == CF_RGB565)
    {
        draw_clear_screen16(&engine->screen, 0xFFFF);
        render_engine(engine);

        for (int i = 0; i < LINE_ITR; i++)
        {
            clktimer_start(&timer);

            draw_line16(
                &engine->screen,
                0b1111100000000000,
                200, 200,
                180, 100
            );

            delta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }
    else if (config.format.format == CF_ARGB)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        render_engine(engine);

        for (int i = 0; i < LINE_ITR; i++)
        {
            clktimer_start(&timer);

            draw_line32(
                &engine->screen,
                0b1111100000000000,
                200, 200,
                180, 100
            );

            delta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }

    printf("Shutting down engine...\n");
    engine_destroy_rayengine(engine);
    printf("Shutdown\n");

    printf(
        "Line took %.3fms %i iterations, ave: %.3fms\n",
        delta,
        LINE_ITR,
        delta / (deltatime)LINE_ITR
    );
}

void run_basic_rect_tests(engine_config config)
{
    printf("Initialising engine...\n");

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
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
            clktimer_start(&timer);

            draw_filled_rect16(
                &engine->screen,
                0b1111100000000000,
                50, 50,
                100, 150
            );

            filledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }

        draw_clear_screen16(&engine->screen, 0x0000);
        render_engine(engine);

        for (int i = 0; i < RECT_ITR; i++)
        {
            clktimer_start(&timer);

            draw_unfilled_rect16(
                &engine->screen,
                0b1111100000000000,
                50, 50,
                100, 150
            );

            unfilledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }
    else if (config.format.format == CF_ARGB)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        render_engine(engine);

        for (int i = 0; i < RECT_ITR; i++)
        {
            clktimer_start(&timer);

            draw_filled_rect32(
                &engine->screen,
                0xFF00FF00,
                50, 50,
                100, 150
            );

            filledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }

        draw_clear_screen32(&engine->screen, 0x00000000);
        render_engine(engine);

        for (int i = 0; i < RECT_ITR; i++)
        {
            clktimer_start(&timer);

            draw_unfilled_rect32(
                &engine->screen,
                0xFF00FF00,
                50, 50,
                100, 150
            );

            unfilledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }

    printf("Shutting down engine...\n");
    engine_destroy_rayengine(engine);
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

void run_file_line_tests(engine_config config)
{
    printf("Reading line file...\n");

    FILE* file = fopen(LINE_FILE, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Failed to open line file, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < LINES_TO_READ; i++)
    {
        int pcount;
        fread(&pcount, 1, sizeof(pcount), file);

        if (pcount == 4)
        {
            int type;
            int p1;
            int p2;
            int p3;
            int p4;

            fread(&type, 1, sizeof(type), file);
            fread(&p1, 1, sizeof(p1), file);
            fread(&p2, 1, sizeof(p2), file);
            fread(&p3, 1, sizeof(p3), file);
            fread(&p4, 1, sizeof(p4), file);

            if (type == line)
            {
                params[i] = (param_4)
                {
                    .p1 = p1,
                    .p2 = p2,
                    .p3 = p3,
                    .p4 = p4
                };
            }
        }
    }

    fclose(file);

    printf("File read\n");

    printf("Initialising engine...\n");

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    printf("Initialised\n");

    clktimer timer;
    deltatime delta = 0;

    if (config.format.format == CF_RGB565)
    {
        draw_clear_screen16(&engine->screen, 0xFFFF);
        render_engine(engine);

        for (int i = 0; i < LINES_TO_READ; i++)
        {
            clktimer_start(&timer);

            draw_line16(
                &engine->screen,
                0b1111100000000000,
                params[i].p1, params[i].p2,
                params[i].p3, params[i].p4
            );

            delta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }
    else if (config.format.format == CF_ARGB)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        render_engine(engine);

        for (int i = 0; i < LINES_TO_READ; i++)
        {
            clktimer_start(&timer);

            draw_line32(
                &engine->screen,
                0b1111100000000000,
                params[i].p1, params[i].p2,
                params[i].p3, params[i].p4
            );

            delta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }

    printf("Shutting down engine...\n");
    engine_destroy_rayengine(engine);
    printf("Shutdown\n");

    printf(
        "Lines took %.3fms %i iterations, ave: %.3fms\n",
        delta,
        LINES_TO_READ,
        delta / (deltatime)LINES_TO_READ
    );
}

void run_file_rect_tests(engine_config config)
{
    printf("Reading rect file...\n");

    FILE* file = fopen(RECT_FILE, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Failed to open rect file, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < RECTS_TO_READ; i++)
    {
        int pcount;
        fread(&pcount, 1, sizeof(pcount), file);

        if (pcount == 4)
        {
            int type;
            int p1;
            int p2;
            int p3;
            int p4;

            fread(&type, 1, sizeof(type), file);
            fread(&p1, 1, sizeof(p1), file);
            fread(&p2, 1, sizeof(p2), file);
            fread(&p3, 1, sizeof(p3), file);
            fread(&p4, 1, sizeof(p4), file);

            if (type == rect)
            {
                params[i] = (param_4)
                {
                    .p1 = p1,
                    .p2 = p2,
                    .p3 = p3,
                    .p4 = p4
                };
            }
        }
    }

    fclose(file);

    printf("File read\n");

    printf("Initialising engine...\n");

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    printf("Initialised\n");

    clktimer timer;
    deltatime unfilledDelta = 0;
    deltatime filledDelta = 0;

    if (config.format.format == CF_RGB565)
    {
        draw_clear_screen16(&engine->screen, 0xFFFF);
        render_engine(engine);

        for (int i = 0; i < RECTS_TO_READ; i++)
        {
            clktimer_start(&timer);

            draw_unfilled_rect16(
                &engine->screen,
                0b1111100000000000,
                params[i].p1, params[i].p2,
                params[i].p3, params[i].p4
            );

            unfilledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }
    else if (config.format.format == CF_ARGB)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        render_engine(engine);

        for (int i = 0; i < RECTS_TO_READ; i++)
        {
            clktimer_start(&timer);

            draw_unfilled_rect32(
                &engine->screen,
                0b1111100000000000,
                params[i].p1, params[i].p2,
                params[i].p3, params[i].p4
            );

            unfilledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }

    if (config.format.format == CF_RGB565)
    {
        draw_clear_screen16(&engine->screen, 0xFFFF);
        render_engine(engine);

        for (int i = 0; i < RECTS_TO_READ; i++)
        {
            clktimer_start(&timer);

            draw_filled_rect16(
                &engine->screen,
                0b1111100000000000,
                params[i].p1, params[i].p2,
                params[i].p3, params[i].p4
            );

            unfilledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }
    else if (config.format.format == CF_ARGB)
    {
        draw_clear_screen32(&engine->screen, 0xFFFFFFFF);
        render_engine(engine);

        for (int i = 0; i < RECTS_TO_READ; i++)
        {
            clktimer_start(&timer);

            draw_filled_rect32(
                &engine->screen,
                0b1111100000000000,
                params[i].p1, params[i].p2,
                params[i].p3, params[i].p4
            );

            unfilledDelta += clktimer_elapsed_ms(&timer);
            render_engine(engine);
        }
    }

    printf("Shutting down engine...\n");
    engine_destroy_rayengine(engine);
    printf("Shutdown\n");

    printf(
        "Unfilled rects took %.3fms %i iterations, ave: %.3fms\n",
        unfilledDelta,
        RECTS_TO_READ,
        unfilledDelta / (deltatime)RECTS_TO_READ
    );

    printf(
        "Filled rects took %.3fms %i iterations, ave: %.3fms\n",
        filledDelta,
        RECTS_TO_READ,
        filledDelta / (deltatime)RECTS_TO_READ
    );
}
