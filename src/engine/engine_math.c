#include "engine_math.h"
#include <stdio.h>
#include <assert.h>

vec2d norm_vec(const vec2d* const vec)
{
	float len = len_vec(vec);

	assert(len != 0.0);

	vec2d normalisedVec =
	{
		.x = vec->x / len,
		.y = vec->y / len,
	};

	return normalisedVec;
}

void print_vec2d(const vec2d* const vec)
{
	printf("vec2d [%.3f, %.3f]\n", vec->x, vec->y);
}

void print_frame2d(const frame2d* const frame)
{
	printf("frame2d [%.3f, %.3f, %.3f]\n", frame->x, frame->y, frame->theta);
}