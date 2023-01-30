#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "engine/engine_math.h"
#include "engine/engine_rayengine.h"
#include "engine/engine_subsystems.h"
#include "engine/engine_draw.h"
#include "gridengine/gridengine_render.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_testscenes.h"
#include "crossplatform/crossplatform_time.h"

#define PLAYER_POSES    5
#define RAY_REPEATS     50000
#define VERT_REPEATS    3000
#define SPRITE_REPEATS  3000

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
frame2d poses[PLAYER_POSES];

void populate_poses();
int run_projection_bench();
int run_vert_bench();
int run_sprite_bench();
int run_render_pipeline_bench();

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

    populate_poses();

    int res = 0;

    /*res = run_projection_bench();
    if (res)
    {
        fprintf(stderr, "Failed to run projection bench, shutting down...\n");
        engine_destroy_rayengine(engine);
        gridengine_destroy_test_scene(scene);
        getchar();
        exit(EXIT_FAILURE);
    }*/

    /*res = run_vert_bench();
    if (res)
    {
        fprintf(stderr, "Failed to run vert strip bench, shutting down...\n");
        engine_destroy_rayengine(engine);
        gridengine_destroy_test_scene(scene);
        getchar();
        exit(EXIT_FAILURE);
    }*/

    /*res = run_sprite_bench();
    if (res)
    {
        fprintf(stderr, "Failed to run sprite bench, shutting down...\n");
        engine_destroy_rayengine(engine);
        gridengine_destroy_test_scene(scene);
        getchar();
        exit(EXIT_FAILURE);
    }*/

    res = run_render_pipeline_bench();
    if (res)
    {
        fprintf(stderr, "Failed to run render pipeline bench, shutting down...\n");
        engine_destroy_rayengine(engine);
        gridengine_destroy_test_scene(scene);
        getchar();
        exit(EXIT_FAILURE);
    }

    engine_destroy_rayengine(engine);
    gridengine_destroy_test_scene(scene);

	printf("\n===== Tests complete =====\n");
	getchar();
	exit(EXIT_SUCCESS);
}

void populate_poses()
{
	poses[0] = frame2d_build(3.500f, 1.500f, 0.000f);
    poses[1] = frame2d_build(4.164f, 4.541f, 1.191f);
    poses[2] = frame2d_build(2.717f, 5.960f, 4.576f);
    poses[3] = frame2d_build(11.041f, 2.938f, 4.678f);
    poses[4] = frame2d_build(2.230f, 4.776f, 8.268f);
}

int run_projection_bench()
{
    printf("Running projection tests...\n");

    clktimer timer;
    deltatime totalTime = 0.0f;
    int totalRuns = 0;

    int steps = engine->screen.width;
    float step = scene->player.fov / (float)steps;

    for (int i = 0; i < RAY_REPEATS; i++)
    {
        for (int j = 0; j < PLAYER_POSES; j++)
        {
            for (int k = 0; k < steps; k++)
            {
                frame2d* pose = &poses[j];
                traverse_result result;
                float alpha = scene->player.position.theta - pose->theta;

                clktimer_start(&timer);

                int res =
                    gridengine_project_ray(
                        scene,
                        pose,
                        &WORLD_FWD,
                        alpha,
                        &result
                    );

                deltatime runTime = clktimer_elapsed_ms(&timer);
                totalTime += runTime;
                totalRuns++;

                if (res)
                {
                    return -1;
                }
            }
        }
    }

    printf("\nProjection tests finished\n");
    printf("Runs:     %i\n", totalRuns);
    printf("Tot time: %.3fms\n", totalTime);
    printf("Ave time: %.6fus\n", totalTime * 1000.0f / (float)totalRuns);

    return 0;
}

int run_vert_bench()
{
    printf("Running vert strip tests...\n");

    clktimer timer;
    deltatime totalTime = 0.0f;
    int totalRuns = 0;

    for (int i = 0; i < VERT_REPEATS; i++)
    {
        for (int j = 0; j < PLAYER_POSES; j++)
        {
            draw_ceiling_floor32(&engine->screen, 0xFFFFFFFF, 0xFF000000);

            scene->player.position = poses[j];

            // Ensure we reset the draw state tracker before we render a new frame
            gridengine_reset_draw_state(&scene->drawState);

            // Find the steps to take in the ray sweep based on the screen width,
            // one ray per screen pixel column
            int steps = engine->screen.width;
            float step = scene->player.fov / (float)steps;

            frame2d playerPos =
            {
                .x = scene->player.position.x,
                .y = scene->player.position.y,
                .theta = scene->player.position.theta - (scene->player.fov / 2.0f)
            };

            int res;

            // Loop through every ray angle
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
                    return -1;
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
                    clktimer_start(&timer);

                    res =
                        gridengine_render_vertical_strip32(
                            engine,
                            scene,
                            &result,
                            i
                        );

                    deltatime runTime = clktimer_elapsed_ms(&timer);
                    totalTime += runTime;
                    totalRuns++;

                    if (res)
                    {
                        return -1;
                    }
                }

                playerPos.theta += step;
            }
        }
    }

    printf("\Vert strip tests finished\n");
    printf("Runs:     %i\n", totalRuns);
    printf("Tot time: %.3fms\n", totalTime);
    printf("Ave time: %.6fus\n", totalTime * 1000.0f / (float)totalRuns);

    return 0;
}

