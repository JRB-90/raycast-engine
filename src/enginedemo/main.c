#include <stdio.h>
#include "engine/engine_types.h"

int main(int argc, char** argv)
{
	test_t test = create_test_t(42);
	printf("%d\n", test.value);
	getchar();
}