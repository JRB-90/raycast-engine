#include "gridengine/gridengine_testscenes.h"

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

void add_wall(grid_scene* const scene, int x, int y, int textureID);
int load_test_textures(
    grid_scene* const scene,
    colformat format
);
void destroy_test_textures(grid_scene* const scene);

grid_scene* create_test_scene1(
    const char* const name,
    const screen_format* const screenFormat)
{
    grid_scene* scene = create_scene(name, screenFormat->width);

    if (scene == NULL)
    {
        return NULL;
    }

    if (load_test_textures(scene, screenFormat->format))
    {
        return NULL;
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
        return NULL;
    }

    scene->world.grid[32][32].type = GRID_PSPAWN;

    add_wall(scene, 30, 28, BRICK_TEX_ID);
    add_wall(scene, 31, 28, BRICK_TEX_ID);
    add_wall(scene, 32, 28, BRICK_TEX_ID);
    add_wall(scene, 33, 28, BRICK_TEX_ID);
    add_wall(scene, 34, 28, BRICK_TEX_ID);

    add_wall(scene, 35, 25, CONCRETE_TEX_ID);
    add_wall(scene, 36, 25, CONCRETE_TEX_ID);
    add_wall(scene, 37, 25, CONCRETE_TEX_ID);

    add_wall(scene, 36, 28, METAL_TEX_ID);
    add_wall(scene, 36, 29, METAL_TEX_ID);
    add_wall(scene, 36, 31, METAL_TEX_ID);

    return scene;
}