int run_sprite_bench()
{
    printf("Running sprite tests...\n");

    clktimer timer;
    deltatime totalTime = 0.0f;
    int totalRuns = 0;

    for (int i = 0; i < SPRITE_REPEATS; i++)
    {
        for (int j = 0; j < PLAYER_POSES; j++)
        {
            draw_ceiling_floor32(&engine->screen, 0xFFFFFFFF, 0xFF000000);

            scene->player.position = poses[j];

            // Ensure we reset the draw state tracker before we render a new frame
            gridengine_reset_draw_state(&scene->drawState);

            // Find the steps to take in the ray sweep based on the screen width,
            // one ray per screen pixel column
            int steps = engine->screen.width;
            float step = scene->player.fov / (float)steps;

            frame2d playerPos =
            {
                .x = scene->player.position.x,
                .y = scene->player.position.y,
                .theta = scene->player.position.theta - (scene->player.fov / 2.0f)
            };

            int res;

            // Loop through every ray angle
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
                    return -1;
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
                        return -1;
                    }
                }

                playerPos.theta += step;
            }

            clktimer_start(&timer);

            res =
                gridengine_render_sprites(
                    engine,
                    scene,
                    &scene->player.position
                );

            deltatime runTime = clktimer_elapsed_ms(&timer);
            totalTime += runTime;
            totalRuns++;

            if (res)
            {
                return -1;
            }
        }
    }

    printf("\Sprite tests finished\n");
    printf("Runs:     %i\n", totalRuns);
    printf("Tot time: %.3fms\n", totalTime);
    printf("Ave time: %.6fms\n", totalTime / (float)totalRuns);

    return 0;
}

int run_render_pipeline_bench()
{
    printf("Running render pipeline tests...\n");

    clktimer clearTimer;
    deltatime totalClearTime = 0.0f;
    int totalClearCalls = 0;

    clktimer projectTimer;
    deltatime totalProjectTime = 0.0f;
    int totalProjectCalls = 0;

    clktimer spriteTimer;
    deltatime totalSpriteTime = 0.0f;
    int totalSpriteCalls = 0;

    clktimer vertTimer;
    deltatime totalVertTime = 0.0f;
    int totalVertCalls = 0;

    clktimer_start(&clearTimer);

    draw_ceiling_floor32(&engine->screen, 0xFFFFFFFF, 0xFF000000);

    totalClearTime += clktimer_elapsed_ms(&clearTimer);
    totalClearCalls++;

    scene->player.position = poses[2];

    // Ensure we reset the draw state tracker before we render a new frame
    gridengine_reset_draw_state(&scene->drawState);

    // Find the steps to take in the ray sweep based on the screen width,
    // one ray per screen pixel column
    int steps = engine->screen.width;
    float step = scene->player.fov / (float)steps;

    frame2d playerPos =
    {
        .x = scene->player.position.x,
        .y = scene->player.position.y,
        .theta = scene->player.position.theta - (scene->player.fov / 2.0f)
    };

    int res;

    // Loop through every ray angle
    for (int i = 0; i < steps; i++)
    {
        traverse_result result;
        float alpha = scene->player.position.theta - playerPos.theta;

        clktimer_start(&projectTimer);

        // Find object ray collides with first, if any
        res =
            gridengine_project_ray(
                scene,
                &playerPos,
                &WORLD_FWD,
                alpha,
                &result
            );

        totalProjectTime += clktimer_elapsed_ms(&projectTimer);
        totalProjectCalls++;

        if (res)
        {
            return -1;
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
            clktimer_start(&vertTimer);

            res =
                gridengine_render_vertical_strip32(
                    engine,
                    scene,
                    &result,
                    i
                );

            totalVertTime += clktimer_elapsed_ms(&vertTimer);
            totalVertCalls++;

            if (res)
            {
                return -1;
            }
        }

        playerPos.theta += step;
    }

    clktimer_start(&spriteTimer);

    res =
        gridengine_render_sprites(
            engine,
            scene,
            &scene->player.position
        );

    totalSpriteTime += clktimer_elapsed_ms(&spriteTimer);
    totalSpriteCalls++;

    if (res)
    {
        return -1;
    }

    printf("Render pipeline tests finished\n");

    printf("\nClear calls: %i\n", totalClearCalls);
    printf("Clear total: %.3fms\n", totalClearTime);
    printf("Clear ave:   %.3fms\n", totalClearTime / (deltatime)totalClearCalls);

    printf("\nProject calls: %i\n", totalProjectCalls);
    printf("Project total: %.3fms\n", totalProjectTime);
    printf("Project ave:   %.3fms\n", totalProjectTime / (deltatime)totalProjectCalls);

    printf("\nSprite calls: %i\n", totalSpriteCalls);
    printf("Sprite total: %.3fms\n", totalSpriteTime);
    printf("Sprite ave:   %.3fms\n", totalSpriteTime / (deltatime)totalSpriteCalls);

    printf("\nVert calls: %i\n", totalVertCalls);
    printf("Vert total: %.3fms\n", totalVertTime);
    printf("Vert ave:   %.3fms\n", totalVertTime / (deltatime)totalVertCalls);

    return 0;
}
