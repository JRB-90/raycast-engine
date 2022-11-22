#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine_logic.h"
#include "engine_screen.h"
#include "engine_draw.h"
#include "engine_color.h"

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
