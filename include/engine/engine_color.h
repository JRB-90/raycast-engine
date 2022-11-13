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

extern int to_bpp(const colformat colorFormat);