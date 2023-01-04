#include "engine/engine_rayengine.h"
#include "engine/engine_subsystems.h"
#include "time/time_helper.h"
#include <stdio.h>
#include <stdlib.h>

input_state blank_input_state()
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

rayengine *init_engine(const engine_config *const config)
{
    rayengine *engine = (rayengine *)malloc(sizeof(rayengine));

    if ((engine) == NULL)
    {
        fprintf(stderr, "Failed to malloc rayengine\n");
        return NULL;
    }

    engine->config = *config;
    engine->screen = default_screen();
    engine->input = blank_input_state();
    engine->on_update = NULL;

    if (init_input_subsystem())
    {
        return NULL;
    }

    if (init_render_subsystem(&engine->config.format, &engine->screen))
    {
        destroy_input_subsystem();

        return NULL;
    }

    return engine;
}

void destroy_engine(rayengine *engine)
{
    if (engine != NULL)
    {
        destroy_render_subsystem(&engine->screen);
        destroy_input_subsystem();
        free(engine);
    }
}

int update_engine(rayengine *engine)
{
    return update_input_state(&engine->input);
    // TODO - In future, update AI logic here too
}

int render_engine(rayengine *engine)
{
    return render_screen(&engine->screen);
}

int run_engine(rayengine* const engine)
{
    // Ensure callbacks have been set
    if (engine->on_update == NULL ||
        engine->on_render == NULL)
    {
        fprintf(stderr, "Engine callbacks have not been set\n");
        return -1;
    }

    float targetDeltaMS = 1000.0f / (float)engine->config.targetFps;
    clktick previousTicks = get_ticks();
    clktick currentTicks = previousTicks;

    input_state inputState = blank_input_state();

    while (inputState.quit == false)
    {
        currentTicks = get_ticks();
        deltatime deltaTimeMS = get_delta_ms(currentTicks - previousTicks);

        if (deltaTimeMS > targetDeltaMS)
        {
            int res;

            res = update_input_state(&inputState);
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

            previousTicks = currentTicks;
        }
    }

    return 0;
}
