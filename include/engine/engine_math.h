#pragma once

/*
	Basic math objects/functions for dealing with 2d scenes.
*/

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

/// <summary>
/// Compose a vec2d from it's components.
/// </summary>
static inline vec2d vec2d_build(float x, float y)
{
	vec2d vec =
	{
		.x = x,
		.y = y
	};

	return vec;
}

/// <summary>
/// Compose a frame2d from it's components.
/// </summary>
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

/// <summary>
/// Add two vec2d's together.
/// </summary>
static inline vec2d vec2d_add(const vec2d* const v1, const vec2d* const v2)
{
	vec2d res =
	{
		.x = v1->x + v2->x,
		.y = v1->y + v2->y,
	};

	return res;
}

/// <summary>
/// Subract a vec2d from the other.
/// </summary>
static inline vec2d vec2d_sub(const vec2d* const v1, const vec2d* const v2)
{
	vec2d res =
	{
		.x = v1->x - v2->x,
		.y = v1->y - v2->y,
	};

	return res;
}

/// <summary>
/// Multiply a vec2d's by a scalar.
/// </summary>
static inline vec2d vec2d_mul(const vec2d* const v1, float scalar)
{
	vec2d res =
	{
		.x = v1->x * scalar,
		.y = v1->y * scalar,
	};

	return res;
}

/// <summary>
/// Divide a vec2d by a scalar.
/// </summary>
static inline vec2d vec2d_div(const vec2d* const v1, float scalar)
{
	vec2d res =
	{
		.x = v1->x / scalar,
		.y = v1->y / scalar,
	};

	return res;
}

/// <summary>
/// Dot product of two vec2d's.
/// </summary>
static inline float vec2d_dot(const vec2d* const v1, const vec2d* const v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y);
}

/// <summary>
/// Cross product of two vec2d's.
/// </summary>
static inline float vec2d_cross(const vec2d* const v1, const vec2d* const v2)
{
	return (v1->x * v2->y) - (v1->y * v2->x);
}

/// <summary>
/// Calculates the angle between two vec2d's.
/// </summary>
/// <returns>Angle between the vec2d's in radians.</returns>
static inline float vec2d_angle_between(const vec2d* const v1, const vec2d* const v2)
{
	return
		atan2f(
			(v2->y * v1->x) - (v2->x * v1->y),
			(v2->x * v1->x) + (v2->y * v1->y)
		);
}

/// <summary>
/// Calculates the length of a vec2d.
/// </summary>
/// <param name="vec">vec2d to calculate length of.</param>
/// <returns>Length of the vec2d.</returns>
static inline float vec2d_len(const vec2d* const vec)
{
	return (float)sqrt((vec->x * vec->x) + (vec->y * vec->y));
}

/// <summary>
/// Calculates the normalised form of a vec2d and returns it.
/// </summary>
/// <param name="vec">Vec2d to calculate normalised form.</param>
/// <returns>Normalised vec2d.</returns>
extern vec2d vec2d_norm(const vec2d* const vec);

/// <summary>
/// Transforms a vec2d by the given frame2d transform.
/// </summary>
extern vec2d vec2_transform_by_frame2d(
	const vec2d* const point,
	const frame2d* const frame
);

/// <summary>
/// Transforms a vec2d by the inverse of the given frame2d transform.
/// </summary>
extern vec2d vec2_transform_by_frame2d_inv(
	const vec2d* const point,
	const frame2d* const frame
);

/// <summary>
/// Calculates the forward vector in world frame of a given frame2d.
/// </summary>
/// <param name="frame">Frame2d to use as the transform.</param>
/// <param name="worldForward">The direction of forwards in world coords.</param>
/// <returns>Forwards direction of the transform.</returns>
extern vec2d vec2d_calc_forwards(
	const frame2d* const frame, 
	const vec2d* const worldForward
);

/// <summary>
/// Calculates the forward vector in world frame of a given transform2d.
/// </summary>
/// <param name="frame">Transform2d to use as the transform.</param>
/// <param name="worldForward">The direction of forwards in world coords.</param>
/// <returns>Forwards direction of the transform.</returns>
extern vec2d vec2d_calc_forwards_trans(
	const transform2d* const frame,
	const vec2d* const worldForward
);

/// <summary>
/// Transforms a vec2d by the given transform2d.
/// </summary>
extern vec2d vec2_transform(
	const vec2d* const point,
	const transform2d* const transform
);

/// <summary>
/// Transforms a vec2d by the inverse of the given transform2d.
/// </summary>
extern vec2d vec2_transform_inv(
	const vec2d* const point,
	const transform2d* const transform
);

/// <summary>
/// Prints the value of the given vec2d.
/// </summary>
extern void vec2d_print(const vec2d* const vec);

/// <summary>
/// Prints the value of the given frame2d.
/// </summary>
extern void frame2d_print(const frame2d* const frame);

/// <summary>
/// Calculates the sign of the given integer.
/// </summary>
/// <param name="value">Value to calculate the sign of.</param>
/// <returns>0 if value is zero, 1 if sign is positive, -1 if sign is negative.</returns>
static inline int get_signi(int value)
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

/// <summary>
/// Calculates the sign of the given float.
/// </summary>
/// <param name="value">Value to calculate the sign of.</param>
/// <returns>0 if value is zero, 1 if sign is positive, -1 if sign is negative.</returns>
static inline int get_signf(float value)
{
	if (value < 0.0f)
	{
		return -1;
	}
	else if (value > 0.0f)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/// <summary>
/// Calculates the sign of the given double.
/// </summary>
/// <param name="value">Value to calculate the sign of.</param>
/// <returns>0 if value is zero, 1 if sign is positive, -1 if sign is negative.</returns>
static inline int get_sign(double value)
{
	if (value < 0.0)
	{
		return -1;
	}
	else if (value > 0.0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/// <summary>
/// Converts an angle in degrees to radians.
/// </summary>
/// <param name="deg">Angle in degrees.</param>
/// <returns>Angle in radians.</returns>
static inline float to_radf(float deg)
{
	return (deg * (float)M_PI) / 180.0f;
}

/// <summary>
/// Converts an angle in radians to degrees.
/// </summary>
/// <param name="deg">Angle in radians.</param>
/// <returns>Angle in degrees.</returns>
static inline float to_degf(float rad)
{
	return (rad * 180.0f) / (float)M_PI;
}

/// <summary>
/// Clamps a given value into the range provided.
/// </summary>
/// <param name="value">Value to clamp.</param>
/// <param name="lower">Lower bound of range.</param>
/// <param name="upper">Upper bound of range.</param>
/// <returns>Value clamped into the provided range.</returns>
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

/// <summary>
/// Clamps a given value into the range provided.
/// </summary>
/// <param name="value">Value to clamp.</param>
/// <param name="lower">Lower bound of range.</param>
/// <param name="upper">Upper bound of range.</param>
/// <returns>Value clamped into the provided range.</returns>
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

/// <summary>
/// Clamps a given value into the range provided.
/// </summary>
/// <param name="value">Value to clamp.</param>
/// <param name="lower">Lower bound of range.</param>
/// <param name="upper">Upper bound of range.</param>
/// <returns>Value clamped into the provided range.</returns>
static inline double clamp(double value, double lower, double upper)
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
