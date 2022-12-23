#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"

int main(int argc, char** argv)
{
	printf("Loading file...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		fprintf(stderr, "Failed to init SDL\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	
	SDL_Surface* img = IMG_Load("../../../../data/textures/brick/brick_64.bmp");

	if (img == NULL)
	{
		fprintf(stderr, "Failed to load image\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	printf("File loaded successfully\n");
	printf("Processing image...\n");


	printf("Processing complete, now exiting...\n");
	SDL_FreeSurface(img);
	SDL_Quit();

	getchar();
	exit(EXIT_SUCCESS);
}