#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

typedef struct {
	float x;
	float y;
} vec2d;

typedef struct {
	float x;
	float y;
	float theta;
} frame2d;

typedef struct {
	vec2d trans;
	vec2d scale;
	float rot;
} transform2d;

static inline vec2d vec2d_build(float x, float y)
{
	vec2d vec =
	{
		.x = x,
		.y = y
	};

	return vec;
}

static inline frame2d frame2d_build(float x, float y, float theta)
{
	frame2d frame =
	{
		.x = x,
		.y = y,
		.theta = theta
	};

	return frame;
}

static inline vec2d vec2d_add(const vec2d* const v1, const vec2d* const v2)
{
	vec2d res =
	{
		.x = v1->x + v2->x,
		.y = v1->y + v2->y,
	};

	return res;
}

static inline vec2d vec2d_sub(const vec2d* const v1, const vec2d* const v2)
{
	vec2d res =
	{
		.x = v1->x - v2->x,
		.y = v1->y - v2->y,
	};

	return res;
}

static inline vec2d vec2d_mul(const vec2d* const v1, float scalar)
{
	vec2d res =
	{
		.x = v1->x * scalar,
		.y = v1->y * scalar,
	};

	return res;
}

static inline vec2d vec2d_div(const vec2d* const v1, float scalar)
{
	vec2d res =
	{
		.x = v1->x / scalar,
		.y = v1->y / scalar,
	};

	return res;
}

static inline float vec2d_dot(const vec2d* const v1, const vec2d* const v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y);
}

static inline float vec2d_angle_between(const vec2d* const v1, const vec2d* const v2)
{
	return
		atan2f(
			(v2->y * v1->x) - (v2->x * v1->y),
			(v2->x * v1->x) + (v2->y * v1->y)
		);
}

static inline float vec2d_cross(const vec2d* const v1, const vec2d* const v2)
{
	return (v1->x * v2->y) - (v1->y * v2->x);
}

static inline float vec2d_len(const vec2d* const vec)
{
	return (float)sqrt((vec->x * vec->x) + (vec->y * vec->y));
}

extern vec2d vec2d_norm(const vec2d* const vec);

extern vec2d vec2_transform_by_frame2d(
	const vec2d* const point,
	const frame2d* const frame
);

extern vec2d vec2_transform_by_frame2d_inv(
	const vec2d* const point,
	const frame2d* const frame
);

extern vec2d vec2d_calc_forwards(
	const frame2d* const frame, 
	const vec2d* const worldForward
);

extern vec2d vec2d_calc_forwards_trans(
	const transform2d* const frame,
	const vec2d* const worldForward
);

extern vec2d vec2_transform(
	const vec2d* const point,
	const transform2d* const transform
);

extern vec2d vec2_transform_inv(
	const vec2d* const point,
	const transform2d* const transform
);

extern void vec2d_print(const vec2d* const vec);

extern void frame2d_print(const frame2d* const frame);

static inline int get_sign(int value)
{
	if (value < 0)
	{
		return -1;
	}
	else if (value > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static inline float to_rad(float deg)
{
	return (deg * (float)M_PI) / 180.0f;
}

static inline float to_deg(float rad)
{
	return (rad * 180.0f) / (float)M_PI;
}

static inline int clampi(int value, int lower, int upper)
{
	if (value >= lower &&
		value <= upper)
	{
		return value;
	}
	else if (value < lower)
	{
		return lower;
	}
	else
	{
		return upper;
	}
}

static inline float clampf(float value, float lower, float upper)
{
	if (value >= lower &&
		value <= upper)
	{
		return value;
	}
	else if (value < lower)
	{
		return lower;
	}
	else
	{
		return upper;
	}
}
