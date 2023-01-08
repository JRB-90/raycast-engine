#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "SDL.h"
#include "SDL_image.h"
#include "engine/engine_resource.h"
#include "crossplatform/crossplatform_time.h"

const char* IN_DATA_DIR = "../../../../raw/";
const char* OUT_DATA_DIR = "../../../../data/textures/";

int process_dir(const char* dirPath);
int process_file(const char* path, const char* outputDir);
void convert_and_save(const char* input, const char* output);
bool is_supported_format(const SDL_PixelFormat* const format);

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		fprintf(stderr, "Failed to init SDL\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	int res = process_dir(IN_DATA_DIR);

	SDL_Quit();

	getchar();
	exit(res == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

int process_dir(const char* dirPath)
{
	WIN32_FIND_DATA findData;
	HANDLE file = NULL;

	char path[2048];
	sprintf(path, "%s\\*.*", dirPath);

	if ((file = FindFirstFile(path, &findData)) == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Path not found: %s\n", dirPath);
		return -1;
	}

	do
	{
		if (strcmp(findData.cFileName, ".") != 0 &&
			strcmp(findData.cFileName, "..") != 0)
		{
			sprintf(path, "%s\\%s", dirPath, findData.cFileName);

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("Entering dir: %s\n", path);
				if (process_dir(path))
				{
					return -1;
				}
			}
			else
			{
				printf("Processing file: %s\n", path);
				process_file(path, OUT_DATA_DIR);
			}
		}
	} while (FindNextFile(file, &findData));

	FindClose(file);

	return 0;
}

int process_file(const char* path, const char* outputDir)
{
	char filename[1024];
	char ext[1024];

	// TODO - Extract ext and filename from path
	//        then convert to multiple versions
	//        and save to disk
}

void convert_and_save(
	const char* input, 
	const char* output)
{
	printf("Loading file %s...\n", input);

	SDL_Surface* origImg = IMG_Load(input);

	if (origImg == NULL)
	{
		fprintf(stderr, "Failed to load image\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("File loaded successfully\n");
	printf("Processing image...\n");

	if (!is_supported_format(origImg->format))
	{
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "File color format not supported\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	texture_data texture =
	{
		.format = CF_ARGB,
		.width = origImg->w,
		.height = origImg->h,
		.sizeInPixels = origImg->w * origImg->h,
		.sizeInBytes = origImg->w * origImg->h * 4,
		.pixels = malloc(origImg->w * origImg->h * 4)
	};

	if (texture.pixels == NULL)
	{
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "Failed to malloc texture pixels\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	SDL_Surface* convImg =
		SDL_ConvertSurfaceFormat(
			origImg,
			SDL_PIXELFORMAT_ARGB32,
			0
		);

	if (convImg == NULL)
	{
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "Failed to malloc texture pixels\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	memcpy(texture.pixels, convImg->pixels, texture.sizeInBytes);

	uint8_t* imgPixels = (uint8_t*)convImg->pixels;
	uint8_t* texPixels = (uint8_t*)texture.pixels;

	for (int i = 0; i < texture.sizeInBytes; i += 4)
	{
		texPixels[i + 0] = imgPixels[i + 3];
		texPixels[i + 1] = imgPixels[i + 2];
		texPixels[i + 2] = imgPixels[i + 1];
		texPixels[i + 3] = imgPixels[i + 0];
	}

	size_t sizeOfTextureBlob = sizeof(texture_data) - sizeof(void*);

	texture_resource textureRes =
	{
		.header = (resource_header)
		{
			.type = RES_TEXTURE,
			.id = 1,
			.fileOffset = 0,
			.size = sizeOfTextureBlob
		},
		.texture = texture
	};

	printf("Conversion successful, saving to disk...\n");

	if (save_texture(output, &textureRes))
	{
		SDL_FreeSurface(convImg);
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "Failed to save texture to disk\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("Texture saved to disk\n");
	printf("Loading texture from disk...\n");

	texture_resource loadedTexture;
	if (load_texture(output, &loadedTexture))
	{
		SDL_FreeSurface(convImg);
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "Failed to load texture from disk\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("Texture loaded successfully\n");

	printf("Processing complete, now exiting...\n");
	free(texture.pixels);
	SDL_FreeSurface(convImg);
	SDL_FreeSurface(origImg);
}

bool is_supported_format(const SDL_PixelFormat* const format)
{
	if (format->format == SDL_PIXELFORMAT_ARGB32)
	{
		return true;
	}
	else if (format->format == SDL_PIXELFORMAT_ARGB8888)
	{
		return true;
	}
	else if (format->format == SDL_PIXELFORMAT_RGB24)
	{
		return true;
	}
	else if (format->format == SDL_PIXELFORMAT_RGB888)
	{
		return true;
	}
	else if (format->format == SDL_PIXELFORMAT_BGR24)
	{
		return true;
	}
	else if (format->format == SDL_PIXELFORMAT_BGR888)
	{
		return true;
	}
	else
	{
		return false;
	}
}
