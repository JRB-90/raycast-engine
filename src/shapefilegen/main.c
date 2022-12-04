#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* LINE_FILE = "lines.dat";
const char* RECT_FILE = "rects.dat";
const int WIDTH = 640;
const int HEIGHT = 480;
const int NUM_LINES = 100;
const int NUM_RECTS = 100;

typedef enum {
	line = 1,
	rect = 2
} shape_type;

void write_4param_data(
	FILE* file, 
	int type, 
	int p1,
	int p2,
	int p3,
	int p4
);

int main(int argc, char** argv)
{
	printf("Generating new line positions...\n");

	FILE* lineFile = fopen(LINE_FILE, "w+");

	if (lineFile == NULL)
	{
		fprintf(stderr, "Failed to create/open lines file, now exiting...\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	for (int i = 0; i < NUM_LINES; i++)
	{
		write_4param_data(
			lineFile,
			(int)line,
			rand() % WIDTH,
			rand() % HEIGHT,
			rand() % WIDTH,
			rand() % HEIGHT
		);
	}

	fclose(lineFile);

	printf("Generating new rect positions...\n");

	FILE* rectFile = fopen(RECT_FILE, "w+");

	if (rectFile == NULL)
	{
		fprintf(stderr, "Failed to create/open lines file, now exiting...\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	for (int i = 0; i < NUM_RECTS; i++)
	{
		int x = rand() % WIDTH;
		int w = rand() % (WIDTH - x);
		int y = rand() % HEIGHT;
		int h = rand() % (HEIGHT - y);

		write_4param_data(
			rectFile,
			(int)rect,
			x,
			y,
			w,
			h
		);
	}

	fclose(rectFile);

	printf("Generation complete\n");

	getchar();

	return EXIT_SUCCESS;
}

void write_4param_data(
	FILE* file,
	int type,
	int p1,
	int p2,
	int p3,
	int p4)
{
	int paramCount = 4;
	fwrite(&paramCount, 1, sizeof(paramCount), file);
	fwrite(&type, 1, sizeof(type), file);
	fwrite(&p1, 1, sizeof(p1), file);
	fwrite(&p2, 1, sizeof(p2), file);
	fwrite(&p3, 1, sizeof(p3), file);
	fwrite(&p4, 1, sizeof(p4), file);
}