#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "engine/engine_resource.h"
#include "crossplatform/crossplatform_file.h"
#include "crossplatform/crossplatform_time.h"

const char* IN_DATA_DIR = "../../../../raw/";
const char* OUT_DATA_DIR = "../../../../data/textures/";

int process_file(const char* path);
int convert_and_save(const char* input, const char* output);
bool is_compatible_file(const char* ext);
bool is_supported_format(const SDL_PixelFormat* const format);

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		fprintf(stderr, "Failed to init SDL\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	int res = process_dir_recursively(IN_DATA_DIR, &process_file);

	SDL_Quit();

	getchar();
	exit(res == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

int process_file(const char* path)
{
	char filename[256];
	char ext[32];

	int res = 
		get_filename_ext(
			path, 
			&filename,
			&ext
		);
	
	if (res)
	{
		return res;
	}

	if (!is_compatible_file(ext))
	{
		fprintf(stderr, "Incorrect file type: %s\n", ext);
		return -1;
	}

	char outputPath[1024] = "";
	strcat(outputPath, OUT_DATA_DIR);
	strcat(outputPath, filename);
	strcat(outputPath, ".rtx");

	res =
		convert_and_save(
			path,
			outputPath
		);

	return res;
}

int convert_and_save(
	const char* input, 
	const char* output)
{
	printf("Loading file %s...\n", input);

	SDL_Surface* origImg = IMG_Load(input);

	if (origImg == NULL)
	{
		fprintf(stderr, "Failed to load image\n");
		
		return -1;
	}

	printf("File loaded successfully\n");
	printf("Processing image...\n");

	if (!is_supported_format(origImg->format))
	{
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "File color format not supported\n");
		
		return -1;
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
		
		return -1;
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
		
		return -1;
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

	if (resource_save_texture(output, &textureRes))
	{
		SDL_FreeSurface(convImg);
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "Failed to save texture to disk\n");
		
		return -1;
	}

	printf("Texture saved to disk\n");
	printf("Loading texture from disk...\n");

	texture_resource loadedTexture;
	if (resource_load_texture_direct(output, &loadedTexture))
	{
		SDL_FreeSurface(convImg);
		SDL_FreeSurface(origImg);
		SDL_Quit();
		fprintf(stderr, "Failed to load texture from disk\n");

		return -1;
	}

	printf("Texture loaded successfully\n");

	printf("Processing complete, now exiting...\n");
	free(texture.pixels);
	SDL_FreeSurface(convImg);
	SDL_FreeSurface(origImg);

	return 0;
}

bool is_compatible_file(const char* ext)
{
	if (strcmp(ext, "PNG"))
	{
		return true;
	}
	else if (strcmp(ext, "BMP"))
	{
		return true;
	}

	return false;
}

bool is_supported_format(const SDL_PixelFormat* const sdlFormat)
{
	uint32_t format = sdlFormat->format;

	if (format == SDL_PIXELFORMAT_UNKNOWN)
	{
		format =
			SDL_MasksToPixelFormatEnum(
				sdlFormat->BitsPerPixel,
				sdlFormat->Rmask,
				sdlFormat->Gmask,
				sdlFormat->Bmask,
				sdlFormat->Amask
			);
	}

	if (format == SDL_PIXELFORMAT_ARGB32)
	{
		return true;
	}
	else if (format == SDL_PIXELFORMAT_ARGB8888)
	{
		return true;
	}
	else if (format == SDL_PIXELFORMAT_ABGR8888)
	{
		return true;
	}
	else if (format == SDL_PIXELFORMAT_RGB24)
	{
		return true;
	}
	else if (format == SDL_PIXELFORMAT_RGB888)
	{
		return true;
	}
	else if (format == SDL_PIXELFORMAT_BGR24)
	{
		return true;
	}
	else if (format == SDL_PIXELFORMAT_BGR888)
	{
		return true;
	}
	else
	{
		char* formatString = SDL_GetPixelFormatName(format);
		fprintf(stderr, "Pixel format: %s, not supported\n", formatString);

		return false;
	}
}
