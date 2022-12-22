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

vec2d transform_vec2_by_frame2d(
	const vec2d* const point,
	const frame2d* const frame)
{
	vec2d res = { 0.0f, 0.0f };

	float x2 = point->x + frame->x;
	float y2 = point->y + frame->y;

	res.x =
		(x2 * cosf(frame->theta)) -
		(y2 * sinf(frame->theta));

	res.y =
		(x2 * sinf(frame->theta)) +
		(y2 * cosf(frame->theta));

	return res;
}

vec2d transform_vec2_by_frame2d_inv(
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

	res.x -= frame->x;
	res.y -= frame->y;

	return res;
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

vec2d calc_forwards_trans(
	const transform2d* const transform,
	const vec2d* const worldForward)
{
	vec2d lookVector =
	{
		.x = 
			(worldForward->x * cosf(transform->rot)) -
			(worldForward->y * sinf(transform->rot)),
		.y = 
			(worldForward->y * cosf(transform->rot)) +
			(worldForward->x * sinf(transform->rot))
	};

	lookVector = norm_vec(&lookVector);

	return lookVector;
}

vec2d transform_vec2(
	const vec2d* const point,
	const transform2d* const transform)
{
	vec2d res =
	{
		.x = point->x * transform->scale.x,
		.y = point->y * transform->scale.y
	};

	float x2 =
		(res.x * cosf(transform->rot)) -
		(res.y * sinf(transform->rot));

	float y2 =
		(res.x * sinf(transform->rot)) +
		(res.y * cosf(transform->rot));

	res.x = x2 + transform->trans.x;
	res.y = y2 + transform->trans.y;

	return res;
}

vec2d transform_vec2_inv(
	const vec2d* const point,
	const transform2d* const transform)
{
	vec2d res =
	{
		.x = point->x - transform->trans.x,
		.y = point->y - transform->trans.y
	};

	float x2 =
		(res.x * cosf(-transform->rot)) -
		(res.y * sinf(-transform->rot));

	float y2 =
		(res.x * sinf(-transform->rot)) +
		(res.y * cosf(-transform->rot));

	res.x = x2 * (1.0f / transform->scale.x);
	res.y = y2 * (1.0f / transform->scale.y);

	return res;
}

void print_vec2d(const vec2d* const vec)
{
	printf("vec2d [%.3f, %.3f]\n", vec->x, vec->y);
}

void print_frame2d(const frame2d* const frame)
{
	printf("frame2d [%.3f, %.3f, %.3f]\n", frame->x, frame->y, frame->theta);
}