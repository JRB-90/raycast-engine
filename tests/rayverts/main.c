#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "engine/engine_rayengine.h"
#include "engine/engine_screen.h"
#include "engine/engine_color.h"
#include "engine/engine_draw.h"
#include "engine/engine_math.h"
#include "engine/engine_resource.h"
#include "gridengine/gridengine_scene.h"
#include "gridengine/gridengine_render.h"
#include "time/time_helper.h"

const colformat SFORMAT = CF_ARGB;
const int SWIDTH = 640;
const int SHEIGHT = 480;
const int SSIZE = 30;
const int GRID_MIN_SIZE = 4;
const int GRID_MAX_SIZE = 30;
const float TRANS_AMT = 0.025f;
const float ROT_AMT = 0.005f;

const char* BRICK_TEX_PATH = "textures/brick/brick_64.rtx";
const char* CONCRETE_TEX_PATH = "textures/concrete/concrete_64.rtx";
const char* METAL_TEX_PATH = "textures/metal/metal_64.rtx";
const char* LAMP_TEX_PATH = "sprites/static/lamp/lamp_64.rtx";
const char* COLUMN_TEX_PATH = "sprites/static/column/column_64.rtx";

const int BRICK_TEX_ID = 2;
const int CONCRETE_TEX_ID = 3;
const int METAL_TEX_ID = 4;
const int LAMP_TEX_ID = 5;
const int COLUMN_TEX_ID = 6;

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

bool shouldRender = true;

void sig_handler(int signum);
void cleanup(int status);
void build_test_scene();
void add_wall(int x, int y, int textureID);
int on_update(const input_state* const inputState, const float deltaTimeMS);
int on_render(screen_buffer* const screen);

int main(int argc, char** argv)
{
    engine = NULL;
    scene = NULL;

    signal(SIGINT, sig_handler);

    scene = create_scene("Vert drawing test scene", SWIDTH);
    build_test_scene();

    engine_config config =
    {
        .type = ENGINE_GRID,
        .targetFps = 24,
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

    engine->on_update = &on_update;
    engine->on_render = &on_render;

    int res = run_engine(engine);

    destroy_engine(engine);
    destroy_scene(scene);
    int c = getchar();

    exit(res == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
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

    if (scene != NULL)
    {
        if (scene->resources.textures[BRICK_TEX_ID] != NULL)
        {
            destroy_texture_resources(&scene->resources, BRICK_TEX_ID);
            destroy_texture_resources(&scene->resources, CONCRETE_TEX_ID);
            destroy_texture_resources(&scene->resources, METAL_TEX_ID);
            destroy_texture_resources(&scene->resources, LAMP_TEX_ID);
            destroy_texture_resources(&scene->resources, COLUMN_TEX_ID);
        }

        destroy_scene(scene);
    }

    if (status != EXIT_SUCCESS)
    {
        getchar();
    }
    
    exit(status);
}

void build_test_scene()
{
    int textureLoadError =
        create_texture_resources(
            &scene->resources, 
            BRICK_TEX_PATH, 
            BRICK_TEX_ID,
            SFORMAT
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            CONCRETE_TEX_PATH,
            CONCRETE_TEX_ID,
            SFORMAT
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            METAL_TEX_PATH,
            METAL_TEX_ID,
            SFORMAT
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            LAMP_TEX_PATH,
            LAMP_TEX_ID,
            SFORMAT
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            COLUMN_TEX_PATH,
            COLUMN_TEX_ID,
            SFORMAT
        );

    if (textureLoadError)
    {
        fprintf(stderr, "Failed to create texture resources\n");
        cleanup(EXIT_FAILURE);
    }

    int spriteAddError =
        add_sprite(
            scene,
            to_vec2d(32.0f, 30.0f),
            0,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(35.5f, 28.0f),
            1,
            LAMP_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(32.0f, 29.5f),
            2,
            LAMP_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(32.0f, 30.5f),
            3,
            LAMP_TEX_ID,
            1000.0f
        );

    if (spriteAddError)
    {
        fprintf(stderr, "Failed to add static sprites\n");
        cleanup(EXIT_FAILURE);
    }

    scene->world.grid[32][32].type = GRID_PSPAWN;

    add_wall(30, 28, BRICK_TEX_ID);
    add_wall(31, 28, BRICK_TEX_ID);
    add_wall(32, 28, BRICK_TEX_ID);
    add_wall(33, 28, BRICK_TEX_ID);
    add_wall(34, 28, BRICK_TEX_ID);

    add_wall(35, 25, CONCRETE_TEX_ID);
    add_wall(36, 25, CONCRETE_TEX_ID);
    add_wall(37, 25, CONCRETE_TEX_ID);

    add_wall(36, 28, METAL_TEX_ID);
    add_wall(36, 29, METAL_TEX_ID);
    add_wall(36, 31, METAL_TEX_ID);
}

void add_wall(int x, int y, int textureID)
{
    scene->world.grid[x][y].type = GRID_WALL;
    scene->world.grid[x][y].textureID = textureID;
}

int on_update(const input_state* const inputState, const float deltaTimeMS)
{
    printf("Delta: %.3fms\n", deltaTimeMS);
    move_player(
        inputState,
        scene,
        &WORLD_FWD,
        &WORLD_LEFT,
        deltaTimeMS
    );

    return 0;
}

int on_render(screen_buffer* const screen)
{
    if (SFORMAT == CF_RGB565)
    {
        draw_ceiling_floor16(
            &engine->screen,
            to_rgb565(&scene->colors.ceilingCol),
            to_rgb565(&scene->colors.floorCol)
        );
    }
    else
    {
        draw_ceiling_floor32(
            &engine->screen,
            to_argb(&scene->colors.ceilingCol),
            to_argb(&scene->colors.floorCol)
        );
    }

    render_grid_verts(
        engine,
        scene
    );

    if (SFORMAT == CF_RGB565)
    {

    }
    else
    {
        render_sprites32(
            engine,
            scene
        );
    }

    return 0;
}
