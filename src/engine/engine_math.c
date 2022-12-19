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

vec2d transform_vec2(
	const vec2d* const point,
	const frame2d* const frame)
{
	vec2d res = { 0.0f, 0.0f };

	res.x = 
		(point->x * cosf(frame->theta)) - 
		(point->y * sinf(frame->theta));

	res.y = 
		(point->x * sinf(frame->theta)) +
		(point->y * cosf(frame->theta));

	res.x += frame->x;
	res.y += frame->y;

	return res;
}

frame2d inverse_frame2d(const frame2d* const frame)
{
	// TODO
	return to_frame2d(0, 0, 0);
}

vec2d calc_forwards(
	const frame2d* const frame,
	const vec2d* const worldForward)
{
	vec2d lookVector =
	{
		.x = (worldForward->x * cosf(frame->theta)) - (worldForward->y * sinf(frame->theta)),
		.y = (worldForward->y * cosf(frame->theta)) + (worldForward->x * sinf(frame->theta))
	};

	lookVector = norm_vec(&lookVector);

	return lookVector;
}

void print_vec2d(const vec2d* const vec)
{
	printf("vec2d [%.3f, %.3f]\n", vec->x, vec->y);
}

void print_frame2d(const frame2d* const frame)
{
	printf("frame2d [%.3f, %.3f, %.3f]\n", frame->x, frame->y, frame->theta);
}