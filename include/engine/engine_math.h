#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

// Inlined definitions
static inline int Sign(int value)
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

static inline float ToRad(float deg)
{
	return (deg * (float)M_PI) / 180.0f;
}

static inline float ToDeg(float rad)
{
	return (rad * 180.0f) / (float)M_PI;
}