#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* LINE_FILE = "../../../../data/shapefiles/lines.dat";
const char* RECT_FILE = "../../../../data/shapefiles/rects.dat";
const int WIDTH = 640;
const int HEIGHT = 480;
const int NUM_LINES = 1000000;
const int NUM_RECTS = 1000000;

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

	FILE* lineFile = NULL;
	int err = fopen_s(&lineFile, LINE_FILE, "r");

	if (err != 0)
	{
		fprintf(stderr, "Failed to create/open lines file, now exiting...\n");
		int c = getchar();
		exit(EXIT_FAILURE);
	}

	srand((unsigned int)time(NULL));

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

	FILE* rectFile = NULL;
	err = fopen_s(&rectFile, RECT_FILE, "r");

	if (err != 0)
	{
		fprintf(stderr, "Failed to create/open rect file, now exiting...\n");
		int c = getchar();
		exit(EXIT_FAILURE);
	}

	srand((unsigned int)time(NULL));

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

	int c = getchar();

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