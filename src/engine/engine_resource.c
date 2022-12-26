#include "engine_resource.h"

#include <stdlib.h>

int create_texture_resources(
	scene_resources* const resources,
	const char* const path,
	int textureID,
	colformat format)
{
	texture_resource* loadedTexture = malloc(sizeof(texture_resource));

	if (loadedTexture == NULL)
	{
		fprintf(stderr, "Failed to malloc loaded texture\n");
		return -1;
	}

	if (load_texture(path, loadedTexture))
	{
		fprintf(stderr, "Failed to load texture\n");
		free(loadedTexture);
		return -1;
	}

	resources->textures[textureID] =
		create_new_texture_variant(
			loadedTexture,
			format,
			1.0f
		);

	if (resources->textures[textureID] == NULL)
	{
		fprintf(stderr, "Failed to create dark texture variant\n");
		destroy_texture(loadedTexture);

		return -1;
	}
	
	resources->texturesDark[textureID] =
		create_new_texture_variant(
			loadedTexture,
			format,
			0.5f
		);

	if (resources->texturesDark[textureID] == NULL)
	{
		fprintf(stderr, "Failed to create dark texture variant\n");
		destroy_texture(loadedTexture);
		destroy_texture(resources->textures[textureID]);

		return -1;
	}

	resources->texturesLight[textureID] =
		create_new_texture_variant(
			loadedTexture,
			format,
			1.2f
		);

	if (resources->texturesLight[textureID] == NULL)
	{
		fprintf(stderr, "Failed to create light texture variant\n");
		destroy_texture(loadedTexture);
		destroy_texture(loadedTexture);
		destroy_texture(resources->textures[textureID]);
		destroy_texture(resources->texturesDark[textureID]);

		return -1;
	}

	destroy_texture(loadedTexture);

	return 0;
}

void destroy_texture_resources(
	const scene_resources* const resources,
	int textureID)
{
	free(resources->textures[textureID]);
	free(resources->texturesDark[textureID]);
	free(resources->texturesLight[textureID]);
}

texture_resource* create_new_texture_variant(
	const texture_resource* const texture,
	colformat newFormat,
	float scale)
{
	texture_resource* scaledTexture = malloc(sizeof(texture_resource));

	if (scaledTexture == NULL)
	{
		return NULL;
	}

	scaledTexture->header = texture->header;
	scaledTexture->texture = texture->texture;

	if (newFormat == CF_RGB565)
	{
		scaledTexture->texture.pixels = 
			malloc(texture->texture.sizeInPixels * 2);
	}
	else
	{
		scaledTexture->texture.pixels = 
			malloc(texture->texture.sizeInPixels * 4);
	}

	if (scaledTexture->texture.pixels == NULL)
	{
		free(scaledTexture);

		return NULL;
	}
	
	uint32_t* origPixels = (uint32_t*)texture->texture.pixels;

	for (int i = 0; i < scaledTexture->texture.sizeInPixels; i++)
	{
		uint8_t a = (origPixels[i] & 0xFF000000) >> 24;
		uint8_t r = (origPixels[i] & 0x00FF0000) >> 16;
		uint8_t g = (origPixels[i] & 0x0000FF00) >> 8;
		uint8_t b = (origPixels[i] & 0x000000FF) >> 0;

		color scaledColor = 
			to_col(
				a, 
				(int)((float)r * scale),
				(int)((float)g * scale),
				(int)((float)b * scale)
			);

		if (newFormat == CF_ARGB)
		{
			((uint32_t*)scaledTexture->texture.pixels)[i] =
				to_argb(&scaledColor);
		}
		else if (newFormat == CF_RGB565)
		{
			((uint16_t*)scaledTexture->texture.pixels)[i] = 
				to_rgb565(&scaledColor);
		}
	}

	return scaledTexture;
}

int save_texture(
	const char* const path,
	const texture_resource* const texture)
{
	FILE* file = fopen(path, "wb");

	if (file == NULL)
	{
		fprintf(stderr, "Failed open file %s\n", path);
		return -1;
	}
	
	write_resource_header(file, &texture->header);

	write_uint32(file, texture->texture.format);
	write_uint64(file, texture->texture.sizeInPixels);
	write_uint64(file, texture->texture.sizeInBytes);
	write_uint64(file, texture->texture.width);
	write_uint64(file, texture->texture.height);

	uint8_t* data = (uint8_t*)texture->texture.pixels;
	for (int i = 0; i < texture->texture.sizeInBytes; i++)
	{
		if (write_uint8(file, data[i]))
		{
			return -1;
		}
	}

	fflush(file);
	fclose(file);
}

