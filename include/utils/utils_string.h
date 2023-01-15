#pragma once

/// <summary>
/// Extracts a sub-string from a given string.
/// </summary>
/// <param name="destination">The buffer to put the sub-string in.</param>
/// <param name="source">The original string.</param>
/// <param name="start">The start index of the sub-string.</param>
/// <param name="len">The end index of the sub-string.</param>
/// <returns>Non-zero if error occurred.</returns>
extern char* substring(
	char* destination,
	const char* source,
	int start,
	int len
);