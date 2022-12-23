#include "engine_resource.h"

int save_texture(
	const char* const path,
	const texture_resource* const texture)
{
	FILE* file = fopen(path, "w");
	
	// TODO

	fclose(file);
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
