#include "engine_logic.h"

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
