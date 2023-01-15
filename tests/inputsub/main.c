#include <stdio.h>
#include <stdlib.h>
#include "engine/engine_subsystems.h"
#include "engine/engine_rayengine.h"
#include "crossplatform/crossplatform_time.h"

int main(int argc, char** argv)
{
	printf("Starting input subsystem test...\n");

#ifdef _USE_SDL
	screen_format sformat =
	{
		.width = 640,
		.height = 480,
		.format = CF_ARGB
	};

	screen_buffer screen = engine_get_default_screen();

	if (engine_init_render_subsystem(&sformat, &screen))
	{
		fprintf(stderr, "Failed to init render subsystem, exiting...");
		int c = getchar();
		exit(EXIT_FAILURE);
	}
#endif // _USE_SDL

	if (engine_init_input_subsystem())
	{
		fprintf(stderr, "Failed to init input subsystem, exiting...");
		int c = getchar();
		exit(EXIT_FAILURE);
	}

	printf("Input subsystem initialised\n");
	printf("Waiting for quit to be entered...\n");
	input_state inputState = engine_get_default_input();

	while (!inputState.quit)
	{
		if (engine_update_input_state(&inputState))
		{
			fprintf(stderr, "Failed to update input state, exiting...");
			engine_destroy_input_subsystem();
#ifdef _USE_SDL
			engine_destroy_render_subsystem(&screen);
#endif // _USE_SDL
			int c = getchar();
			exit(EXIT_FAILURE);
		}

		printf(
			"F:%d\tB:%d\tL:%d\tR:%d\t\n", 
			inputState.forwards,
			inputState.backwards,
			inputState.left,
			inputState.right
		);

		cross_sleep_ms(100);
	}

	if (engine_destroy_input_subsystem())
	{
		fprintf(stderr, "Failed to destroy input subsystem, exiting...");
		int c = getchar();
		exit(EXIT_FAILURE);
	}

#ifdef _USE_SDL
	if (engine_destroy_render_subsystem(&screen))
	{
		fprintf(stderr, "Failed to destroy render subsystem, exiting...");
		int c = getchar();
		exit(EXIT_FAILURE);
	}
#endif // _USE_SDL

	printf("Input subsystem destroyed\n");
	printf("Test complete\n");
	int c = getchar();
}