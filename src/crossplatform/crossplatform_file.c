#include "crossplatform/crossplatform_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils/utils_string.h"

int process_dir_recursively(
	const char* dirPath, 
	int (*on_file)(const char* const))
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
				if (process_dir_recursively(path, on_file))
				{
					return -1;
				}
			}
			else
			{
				if (on_file(path))
				{
					return -1;
				}
			}
		}
	} while (FindNextFile(file, &findData));

	FindClose(file);

	return 0;
}

int get_filename_ext(
	const char* path,
	char* filename,
	char* ext)
{
	int lastSeperatorIndex = -1;
	int lastDotIndex = -1;
	int index = 0;
	char currentChar;

	do
	{
		currentChar = path[index];

		if (currentChar == '\\' ||
			currentChar == '/')
		{
			lastSeperatorIndex = index;
		}

		if (currentChar == '.')
		{
			lastDotIndex = index;
		}

		index++;
	} while (currentChar != '\0');

	if (lastSeperatorIndex < 0 ||
		lastDotIndex < 0 ||
		lastDotIndex < lastSeperatorIndex)
	{
		return -1;
	}

	filename =
		substring(
			filename,
			path,
			lastSeperatorIndex + 1,
			lastDotIndex - lastSeperatorIndex - 1
		);

	ext =
		substring(
			ext,
			path,
			lastDotIndex + 1,
			index - lastDotIndex - 1
		);

	int i = 0;
	while (ext[i] != '\0')
	{
		ext[i] = toupper(ext[i]);
		i++;
	}

	return 0;
}
