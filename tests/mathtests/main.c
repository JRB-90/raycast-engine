#include <stdio.h>
#include <stdlib.h>
#include "engine/engine_math.h"

int main(int argc, char** argv)
{
	printf("Starting math tests...\n\n");

	vec2d v1 = vec2d_build(0.1f, 0.2f);
	vec2d_print(&v1);

	vec2d v2 = vec2d_build(1.0f, -1.0f);

	vec2d v3 = vec2d_add(&v1, &v2);
	vec2d_print(&v3);

	v3 = vec2d_sub(&v1, &v2);
	vec2d_print(&v3);

	v3 = vec2d_mul(&v1, 10.0f);
	vec2d_print(&v3);

	v3 = vec2d_div(&v2, 2.0f);
	vec2d_print(&v3);

	float dot = vec2d_dot(&v1, &v2);
	printf("Dot: %.3f\n", dot);

	float cross = vec2d_cross(&v1, &v2);
	printf("Cross: %.3f\n", cross);

	float len = vec2d_len(&v2);
	printf("Len: %.3f\n", len);

	v3 = vec2d_norm(&v2);
	vec2d_print(&v3);

	frame2d f1 = frame2d_build(0.3f, 0.4f, to_radf(90.0f));
	frame2d_print(&f1);

	vec2d fwd = vec2d_build(0.0f, 1.0f);

	vec2d vf1 = vec2d_calc_forwards(&f1, &fwd);
	vec2d_print(&vf1);

	f1.theta = to_radf(360.0f);
	vf1 = vec2d_calc_forwards(&f1, &fwd);
	vec2d_print(&vf1);

	printf("\nTests complete\n");
	int c = getchar();
	exit(EXIT_SUCCESS);
}