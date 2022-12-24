#include "engine_resource.h"

#include <stdlib.h>

int save_texture(
	const char* const path,
	const texture_resource* const texture)
{
	FILE* file = fopen(path, "w");
	
	write_resource_header(file, &texture->header);

	write_uint32(file, texture->texture.format);
	write_uint64(file, texture->texture.sizeInPixels);
	write_uint64(file, texture->texture.sizeInBytes);
	write_uint64(file, texture->texture.width);
	write_uint64(file, texture->texture.height);

	uint8_t* data = (uint8_t*)texture->texture.pixels;
	for (int i = 0; i < texture->texture.sizeInBytes; i++)
	{
		write_uint8(file, data[i]);
	}

	fclose(file);
}

int load_texture(
	const char* const path,
	texture_resource* const texture)
{
	FILE* file = fopen(path, "r");

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
			return -1;
		}
		else
		{
			data[i] = value;
		}
	}

	fclose(file);

	return res;
}

void destroy_texture(texture_resource* texture)
{
	free(texture->texture.pixels);
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
	fwrite(&buf, sizeof(uint8_t), 1, file);

	return 0;
}

int read_uint8(FILE* const file, uint8_t* const value)
{
	uint8_t buf[1];
	if (fread(&buf, sizeof(uint8_t), 1, file))
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
	fwrite(&buf, sizeof(uint16_t), 1, file);

	return 0;
}

int read_uint16(FILE* const file, uint16_t* const value)
{
	uint16_t buf[1];
	if (fread(&buf, sizeof(uint16_t), 1, file))
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
	fwrite(&buf, sizeof(uint32_t), 1, file);

	return 0;
}

int read_uint32(FILE* const file, uint32_t* const value)
{
	uint32_t buf[1];
	if (fread(&buf, sizeof(uint32_t), 1, file))
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
	fwrite(&buf, sizeof(uint64_t), 1, file);

	return 0;
}

int read_uint64(FILE* const file, uint64_t* const value)
{
	uint64_t buf[1];
	if (fread(&buf, sizeof(uint64_t), 1, file))
	{
		*value = buf[0];

		return 0;
	}
	else
	{
		return -1;
	}
}
