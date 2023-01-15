#pragma once

/// <summary>
/// Recursively walks through a directory and calls a callback for each
/// non-dir file found.
/// </summary>
/// <param name="dirPath">Path to the directory to walk.</param>
/// <param name="on_file">Callback used to receive each file as it's found.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int process_dir_recursively(
	const char* dirPath, 
	int (*on_file)(const char* const)
);

/// <summary>
/// Extracts the filename and extension from a given file.
/// </summary>
/// <param name="path">File to extract from.</param>
/// <param name="filename">Buffer to put the extracted filename in.</param>
/// <param name="ext">Buffer to put the extracted extension in.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int get_filename_ext(
	const char* path,
	char* filename,
	char* ext
);
