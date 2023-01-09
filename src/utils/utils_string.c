#include "utils/utils_string.h"

char* substring(
	char* destination,
	const char* source,
	int start,
	int len)
{
	strncpy(destination, (source + start), len);
	destination[len] = '\0';

	return destination;
}