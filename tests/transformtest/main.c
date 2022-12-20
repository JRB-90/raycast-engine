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

bool shouldRender;

rayengine* engine;
frame2d trans;
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
    float halfSize = SIZE / 2.0f;

    sq1 = (vec2d){ -halfSize, -halfSize };
    sq2 = (vec2d){ halfSize, -halfSize };
    sq3 = (vec2d){ halfSize,  halfSize };
    sq4 = (vec2d){ -halfSize,  halfSize };

    trans = (frame2d)
    { 
        (SWIDTH / 2), 
        (SHEIGHT / 2), 
        to_rad(0.0f) 
    };

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
        trans.y -= TRANS_AMT;
        shouldRender = true;
    }

    if (engine->input.backwards)
    {
        trans.y += TRANS_AMT;
        shouldRender = true;
    }

    if (engine->input.left)
    {
        trans.x -= TRANS_AMT;
        shouldRender = true;
    }

    if (engine->input.right)
    {
        trans.x += TRANS_AMT;
        shouldRender = true;
    }

    if (engine->input.rotLeft)
    {
        trans.theta -= ROT_AMT;
        shouldRender = true;
    }

    if (engine->input.rotRight)
    {
        trans.theta += ROT_AMT;
        shouldRender = true;
    }
}

void render_shape()
{
    shouldRender = false;

    clktimer timer;
    start_timer(&timer);

    vec2d p1 = transform_vec2(&sq1, &trans);
    vec2d p2 = transform_vec2(&sq2, &trans);
    vec2d p3 = transform_vec2(&sq3, &trans);
    vec2d p4 = transform_vec2(&sq4, &trans);

    deltatime delta = elapsed_millis(&timer);

    draw_clear_screen32(&engine->screen, 0xFF000000);

    draw_line32_safe(&engine->screen, 0xFFFFFFFF, p1.x, p1.y, p2.x, p2.y);
    draw_line32_safe(&engine->screen, 0xFFFFFFFF, p2.x, p2.y, p3.x, p3.y);
    draw_line32_safe(&engine->screen, 0xFFFFFFFF, p3.x, p3.y, p4.x, p4.y);
    draw_line32_safe(&engine->screen, 0xFFFFFFFF, p4.x, p4.y, p1.x, p1.y);

    render_engine(engine);

    printf("Transform time: %.3fms\n", delta);
    print_frame2d(&trans);
}
