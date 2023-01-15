#include "engine/engine_color.h"

color color_build(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    color c =
    {
        .a = a,
        .r = r,
        .g = g,
        .b = b
    };

    return c;
}

int colformat_to_bpp(const colformat colorFormat)
{
    switch (colorFormat)
    {
    case CF_ARGB:
        return 32;
    case CF_RGB565:
        return 16;
    default:
        return -1;
    }
}

col_rgb565 color_to_rgb565(const color* const color)
{
    uint16_t r = (color->r >> 3) << 11;
    uint16_t g = (color->g >> 2) << 5;
    uint16_t b = (color->b >> 3);

    return r | g | b;
}

col_argb color_to_argb(const color* const color)
{
    return
        ((uint32_t)color->a << 24) |
        ((uint32_t)color->r << 16) |
        ((uint32_t)color->g << 8) |
        ((uint32_t)color->b);
}
