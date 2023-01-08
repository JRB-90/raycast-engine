#pragma once

extern int process_dir_recursively(
	const char* dirPath, 
	int (*on_file)(const char* const)
);

extern int get_filename_ext(
	const char* path,
	char* filename,
	char* ext
);

extern char* substring(
	char* destination,
	const char* source, 
	int start, 
	int len
);