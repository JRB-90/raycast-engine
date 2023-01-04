#include "engine/engine_screen.h"
#include <stdio.h>

screen_buffer default_screen()
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
