#pragma once

#include "engine_color.h"

typedef struct {
    void* pixels;
    int width;
    int height;
    int sizeInPixels;
    int stride;
    int sizeInBytes;
    int bitsPP;
    int bytesPP;
    colformat colorFormat;
} screen_buffer;

typedef struct {
    int width;
    int height;
    colformat format;
} screen_format;

extern screen_buffer default_screen();