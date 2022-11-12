#include "engine_color.h"

int to_bpp(const colformat colorFormat)
{
    switch (colorFormat)
    {
    case CF_ARGB:
        return 32;
    case CF_RGBA:
        return 32;
    case CF_RGB565:
        return 16;
    default:
        return -1;
    }
}