int load_texture(
	const char* const path,
	texture_resource* const texture)
{
	FILE* file = fopen(path, "rb");

	if (file == NULL)
	{
		fprintf(stderr, "Failed open file %s\n", path);
		return -1;
	}

	int res = read_resource_header(file, &texture->header);
	res |= read_uint32(file, &texture->texture.format);
	res |= read_uint64(file, &texture->texture.sizeInPixels);
	res |= read_uint64(file, &texture->texture.sizeInBytes);
	res |= read_uint64(file, &texture->texture.width);
	res |= read_uint64(file, &texture->texture.height);

	if (res)
	{
		return -1;
	}

	texture->texture.pixels = malloc(texture->texture.sizeInBytes);

	if (texture->texture.pixels == NULL)
	{
		return -1;
	}

	uint8_t* data = (uint8_t*)texture->texture.pixels;

	for (int i = 0; i < texture->texture.sizeInBytes; i++)
	{
		uint8_t value;
		if (read_uint8(file, &value))
		{
			int err = ferror(file);

			return -1;
		}
		else
		{
			data[i] = value;
		}
	}

	fflush(file);
	fclose(file);

	return res;
}

void destroy_texture(texture_resource* texture)
{
	if (texture != NULL)
	{
		if (texture->texture.pixels != NULL)
		{
			free(texture->texture.pixels);
		}
		free(texture);
	}
}

int write_resource_header(
	FILE* const file,
	const resource_header* const header)
{
	write_uint32(file, header->type);
	write_uint32(file, header->id);
	write_uint64(file, header->fileOffset);
	write_uint64(file, header->size);

	return 0;
}

int read_resource_header(
	FILE* const file,
	resource_header* const header)
{
	int res = 0;
	res |= read_uint32(file, &header->type);
	res |= read_uint32(file, &header->id);
	res |= read_uint64(file, &header->fileOffset);
	res |= read_uint64(file, &header->size);

	return res;
}

int write_uint8(FILE* const file, uint8_t value)
{
	uint8_t buf[1];
	buf[0] = value;
	if (fwrite(&buf, sizeof(uint8_t), 1, file) == 1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int read_uint8(FILE* const file, uint8_t* const value)
{
	uint8_t buf[1];
	if (fread(&buf, sizeof(uint8_t), 1, file) == 1)
	{
		*value = buf[0];

		return 0;
	}
	else
	{
		return -1;
	}
}

int write_uint16(FILE* const file, uint16_t value)
{
	uint16_t buf[1];
	buf[0] = value;
	if (fwrite(&buf, sizeof(uint16_t), 1, file) == 1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int read_uint16(FILE* const file, uint16_t* const value)
{
	uint16_t buf[1];
	if (fread(&buf, sizeof(uint16_t), 1, file) == 1)
	{
		*value = buf[0];

		return 0;
	}
	else
	{
		return -1;
	}
}

int write_uint32(FILE* const file, uint32_t value)
{
	uint32_t buf[1];
	buf[0] = value;
	if (fwrite(&buf, sizeof(uint32_t), 1, file) == 1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int read_uint32(FILE* const file, uint32_t* const value)
{
	uint32_t buf[1];
	if (fread(&buf, sizeof(uint32_t), 1, file) == 1)
	{
		*value = buf[0];

		return 0;
	}
	else
	{
		return -1;
	}
}

int write_uint64(FILE* const file, uint64_t value)
{
	uint64_t buf[1];
	buf[0] = value;
	if (fwrite(&buf, sizeof(uint64_t), 1, file) == 1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int read_uint64(FILE* const file, uint64_t* const value)
{
	uint64_t buf[1];
	if (fread(&buf, sizeof(uint64_t), 1, file) == 1)
	{
		*value = buf[0];

		return 0;
	}
	else
	{
		return -1;
	}
}
