#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "engine/engine_subsystems.h"
#include "crossplatform/crossplatform_time.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;
const float SIZE = 100.0f;
const float TRANS_AMT = 0.5f;
const float ROT_AMT = 0.01f;
const float SCALE_AMT = 0.1f;

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

bool shouldRender;

rayengine* engine;
transform2d viewPort;
transform2d playerPos;

vec2d pl1;
vec2d pl2;
vec2d pl3;
vec2d pl4;

vec2d sq1;
vec2d sq2;
vec2d sq3;
vec2d sq4;

void sig_handler(int signum);
void cleanup(int status);
void move_shape();
void render_shape();

int main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    shouldRender = false;
    engine = NULL;

    viewPort = (transform2d)
    {
        .scale = (vec2d)
        {
            .x = 2.0f,
            .y = 2.0f
        },
        .trans = (vec2d)
        {
            .x = (float)(SWIDTH / 2),
            .y = (float)(SHEIGHT / 2)
        },
        .rot = to_radf(180.0f)
    };

    playerPos = (transform2d)
    {
        .scale = (vec2d)
        {
            .x = 1.0f,
            .y = 1.0f
        },
        .trans = (vec2d)
        {
            .x = 0.0f,
            .y = 0.0f
        },
        .rot = to_radf(0.0f)
    };

    pl1 = (vec2d){  5.0f,  5.0f };
    pl2 = (vec2d){ -5.0f, -5.0f };
    pl3 = (vec2d){  0.0f,  0.0f };
    pl4 = (vec2d){  0.0f, 20.0f };

    float halfSize = SIZE / 2.0f;
    sq1 = (vec2d){ -halfSize, -halfSize };
    sq2 = (vec2d){ halfSize, -halfSize };
    sq3 = (vec2d){ halfSize,  halfSize };
    sq4 = (vec2d){ -halfSize,  halfSize };

    engine_config config =
    {
        .type = ENGINE_GRID,
        .format = (screen_format)
        {
            .format = SFORMAT,
            .width = SWIDTH,
            .height = SHEIGHT
        }
    };

    engine = engine_create_new(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    render_shape();
    
    while (!engine->input.quit)
    {
        engine_update_input_state(&engine->input);
        move_shape();

        if (shouldRender)
        {
            render_shape();
        }
        
        cross_sleep_ms(1);
    }

    cleanup(EXIT_SUCCESS);
}

void sig_handler(int signum)
{
    if (signum == SIGTERM ||
        signum == SIGABRT ||
        signum == SIGINT)
    {
        cleanup(EXIT_SUCCESS);
    }
}

void cleanup(int status)
{
    if (engine != NULL)
    {
        engine_destroy(engine);
    }

    exit(status);
}

void move_shape()
{
    if (engine->input.forwards)
    {
        vec2d travelDir = vec2d_calc_forwards_trans(&playerPos, &WORLD_FWD);
        vec2d transVec = vec2d_mul(&travelDir, TRANS_AMT);
        playerPos.trans = vec2d_add(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.backwards)
    {
        vec2d travelDir = vec2d_calc_forwards_trans(&playerPos, &WORLD_FWD);
        vec2d transVec = vec2d_mul(&travelDir, TRANS_AMT);
        playerPos.trans = vec2d_sub(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.left)
    {
        vec2d travelDir = vec2d_calc_forwards_trans(&playerPos, &WORLD_LEFT);
        vec2d transVec = vec2d_mul(&travelDir, TRANS_AMT);
        playerPos.trans = vec2d_add(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.right)
    {
        vec2d travelDir = vec2d_calc_forwards_trans(&playerPos, &WORLD_LEFT);
        vec2d transVec = vec2d_mul(&travelDir, TRANS_AMT);
        playerPos.trans = vec2d_sub(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.toggleRenderMode)
    {
        viewPort.scale.x += SCALE_AMT;
        viewPort.scale.y += SCALE_AMT;
        shouldRender = true;
    }

    if (engine->input.toggleDebug)
    {
        viewPort.scale.x -= SCALE_AMT;
        viewPort.scale.y -= SCALE_AMT;
        shouldRender = true;
    }

    if (engine->input.rotLeft)
    {
        playerPos.rot -= ROT_AMT;
        shouldRender = true;
    }

    if (engine->input.rotRight)
    {
        playerPos.rot += ROT_AMT;
        shouldRender = true;
    }
}

void render_shape()
{
    shouldRender = false;

    clktimer timer;
    clktimer_start(&timer);

    vec2d _sq1 = vec2_transform_inv(&sq1, &playerPos);
    vec2d _sq2 = vec2_transform_inv(&sq2, &playerPos);
    vec2d _sq3 = vec2_transform_inv(&sq3, &playerPos);
    vec2d _sq4 = vec2_transform_inv(&sq4, &playerPos);

    _sq1 = vec2_transform(&_sq1, &viewPort);
    _sq2 = vec2_transform(&_sq2, &viewPort);
    _sq3 = vec2_transform(&_sq3, &viewPort);
    _sq4 = vec2_transform(&_sq4, &viewPort);

    vec2d _pl1 = vec2_transform(&pl1, &viewPort);
    vec2d _pl2 = vec2_transform(&pl2, &viewPort);
    vec2d _pl3 = vec2_transform(&pl3, &viewPort);
    vec2d _pl4 = vec2_transform(&pl4, &viewPort);

    deltatime transformTime = clktimer_elapsed_ms(&timer);

    clktimer_start(&timer);

    draw_clear_screen32(&engine->screen, 0xFF000000);

    draw_line32_safe(
        &engine->screen, 
        0xFFFFFFFF, 
        (int)_sq1.x, 
        (int)_sq1.y, 
        (int)_sq2.x, 
        (int)_sq2.y
    );

    draw_line32_safe(
        &engine->screen, 
        0xFFFFFFFF, 
        (int)_sq2.x, 
        (int)_sq2.y, 
        (int)_sq3.x, 
        (int)_sq3.y
    );

    draw_line32_safe(
        &engine->screen, 
        0xFFFFFFFF, 
        (int)_sq3.x, 
        (int)_sq3.y, 
        (int)_sq4.x, 
        (int)_sq4.y
    );

    draw_line32_safe(
        &engine->screen, 
        0xFFFFFFFF, 
        (int)_sq4.x, 
        (int)_sq4.y, 
        (int)_sq1.x, 
        (int)_sq1.y
    );

    // Draw player
    float width = _pl2.x - _pl1.x;
    float height = _pl2.y - _pl1.y;

    draw_filled_rect32_safe(
        &engine->screen,
        0xFFFF0000,
        (int)_pl1.x,
        (int)_pl1.y,
        (int)_pl2.x - (int)_pl1.x,
        (int)_pl2.y - (int)_pl1.y
    );

    draw_line32_safe(
        &engine->screen,
        0xFFFF0000,
        (int)_pl3.x,
        (int)_pl3.y,
        (int)_pl4.x,
        (int)_pl4.y
    );

    engine_render_screen(&engine->screen);

    deltatime renderTime = clktimer_elapsed_ms(&timer);

    printf("Transform time: %.3fms\n", transformTime);
    printf("Render time:    %.3fms\n", transformTime);
    frame2d_print(&playerPos);
}
