#pragma once

/*
    Provides an abstracted view of colors.
    color object can then be converted to the relevant subformats when needed.
*/

#include <inttypes.h>

typedef enum {
    CF_ARGB,
    CF_RGB565,
} colformat;

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color;

typedef uint16_t col_rgb565;
typedef uint32_t col_argb;

/// <summary>
/// Composes a new color from it's components.
/// </summary>
/// <param name="a">Alpha component.</param>
/// <param name="r">Red component.</param>
/// <param name="g">Green component.</param>
/// <param name="b">Blue component.</param>
/// <returns>Color structure.</returns>
extern color color_build(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

/// <summary>
/// Calculates the bits per pixel from a color format.
/// </summary>
/// <param name="colorFormat">Color format to convert.</param>
/// <returns>Bits per pixel.</returns>
extern int colformat_to_bpp(const colformat colorFormat);

/// <summary>
/// Converts a color to the RGB565 format.
/// </summary>
/// <param name="color">Color to convert.</param>
/// <returns>Color in RGB565 format.</returns>
extern col_rgb565 color_to_rgb565(const color* const color);

/// <summary>
/// Converts a color to the ARGB format.
/// </summary>
/// <param name="color">Color to convert.</param>
/// <returns>Color in ARGB format.</returns>
extern col_argb color_to_argb(const color* const color);