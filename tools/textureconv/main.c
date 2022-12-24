#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"
#include "SDL_image.h"
#include "engine_resource.h"

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

	SDL_Surface* tempSurface =
		SDL_CreateRGBSurfaceWithFormat(
			0,
			0,
			0,
			32,
			SDL_PIXELFORMAT_ARGB32
		);

	if (tempSurface == NULL)
	{
		SDL_Quit();
		fprintf(stderr, "Failed to create temp surface\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	SDL_PixelFormat outputFormat = *tempSurface->format;
	SDL_FreeSurface(tempSurface);

	SDL_Surface* img = IMG_Load(input);

	if (img == NULL)
	{
		fprintf(stderr, "Failed to load image\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("File loaded successfully\n");
	printf("Processing image...\n");

	if (!is_supported_format(img->format))
	{
		SDL_FreeSurface(img);
		SDL_Quit();
		fprintf(stderr, "File color format not supported\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	texture_data texture =
	{
		.format = CF_ARGB,
		.width = img->w,
		.height = img->h,
		.sizeInPixels = img->w * img->h,
		.sizeInBytes = img->w * img->h * 4,
		.pixels = malloc(img->w * img->h * 4)
	};

	if (texture.pixels == NULL)
	{
		SDL_FreeSurface(img);
		SDL_Quit();
		fprintf(stderr, "Failed to malloc texture pixels\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	uint32_t* imgPixels = (uint32_t*)img->pixels;
	uint32_t* texPixels = (uint32_t*)texture.pixels;

	for (int i = 0; i < texture.sizeInPixels; i++)
	{
		uint8_t a;
		uint8_t r;
		uint8_t g;
		uint8_t b;

		SDL_GetRGBA(imgPixels[i], img->format, &r, &g, &b, &a);
		texPixels[i] = SDL_MapRGBA(&outputFormat, r, g, b, a);
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
		SDL_FreeSurface(img);
		SDL_Quit();
		fprintf(stderr, "Failed to save texture to disk\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("Texture saved to disk\n");
	printf("Loading texture from disk...\n");

	texture_resource loadedTexture;
	if (load_texture("../../../../data/textures/brick/brick_64.rtx", &loadedTexture))
	{
		SDL_FreeSurface(img);
		SDL_Quit();
		fprintf(stderr, "Failed to load texture from disk\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("Texture loaded successfully\n");

	printf("Processing complete, now exiting...\n");
	destroy_texture(&loadedTexture);
	free(texture.pixels);
	SDL_FreeSurface(img);
	SDL_Quit();
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
