#include "gridengine/gridengine_testscenes.h"
#include <stdio.h>
#include <stdlib.h>

// Wall textures
const char* BRICK_TEX_PATH      = "textures/brick.rtx";
const char* CHESS_TEX_PATH      = "textures/chess.rtx";
const char* COBBLES_TEX_PATH    = "textures/cobbles.rtx";
const char* CONCRETE_TEX_PATH   = "textures/concrete.rtx";
const char* CROSSHATCH_TEX_PATH = "textures/crosshatch.rtx";
const char* GRASS_TEX_PATH      = "textures/grass.rtx";
const char* METAL1_TEX_PATH     = "textures/metal1.rtx";
const char* METAL2_TEX_PATH     = "textures/metal2.rtx";
const char* OCTOPUS_TEX_PATH    = "textures/octopus.rtx";
const char* ROCK1_TEX_PATH      = "textures/rock1.rtx";
const char* ROCK2_TEX_PATH      = "textures/rock2.rtx";
const char* ROCK3_TEX_PATH      = "textures/rock3.rtx";
const char* ROCK4_TEX_PATH      = "textures/rock4.rtx";
const char* RUST_TEX_PATH       = "textures/rust.rtx";
const char* SOIL1_TEX_PATH      = "textures/soil1.rtx";
const char* SOIL2_TEX_PATH      = "textures/soil2.rtx";
const char* TILES_TEX_PATH      = "textures/tiles.rtx";
const char* WOOD1_TEX_PATH      = "textures/wood1.rtx";
const char* WOOD2_TEX_PATH      = "textures/wood2.rtx";

// Static sprites
const char* AMMOCRATE_TEX_PATH  = "textures/ammocrate.rtx";
const char* BARREL1_TEX_PATH    = "textures/barrel1.rtx";
const char* BARREL2_TEX_PATH    = "textures/barrel2.rtx";
const char* BRICKPILE_TEX_PATH  = "textures/brickpile.rtx";
const char* KEYCARD_TEX_PATH    = "textures/keycard.rtx";
const char* LAMP_TEX_PATH       = "textures/lamp.rtx";
const char* PLANTER_TEX_PATH    = "textures/planter.rtx";

// Robot sprites
const char* ROBOT0_TEX_PATH     = "textures/robot0.rtx";
const char* ROBOT1_TEX_PATH     = "textures/robot1.rtx";
const char* ROBOT2_TEX_PATH     = "textures/robot2.rtx";
const char* ROBOT3_TEX_PATH     = "textures/robot3.rtx";
const char* ROBOT4_TEX_PATH     = "textures/robot4.rtx";
const char* ROBOT5_TEX_PATH     = "textures/robot5.rtx";
const char* ROBOT6_TEX_PATH     = "textures/robot6.rtx";

// Unique ID's for all textures
const int BRICK_TEX_ID      = 1;
const int CHESS_TEX_ID      = 2;
const int COBBLES_TEX_ID    = 3;
const int CONCRETE_TEX_ID   = 4;
const int CROSSHATCH_TEX_ID = 5;
const int GRASS_TEX_ID      = 6;
const int METAL1_TEX_ID     = 7;
const int METAL2_TEX_ID     = 8;
const int OCTOPUS_TEX_ID    = 9;
const int ROCK1_TEX_ID      = 10;
const int ROCK2_TEX_ID      = 11;
const int ROCK3_TEX_ID      = 12;
const int ROCK4_TEX_ID      = 13;
const int RUST_TEX_ID       = 14;
const int SOIL1_TEX_ID      = 15;
const int SOIL2_TEX_ID      = 16;
const int TILES_TEX_ID      = 17;
const int WOOD1_TEX_ID      = 18;
const int WOOD2_TEX_ID      = 19;

const int AMMOCRATE_TEX_ID  = 20;
const int BARREL1_TEX_ID    = 21;
const int BARREL2_TEX_ID    = 22;
const int BRICKPILE_TEX_ID  = 23;
const int KEYCARD_TEX_ID    = 24;
const int LAMP_TEX_ID       = 25;
const int PLANTER_TEX_ID    = 26;

