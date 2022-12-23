#pragma once

#include <stdint.h>
#include "engine_color.h"

typedef enum {
	RES_BLOB = 1,
	RES_TEXTURE = 2
} resource_type;

typedef struct {
	resource_type type;
	int id;
	uint64_t fileOffset;
	uint64_t size;
} resource_header;

typedef struct {
	resource_header header;
	uint8_t* data;
} blob_resource;

typedef struct {
	uint8_t format;
	uint64_t size;
	uint64_t width;
	uint64_t height;
} texture_data;

typedef struct {
	resource_header header;
	texture_data texture;
} texture_resource;
