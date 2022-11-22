#include "engine_logic.h"
#include "engine_subsystems.h"
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

rayengine *init_engine(const engine_config const* config)
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
