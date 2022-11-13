#include <stdio.h>
#include <stdlib.h>
#include "engine_subsystems.h"
#include "engine_logic.h"
#include "time_helper.h"

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

	screen_buffer screen = default_screen();

	if (init_render_subsystem(&sformat, &screen))
	{
		fprintf(stderr, "Failed to init render subsystem, exiting...");
		exit(EXIT_FAILURE);
	}
#endif // _USE_SDL

	if (init_input_subsystem())
	{
		fprintf(stderr, "Failed to init input subsystem, exiting...");
		exit(EXIT_FAILURE);
	}

	printf("Input subsystem initialised\n");
	printf("Waiting for quit to be entered...\n");
	input_state inputState = blank_input_state();

	while (!inputState.quit)
	{
		update_input_state(&inputState);

		printf(
			"F:%d\tB:%d\tL:%d\tR:%d\t\n", 
			inputState.forwards,
			inputState.backwards,
			inputState.left,
			inputState.right
		);

		sleep_millis(100);
	}

	if (destroy_input_subsystem())
	{
		fprintf(stderr, "Failed to destroy input subsystem, exiting...");
		exit(EXIT_FAILURE);
	}

#ifdef _USE_SDL
	if (destroy_render_subsystem(&screen))
	{
		fprintf(stderr, "Failed to destroy render subsystem, exiting...");
		exit(EXIT_FAILURE);
	}
#endif // _USE_SDL

	printf("Input subsystem destroyed\n");
	printf("Test complete\n");
	getchar();
}