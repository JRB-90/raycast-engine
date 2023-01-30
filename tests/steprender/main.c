#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_subsystems.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_testscenes.h"
#include "gridengine/gridengine_render.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;

const vec2d WORLD_FWD =
{
	.x = 0.0,
	.y = 1.0
};

const vec2d WORLD_LEFT =
{
	.x = 1.0,
	.y = 0.0
};

rayengine* engine;
grid_scene* scene;
frame2d poses[5];

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    engine_config config =
    {
        .type = ENGINE_GRID,
        .targetFps = 10000,
        .format = (screen_format)
        {
            .format = SFORMAT,
            .width = SWIDTH,
            .height = SHEIGHT
        }
    };

    scene = gridengine_create_test_scene2("Vert drawing test scene", &config.format);
    if (scene == NULL)
    {
        fprintf(stderr, "Failed to create test scene, shutting down...\n");
        getchar();
        exit(EXIT_FAILURE);
    }

    engine = engine_create_new_rayengine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        gridengine_destroy_test_scene(scene);
        getchar();
        exit(EXIT_FAILURE);
    }

    poses[0] = frame2d_build(3.500f, 1.500f, 0.000f);
    poses[1] = frame2d_build(4.164f, 4.541f, 1.191f);
    poses[2] = frame2d_build(2.717f, 5.960f, 4.576f);
    poses[3] = frame2d_build(11.041f, 2.938f, 4.678f);
    poses[4] = frame2d_build(2.230f, 4.776f, 8.268f);

    scene->player.position = poses[1];

    printf("Press any key to advance render process...\n");
    getchar();

    int res = 0;

    draw_ceiling_floor32(
        &engine->screen,
        color_to_argb(&scene->colors.ceilingCol),
        color_to_argb(&scene->colors.floorCol)
    );
    engine_render_screen(&engine->screen);
    getchar();

    gridengine_reset_draw_state(&scene->drawState);

    int steps = engine->screen.width;
    float step = scene->player.fov / (float)steps;

    frame2d playerPos =
    {
        .x = scene->player.position.x,
        .y = scene->player.position.y,
        .theta = scene->player.position.theta - (scene->player.fov / 2.0f)
    };

    for (int i = 0; i < steps; i++)
    {
        traverse_result result;
        float alpha = scene->player.position.theta - playerPos.theta;

        // Find object ray collides with first, if any
        res =
            gridengine_project_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                alpha,
                &result
            );

        if (res)
        {
            fprintf(stderr, "Failed to project ray, shutting down...\n");
            engine_destroy_rayengine(engine);
            gridengine_destroy_test_scene(scene);
            getchar();
            exit(EXIT_FAILURE);
        }

        if (result.objectDistance >= 0.0f)
        {
            scene->drawState.wallDistances[i] = result.objectDistance;
        }
        else
        {
            scene->drawState.wallDistances[i] = FLT_MAX;
        }

        // If the ray has hit a wall, render the pixel column with texturing
        if (result.intersectedObject != NULL &&
            result.objectDistance > 0)
        {
            res =
                gridengine_render_vertical_strip32(
                    engine,
                    scene,
                    &result,
                    i
                );

            if (res)
            {
                fprintf(stderr, "Failed to render vert strip, shutting down...\n");
                engine_destroy_rayengine(engine);
                gridengine_destroy_test_scene(scene);
                getchar();
                exit(EXIT_FAILURE);
            }

            engine_render_screen(&engine->screen);
            getchar();
        }

        playerPos.theta += step;
    }

    res =
        gridengine_render_sprites(
            engine,
            scene,
            &scene->player.position
        );

    if (res)
    {
        fprintf(stderr, "Failed to render sprites, shutting down...\n");
        engine_destroy_rayengine(engine);
        gridengine_destroy_test_scene(scene);
        getchar();
        exit(EXIT_FAILURE);
    }

    engine_render_screen(&engine->screen);
    getchar();

    printf("\n===== Render process complete =====\n");

    input_state input = engine_get_default_input();
    while (!input.quit)
    {
        engine_update_input_state(&input);
    }

    engine_destroy_rayengine(engine);
    gridengine_destroy_test_scene(scene);
	exit(EXIT_SUCCESS);
}