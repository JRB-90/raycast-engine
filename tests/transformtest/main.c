#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "time/time_helper.h"
#include "engine/engine_rayengine.h"
#include "engine/engine_screen.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"

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
            .x = (SWIDTH / 2),
            .y = (SHEIGHT / 2)
        },
        .rot = to_rad(180.0f)
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
        .rot = to_rad(0.0f)
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

    engine = init_engine(&config);
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to init engine, shutting down...\n");
        cleanup(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    render_shape();
    
    while (!engine->input.quit)
    {
        update_engine(engine);
        move_shape();

        if (shouldRender)
        {
            render_shape();
        }
        
        sleep_millis(1);
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
        destroy_engine(engine);
    }

    exit(status);
}

void move_shape()
{
    if (engine->input.forwards)
    {
        vec2d travelDir = calc_forwards_trans(&playerPos, &WORLD_FWD);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        playerPos.trans = add_vec(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.backwards)
    {
        vec2d travelDir = calc_forwards_trans(&playerPos, &WORLD_FWD);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        playerPos.trans = sub_vec(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.left)
    {
        vec2d travelDir = calc_forwards_trans(&playerPos, &WORLD_LEFT);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        playerPos.trans = add_vec(&playerPos.trans, &transVec);
        shouldRender = true;
    }

    if (engine->input.right)
    {
        vec2d travelDir = calc_forwards_trans(&playerPos, &WORLD_LEFT);
        vec2d transVec = mul_vec(&travelDir, TRANS_AMT);
        playerPos.trans = sub_vec(&playerPos.trans, &transVec);
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
    start_timer(&timer);

    vec2d _sq1 = transform_vec2_inv(&sq1, &playerPos);
    vec2d _sq2 = transform_vec2_inv(&sq2, &playerPos);
    vec2d _sq3 = transform_vec2_inv(&sq3, &playerPos);
    vec2d _sq4 = transform_vec2_inv(&sq4, &playerPos);

    _sq1 = transform_vec2(&_sq1, &viewPort);
    _sq2 = transform_vec2(&_sq2, &viewPort);
    _sq3 = transform_vec2(&_sq3, &viewPort);
    _sq4 = transform_vec2(&_sq4, &viewPort);

    vec2d _pl1 = transform_vec2(&pl1, &viewPort);
    vec2d _pl2 = transform_vec2(&pl2, &viewPort);
    vec2d _pl3 = transform_vec2(&pl3, &viewPort);
    vec2d _pl4 = transform_vec2(&pl4, &viewPort);

    deltatime transformTime = elapsed_millis(&timer);

    start_timer(&timer);

    draw_clear_screen32(&engine->screen, 0xFF000000);

    draw_line32_safe(&engine->screen, 0xFFFFFFFF, _sq1.x, _sq1.y, _sq2.x, _sq2.y);
    draw_line32_safe(&engine->screen, 0xFFFFFFFF, _sq2.x, _sq2.y, _sq3.x, _sq3.y);
    draw_line32_safe(&engine->screen, 0xFFFFFFFF, _sq3.x, _sq3.y, _sq4.x, _sq4.y);
    draw_line32_safe(&engine->screen, 0xFFFFFFFF, _sq4.x, _sq4.y, _sq1.x, _sq1.y);

    // Draw player
    float width = _pl2.x - _pl1.x;
    float height = _pl2.y - _pl1.y;

    draw_filled_rect32_safe(
        &engine->screen,
        0xFFFF0000,
        _pl1.x,
        _pl1.y,
        _pl2.x - _pl1.x,
        _pl2.y - _pl1.y
    );

    draw_line32_safe(
        &engine->screen,
        0xFFFF0000,
        _pl3.x,
        _pl3.y,
        _pl4.x,
        _pl4.y
    );

    render_engine(engine);

    deltatime renderTime = elapsed_millis(&timer);

    printf("Transform time: %.3fms\n", transformTime);
    printf("Render time:    %.3fms\n", transformTime);
    print_frame2d(&playerPos);
}
