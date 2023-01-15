#include "engine/engine_rayengine.h"

#include <stdio.h>
#include <stdlib.h>
#include "engine/engine_subsystems.h"
#include "crossplatform/crossplatform_time.h"

input_state engine_get_default_input()
{
    input_state inputState =
    {
        .quit = false,
        .forwards = false,
        .backwards = false,
        .rotLeft = false,
        .rotRight = false,
        .left = false,
        .right = false,
        .toggleDebug = false,
        .toggleRenderMode = false,
    };

    return inputState;
}

screen_buffer engine_get_default_screen()
{
    screen_buffer screen =
    {
        .pixels = NULL,
        .width = -1,
        .height = -1,
        .sizeInPixels = -1,
        .stride = -1,
        .sizeInBytes = -1,
        .bitsPP = -1,
        .bytesPP = -1,
        .colorFormat = CF_ARGB
    };

    return screen;
}

rayengine *engine_create_new(const engine_config *const config)
{
    rayengine *engine = (rayengine *)malloc(sizeof(rayengine));

    if ((engine) == NULL)
    {
        fprintf(stderr, "Failed to malloc rayengine\n");
        return NULL;
    }

    engine->config = *config;
    engine->screen = engine_get_default_screen();
    engine->input = engine_get_default_input();
    engine->on_update = NULL;

    if (engine_init_input_subsystem())
    {
        return NULL;
    }

    if (engine_init_render_subsystem(&engine->config.format, &engine->screen))
    {
        engine_destroy_input_subsystem();

        return NULL;
    }

    return engine;
}

void engine_destroy(rayengine *engine)
{
    if (engine != NULL)
    {
        engine_destroy_render_subsystem(&engine->screen);
        engine_destroy_input_subsystem();
        free(engine);
    }
}

int update_engine(rayengine *engine)
{
    return engine_update_input_state(&engine->input);
    // TODO - In future, update AI logic here too
}

int render_engine(rayengine *engine)
{
    return engine_render_screen(&engine->screen);
}

int engine_run(rayengine* const engine)
{
    // Ensure callbacks have been set
    if (engine->on_update == NULL ||
        engine->on_render == NULL)
    {
        fprintf(stderr, "Engine callbacks have not been set\n");
        return -1;
    }

    float targetDeltaMS = 1000.0f / (float)engine->config.targetFps;
    clktick previousTicks = cross_get_ticks();
    clktick currentTicks = previousTicks;

    input_state inputState = engine_get_default_input();

    while (inputState.quit == false)
    {
        currentTicks = cross_get_ticks();
        deltatime deltaTimeMS = cross_delta_ms(currentTicks - previousTicks);

        if (deltaTimeMS > targetDeltaMS)
        {
            int res;

            res = engine_update_input_state(&inputState);
            if (res)
            {
                return res;
            }

            if (inputState.quit)
            {
                continue;
            }

            res = engine->on_update(&inputState, deltaTimeMS);
            if (res)
            {
                return res;
            }

            res = engine->on_render(&engine->screen);
            if (res)
            {
                return res;
            }

            res = engine_render_screen(&engine->screen);
            if (res)
            {
                return res;
            }

            previousTicks = currentTicks;
        }
    }

    return 0;
}
