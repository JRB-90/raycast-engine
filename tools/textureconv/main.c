#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "engine/engine_resource.h"
#include "time/time_helper.h"

void convert_and_save(const char* input, const char* output);
bool is_supported_format(const SDL_PixelFormat* const format);

int main(int argc, char** argv)
{
	convert_and_save(
		"../../../../data/textures/brick/brick_64.bmp",
		"../../../../data/textures/brick/brick_64.rtx"
	);

	convert_and_save(
		"../../../../data/textures/brick/brick_128.bmp",
		"../../../../data/textures/brick/brick_128.rtx"
	);

	convert_and_save(
		"../../../../data/textures/brick/brick_256.bmp",
		"../../../../data/textures/brick/brick_256.rtx"
	);

	convert_and_save(
		"../../../../data/textures/concrete/concrete_64.bmp",
		"../../../../data/textures/concrete/concrete_64.rtx"
	);

	convert_and_save(
		"../../../../data/textures/metal/metal_64.bmp",
		"../../../../data/textures/metal/metal_64.rtx"
	);

	convert_and_save(
		"../../../../data/sprites/static/column/column_64.bmp",
		"../../../../data/sprites/static/column/column_64.rtx"
	);

	convert_and_save(
		"../../../../data/sprites/static/lamp/lamp_64.bmp",
		"../../../../data/sprites/static/lamp/lamp_64.rtx"
	);

	getchar();
	exit(EXIT_SUCCESS);
}

void convert_and_save(
	const char* input, 
	const char* output)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		fprintf(stderr, "Failed to init SDL\n");
		getchar();
		exit(EXIT_FAILURE);
	}

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
	SDL_Quit();

	//sleep_secs(1);
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
