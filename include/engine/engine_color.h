#pragma once

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

extern color to_col(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
extern int to_bpp(const colformat colorFormat);
extern col_rgb565 to_rgb565(const color* const color);
extern col_argb to_argb(const color* const color);