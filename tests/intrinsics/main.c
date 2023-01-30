#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xmmintrin.h>

int main(int argc, char** argv)
{
	int32_t vals1[] =
	{
		1,
		2,
		3,
		4,
		5
	};

	int32_t vals2[] =
	{
		1,
		1,
		1,
		1,
		1
	};

	__m128 a = _mm_load_ps(&vals1[1]);
	__m128 b = _mm_load_ps(&vals2[0]);

	__m128 res = _mm_add_ps(a, b);

	int32_t r1 = res.m128_i32[0];
	int32_t r2 = res.m128_i32[1];
	int32_t r3 = res.m128_i32[2];
	int32_t r4 = res.m128_i32[3];

	getchar();
	exit(EXIT_SUCCESS);
}