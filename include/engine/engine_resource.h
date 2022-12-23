#pragma once

#include <stdio.h>
#include <stdint.h>
#include "engine_color.h"

#define RES_HEADER_SIZE		24
#define TEXTURE_DATA_SIZE	36

typedef enum {
	RES_BLOB = 1,
	RES_TEXTURE = 2
} resource_type;

typedef struct {
	uint32_t type;
	uint32_t id;
	uint64_t fileOffset;
	uint64_t size;
} resource_header;

typedef struct {
	resource_header header;
	uint8_t* data;
} blob_resource;

typedef struct {
	uint32_t format;
	uint64_t sizeInPixels;
	uint64_t sizeInBytes;
	uint64_t width;
	uint64_t height;
	void* pixels;
} texture_data;

typedef struct {
	resource_header header;
	texture_data texture;
} texture_resource;

int save_texture(
	const char* const path,
	const texture_resource* const texture
);

int write_uint8(FILE* const file, uint8_t value);
int read_uint8(FILE* const file, uint8_t* const value);
int write_uint16(FILE* const file, uint16_t value);
int read_uint16(FILE* const file, uint16_t* const value);
int write_uint32(FILE* const file, uint32_t value);
int read_uint32(FILE* const file, uint32_t* const value);
int write_uint64(FILE* const file, uint64_t value);
int read_uint64(FILE* const file, uint64_t* const value);
