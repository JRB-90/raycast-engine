#pragma once

/*
	Functions for dealing with resources (textures etc).
	Provides save/load and file handling for all resource types.
*/

#include <stdio.h>
#include <stdint.h>
#include "engine_color.h"

#define RES_HEADER_SIZE		24
#define TEXTURE_DATA_SIZE	36

#define MAX_TEXTURES		128

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

typedef struct {
	texture_resource* textures[MAX_TEXTURES];
	texture_resource* texturesDark[MAX_TEXTURES];
	texture_resource* texturesLight[MAX_TEXTURES];
} scene_resources;

/// <summary>
/// Directly loads a texture from disk and puts it in the suplied resource.
/// </summary>
/// <param name="path">Path to texture file.</param>
/// <param name="texture">Resource to store the texture into.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int resource_load_texture_direct(
	const char* const path,
	texture_resource* const texture
);

/// <summary>
/// Loads a texture from disk and assigns it to the given texture slot.
/// </summary>
/// <param name="resources">Scenes resource holder to add the texture to.</param>
/// <param name="path">Path to the texture.</param>
/// <param name="textureID">Texture slot to assign the texture to.</param>
/// <param name="format">Color format to convert the texture to.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int resource_load_texture_res(
	scene_resources* const resources,
	const char* const path,
	int textureID,
	colformat format
);

/// <summary>
/// Destroys the provided texture resource.
/// </summary>
/// <param name="texture">Texture resource to destroy.</param>
extern void resource_destroy_direct_texture(texture_resource* texture);

/// <summary>
/// Destroys a loaded texture resource.
/// </summary>
/// <param name="resources">Scene resource holder that contains the texture.</param>
/// <param name="textureID">Texture slot the texture is residing in.</param>
extern void resource_destroy_loaded_texture(
	const scene_resources* const resources,
	int textureID
);

/// <summary>
/// Saves a texture to disk.
/// </summary>
/// <param name="path">Path to the desired save location.</param>
/// <param name="texture"></param>
/// <returns>Non-zero if error occurred.</returns>
extern int resource_save_texture(
	const char* const path,
	const texture_resource* const texture
);
