#include <stdio.h>
#include <stdlib.h>
#include "engine/engine_math.h"

int main(int argc, char** argv)
{
	printf("Starting math tests...\n\n");

	vec2d v1 = to_vec2d(0.1f, 0.2f);
	print_vec2d(&v1);

	vec2d v2 = to_vec2d(1.0f, -1.0f);

	vec2d v3 = add_vec(&v1, &v2);
	print_vec2d(&v3);

	v3 = sub_vec(&v1, &v2);
	print_vec2d(&v3);

	v3 = mul_vec(&v1, 10.0f);
	print_vec2d(&v3);

	v3 = div_vec(&v2, 2.0f);
	print_vec2d(&v3);

	float dot = dot_vec(&v1, &v2);
	printf("Dot: %.3f\n", dot);

	float cross = cross_vec(&v1, &v2);
	printf("Cross: %.3f\n", cross);

	float len = len_vec(&v2);
	printf("Len: %.3f\n", len);

	v3 = norm_vec(&v2);
	print_vec2d(&v3);

	frame2d f1 = to_frame2d(0.3f, 0.4f, to_rad(90.0f));
	print_frame2d(&f1);

	vec2d fwd = to_vec2d(0.0f, 1.0f);

	vec2d vf1 = calc_forwards(&f1, &fwd);
	print_vec2d(&vf1);

	f1.theta = to_rad(360.0f);
	vf1 = calc_forwards(&f1, &fwd);
	print_vec2d(&vf1);

	printf("\nTests complete\n");
	int c = getchar();
	exit(EXIT_SUCCESS);
}