const int ROBOT0_TEX_ID     = 27;
const int ROBOT1_TEX_ID     = 28;
const int ROBOT2_TEX_ID     = 29;
const int ROBOT3_TEX_ID     = 30;
const int ROBOT4_TEX_ID     = 31;
const int ROBOT5_TEX_ID     = 32;
const int ROBOT6_TEX_ID     = 33;

void add_wall(grid_scene* const scene, int x, int y, int textureID);
int load_test_textures(
    grid_scene* const scene,
    colformat format
);
void destroy_test_sprites(grid_scene* const scene);
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

    int spriteID = 0;

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(32.0f, 30.0f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        LAMP_TEX_ID,
        to_vec2d(35.5f, 28.0f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        LAMP_TEX_ID,
        to_vec2d(32.0f, 29.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        LAMP_TEX_ID,
        to_vec2d(32.0f, 30.5f),
        1000.0f
    );

    scene->world.grid[32][32].type = GRID_PSPAWN;

    add_wall(scene, 30, 28, BRICK_TEX_ID);
    add_wall(scene, 31, 28, BRICK_TEX_ID);
    add_wall(scene, 32, 28, BRICK_TEX_ID);
    add_wall(scene, 33, 28, BRICK_TEX_ID);
    add_wall(scene, 34, 28, BRICK_TEX_ID);

    add_wall(scene, 35, 25, CONCRETE_TEX_ID);
    add_wall(scene, 36, 25, CONCRETE_TEX_ID);
    add_wall(scene, 37, 25, CONCRETE_TEX_ID);

    add_wall(scene, 36, 28, METAL1_TEX_ID);
    add_wall(scene, 36, 29, METAL1_TEX_ID);
    add_wall(scene, 36, 31, METAL1_TEX_ID);

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

    int spriteID = 0;

    create_sprite(
        scene,
        spriteID++,
        LAMP_TEX_ID,
        to_vec2d(1.5f, 4.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        LAMP_TEX_ID,
        to_vec2d(1.5f, 6.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(9.5f, 2.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(12.5f, 2.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(15.5f, 2.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(18.5f, 2.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(9.5f, 8.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(12.5f, 8.5f),
        1000.0f
    );

    create_sprite(
        scene,
        spriteID++,
        BARREL1_TEX_ID,
        to_vec2d(15.5f, 8.5f),
        1000.0f
    );

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

    add_wall(scene, 20, 11, METAL1_TEX_ID);
    add_wall(scene, 20, 12, METAL1_TEX_ID);
    add_wall(scene, 20, 13, METAL1_TEX_ID);
    add_wall(scene, 20, 14, METAL1_TEX_ID);
    add_wall(scene, 20, 15, METAL1_TEX_ID);
    add_wall(scene, 20, 16, METAL1_TEX_ID);
    add_wall(scene, 20, 17, METAL1_TEX_ID);
    add_wall(scene, 20, 18, METAL1_TEX_ID);
    add_wall(scene, 20, 19, METAL1_TEX_ID);
    add_wall(scene, 20, 20, METAL1_TEX_ID);

    add_wall(scene, 19, 20, METAL1_TEX_ID);
    add_wall(scene, 18, 20, METAL1_TEX_ID);
    add_wall(scene, 17, 20, METAL1_TEX_ID);

    add_wall(scene, 17, 11, METAL1_TEX_ID);
    add_wall(scene, 17, 12, METAL1_TEX_ID);
    add_wall(scene, 17, 13, METAL1_TEX_ID);
    add_wall(scene, 17, 14, METAL1_TEX_ID);
    add_wall(scene, 17, 15, METAL1_TEX_ID);
    add_wall(scene, 17, 16, METAL1_TEX_ID);
    add_wall(scene, 17, 17, METAL1_TEX_ID);

    return scene;
}

void destroy_test_scene(grid_scene* scene)
{
    destroy_test_sprites(scene);
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
            AMMOCRATE_TEX_PATH,
            AMMOCRATE_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            BARREL1_TEX_PATH,
            BARREL1_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            BARREL2_TEX_PATH,
            BARREL2_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            BRICK_TEX_PATH,
            BRICK_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            BRICKPILE_TEX_PATH,
            BRICKPILE_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            CHESS_TEX_PATH,
            CHESS_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            COBBLES_TEX_PATH,
            COBBLES_TEX_ID,
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
            CROSSHATCH_TEX_PATH,
            CROSSHATCH_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            GRASS_TEX_PATH,
            GRASS_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            KEYCARD_TEX_PATH,
            KEYCARD_TEX_ID,
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
            METAL1_TEX_PATH,
            METAL1_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            METAL2_TEX_PATH,
            METAL2_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            OCTOPUS_TEX_PATH,
            OCTOPUS_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT0_TEX_PATH,
            ROBOT0_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT1_TEX_PATH,
            ROBOT1_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT2_TEX_PATH,
            ROBOT2_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT3_TEX_PATH,
            ROBOT3_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT4_TEX_PATH,
            ROBOT4_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT5_TEX_PATH,
            ROBOT5_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROBOT6_TEX_PATH,
            ROBOT6_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROCK1_TEX_PATH,
            ROCK1_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROCK2_TEX_PATH,
            ROCK2_TEX_ID,
            format
        );
    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROCK3_TEX_PATH,
            ROCK3_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            ROCK4_TEX_PATH,
            ROCK4_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            RUST_TEX_PATH,
            RUST_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            SOIL1_TEX_PATH,
            SOIL1_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            SOIL2_TEX_PATH,
            SOIL2_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            TILES_TEX_PATH,
            TILES_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            WOOD1_TEX_PATH,
            WOOD1_TEX_ID,
            format
        );

    textureLoadError |=
        create_texture_resources(
            &scene->resources,
            WOOD2_TEX_PATH,
            WOOD2_TEX_ID,
            format
        );

    if (textureLoadError)
    {
        fprintf(stderr, "Failed to create texture resources\n");
        return -1;
    }

    return 0;
}

void destroy_test_sprites(grid_scene* const scene)
{
    list_node* current = scene->world.sprites.head;
    
    while (current != NULL)
    {
        sprite_obj* sprite = (sprite_obj*)current->data;

        if (sprite != NULL)
        {
            free(sprite);
            current->data = NULL;
        }

        current = current->next;
    }

    list_clear(&scene->world.sprites);
}

void destroy_test_textures(grid_scene* const scene)
{
    destroy_texture_resources(&scene->resources, AMMOCRATE_TEX_ID);
    destroy_texture_resources(&scene->resources, BARREL1_TEX_ID);
    destroy_texture_resources(&scene->resources, BARREL2_TEX_ID);
    destroy_texture_resources(&scene->resources, BRICK_TEX_ID);
    destroy_texture_resources(&scene->resources, BRICKPILE_TEX_ID);
    destroy_texture_resources(&scene->resources, CHESS_TEX_ID);
    destroy_texture_resources(&scene->resources, COBBLES_TEX_ID);
    destroy_texture_resources(&scene->resources, CONCRETE_TEX_ID);
    destroy_texture_resources(&scene->resources, CROSSHATCH_TEX_ID);
    destroy_texture_resources(&scene->resources, GRASS_TEX_ID);
    destroy_texture_resources(&scene->resources, KEYCARD_TEX_ID);
    destroy_texture_resources(&scene->resources, LAMP_TEX_ID);
    destroy_texture_resources(&scene->resources, METAL1_TEX_ID);
    destroy_texture_resources(&scene->resources, METAL2_TEX_ID);
    destroy_texture_resources(&scene->resources, OCTOPUS_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT0_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT1_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT2_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT3_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT4_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT5_TEX_ID);
    destroy_texture_resources(&scene->resources, ROBOT6_TEX_ID);
    destroy_texture_resources(&scene->resources, ROCK1_TEX_ID);
    destroy_texture_resources(&scene->resources, ROCK2_TEX_ID);
    destroy_texture_resources(&scene->resources, ROCK3_TEX_ID);
    destroy_texture_resources(&scene->resources, ROCK4_TEX_ID);
    destroy_texture_resources(&scene->resources, RUST_TEX_ID);
    destroy_texture_resources(&scene->resources, SOIL1_TEX_ID);
    destroy_texture_resources(&scene->resources, SOIL2_TEX_ID);
    destroy_texture_resources(&scene->resources, TILES_TEX_ID);
    destroy_texture_resources(&scene->resources, WOOD1_TEX_ID);
    destroy_texture_resources(&scene->resources, WOOD2_TEX_ID);
}