grid_scene* create_test_scene2(
    const char* const name,
    const screen_format* const screenFormat)
{
    grid_scene* scene = create_scene(name, screenFormat->width);

    if (scene == NULL)
    {
        return NULL;
    }

    if (load_test_textures(scene, screenFormat->format))
    {
        return NULL;
    }

    int spriteAddError =
        add_sprite(
            scene,
            to_vec2d(1.5f, 4.5f),
            0,
            LAMP_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(1.5f, 6.5f),
            1,
            LAMP_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(9.5f, 2.5f),
            2,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(12.5f, 2.5f),
            3,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(15.5f, 2.5f),
            4,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(18.5f, 2.5f),
            5,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(9.5f, 8.5f),
            6,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(12.5f, 8.5f),
            7,
            COLUMN_TEX_ID,
            1000.0f
        );

    spriteAddError |=
        add_sprite(
            scene,
            to_vec2d(15.5f, 8.5f),
            8,
            COLUMN_TEX_ID,
            1000.0f
        );

    if (spriteAddError)
    {
        fprintf(stderr, "Failed to add static sprites\n");
        return NULL;
    }

    scene->player.position = (frame2d)
    {
        3.5f, 1.5f, 0.0f
    };

    scene->world.grid[3][1].type = GRID_PSPAWN;

    // Starting area

    add_wall(scene, 2, 0, BRICK_TEX_ID);
    add_wall(scene, 3, 0, BRICK_TEX_ID);
    add_wall(scene, 4, 0, BRICK_TEX_ID);

    add_wall(scene, 1, 1, BRICK_TEX_ID);
    add_wall(scene, 2, 1, BRICK_TEX_ID);
    add_wall(scene, 4, 1, BRICK_TEX_ID);
    add_wall(scene, 5, 1, BRICK_TEX_ID);

    add_wall(scene, 1, 2, BRICK_TEX_ID);
    add_wall(scene, 1, 3, BRICK_TEX_ID);
    add_wall(scene, 1, 5, BRICK_TEX_ID);
    add_wall(scene, 1, 7, BRICK_TEX_ID);
    add_wall(scene, 1, 8, BRICK_TEX_ID);

    add_wall(scene, 0, 3, BRICK_TEX_ID);
    add_wall(scene, 0, 4, BRICK_TEX_ID);
    add_wall(scene, 0, 5, BRICK_TEX_ID);
    add_wall(scene, 0, 6, BRICK_TEX_ID);
    add_wall(scene, 0, 7, BRICK_TEX_ID);

    add_wall(scene, 5, 2, BRICK_TEX_ID);
    add_wall(scene, 5, 3, BRICK_TEX_ID);
    add_wall(scene, 5, 7, BRICK_TEX_ID);
    add_wall(scene, 5, 8, BRICK_TEX_ID);

    add_wall(scene, 1, 9, BRICK_TEX_ID);
    add_wall(scene, 2, 9, BRICK_TEX_ID);
    add_wall(scene, 3, 9, BRICK_TEX_ID);
    add_wall(scene, 4, 9, BRICK_TEX_ID);
    add_wall(scene, 5, 9, BRICK_TEX_ID);

    add_wall(scene, 6, 3, BRICK_TEX_ID);
    add_wall(scene, 6, 7, BRICK_TEX_ID);

    // Big hall

    add_wall(scene, 7, 0, CONCRETE_TEX_ID);
    add_wall(scene, 7, 1, CONCRETE_TEX_ID);
    add_wall(scene, 7, 2, CONCRETE_TEX_ID);
    add_wall(scene, 7, 3, CONCRETE_TEX_ID);

    add_wall(scene, 7, 7, CONCRETE_TEX_ID);
    add_wall(scene, 7, 8, CONCRETE_TEX_ID);
    add_wall(scene, 7, 9, CONCRETE_TEX_ID);
    add_wall(scene, 7, 10, CONCRETE_TEX_ID);

    add_wall(scene, 8, 0, CONCRETE_TEX_ID);
    add_wall(scene, 9, 0, CONCRETE_TEX_ID);
    add_wall(scene, 10, 0, CONCRETE_TEX_ID);
    add_wall(scene, 11, 0, CONCRETE_TEX_ID);
    add_wall(scene, 12, 0, CONCRETE_TEX_ID);
    add_wall(scene, 13, 0, CONCRETE_TEX_ID);
    add_wall(scene, 14, 0, CONCRETE_TEX_ID);
    add_wall(scene, 15, 0, CONCRETE_TEX_ID);
    add_wall(scene, 16, 0, CONCRETE_TEX_ID);
    add_wall(scene, 17, 0, CONCRETE_TEX_ID);
    add_wall(scene, 18, 0, CONCRETE_TEX_ID);
    add_wall(scene, 19, 0, CONCRETE_TEX_ID);
    add_wall(scene, 20, 0, CONCRETE_TEX_ID);

    add_wall(scene, 20, 1, CONCRETE_TEX_ID);
    add_wall(scene, 20, 2, CONCRETE_TEX_ID);
    add_wall(scene, 20, 3, CONCRETE_TEX_ID);
    add_wall(scene, 20, 4, CONCRETE_TEX_ID);
    add_wall(scene, 20, 5, CONCRETE_TEX_ID);
    add_wall(scene, 20, 6, CONCRETE_TEX_ID);
    add_wall(scene, 20, 7, CONCRETE_TEX_ID);
    add_wall(scene, 20, 8, CONCRETE_TEX_ID);
    add_wall(scene, 20, 9, CONCRETE_TEX_ID);
    add_wall(scene, 20, 10, CONCRETE_TEX_ID);

    add_wall(scene, 8, 10, CONCRETE_TEX_ID);
    add_wall(scene, 9, 10, CONCRETE_TEX_ID);
    add_wall(scene, 10, 10, CONCRETE_TEX_ID);
    add_wall(scene, 11, 10, CONCRETE_TEX_ID);
    add_wall(scene, 12, 10, CONCRETE_TEX_ID);
    add_wall(scene, 13, 10, CONCRETE_TEX_ID);
    add_wall(scene, 14, 10, CONCRETE_TEX_ID);
    add_wall(scene, 15, 10, CONCRETE_TEX_ID);
    add_wall(scene, 16, 10, CONCRETE_TEX_ID);
    add_wall(scene, 17, 10, CONCRETE_TEX_ID);

    // Long corridor

    add_wall(scene, 20, 11, METAL_TEX_ID);
    add_wall(scene, 20, 12, METAL_TEX_ID);
    add_wall(scene, 20, 13, METAL_TEX_ID);
    add_wall(scene, 20, 14, METAL_TEX_ID);
    add_wall(scene, 20, 15, METAL_TEX_ID);
    add_wall(scene, 20, 16, METAL_TEX_ID);
    add_wall(scene, 20, 17, METAL_TEX_ID);
    add_wall(scene, 20, 18, METAL_TEX_ID);
    add_wall(scene, 20, 19, METAL_TEX_ID);
    add_wall(scene, 20, 20, METAL_TEX_ID);

    add_wall(scene, 19, 20, METAL_TEX_ID);
    add_wall(scene, 18, 20, METAL_TEX_ID);
    add_wall(scene, 17, 20, METAL_TEX_ID);

    add_wall(scene, 17, 11, METAL_TEX_ID);
    add_wall(scene, 17, 12, METAL_TEX_ID);
    add_wall(scene, 17, 13, METAL_TEX_ID);
    add_wall(scene, 17, 14, METAL_TEX_ID);
    add_wall(scene, 17, 15, METAL_TEX_ID);
    add_wall(scene, 17, 16, METAL_TEX_ID);
    add_wall(scene, 17, 17, METAL_TEX_ID);

    return scene;
}

void destroy_test_scene(grid_scene* scene)
{
    destroy_test_textures(scene);
    destroy_scene(scene);
}

void add_wall(grid_scene* const scene, int x, int y, int textureID)
{
    scene->world.grid[x][y].type = GRID_WALL;
    scene->world.grid[x][y].textureID = textureID;
}

int load_test_textures(
    grid_scene* const scene,
    colformat format)
{
    int textureLoadError =
        create_texture_resources(
            &scene->resources,
            BRICK_TEX_PATH,
            BRICK_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            CONCRETE_TEX_PATH,
            CONCRETE_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            METAL_TEX_PATH,
            METAL_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            LAMP_TEX_PATH,
            LAMP_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            COLUMN_TEX_PATH,
            COLUMN_TEX_ID,
            format
        );

    if (textureLoadError)
    {
        fprintf(stderr, "Failed to create texture resources\n");
        return -1;
    }

    return 0;
}

void destroy_test_textures(grid_scene* const scene)
{
    destroy_texture_resources(&scene->resources, BRICK_TEX_ID);
    destroy_texture_resources(&scene->resources, CONCRETE_TEX_ID);
    destroy_texture_resources(&scene->resources, METAL_TEX_ID);
    destroy_texture_resources(&scene->resources, LAMP_TEX_ID);
    destroy_texture_resources(&scene->resources, COLUMN_TEX_ID);
}
