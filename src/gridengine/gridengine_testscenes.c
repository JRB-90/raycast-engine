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

    if (load_test_sprites(scene))
    {
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
}

void destroy_test_textures(grid_scene* const scene)
{
    destroy_texture_resources(&scene->resources, BRICK_TEX_ID);
    destroy_texture_resources(&scene->resources, CONCRETE_TEX_ID);
    destroy_texture_resources(&scene->resources, METAL_TEX_ID);
    destroy_texture_resources(&scene->resources, LAMP_TEX_ID);
    destroy_texture_resources(&scene->resources, COLUMN_TEX_ID);
}

int load_test_sprites(grid_scene* const scene)
{
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
        return -1;
    }
}
