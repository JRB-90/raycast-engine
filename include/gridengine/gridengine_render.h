#pragma once

/*
    Helper functions for rendering a grid based scene.
*/

#include "gridengine/gridengine_scene.h"
#include "engine/engine_rayengine.h"
#include <stdbool.h>

/// <summary>
/// Renders a top down view of the grid scene.
/// </summary>
/// <param name="engine">Engine to render to.</param>
/// <param name="scene">Scene to render.</param>
/// <param name="mapPosition">Position of the map on screen.</param>
/// <param name="drawGrid">If true, draws gridlines.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_topdown_scene(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    bool drawGridLines
);

/// <summary>
/// Renders the player onto a top down view of the grid scene. 
/// </summary>
/// <param name="engine">Engine to render to.</param>
/// <param name="mapPosition">Position of the map on screen.</param>
/// <param name="player">Player to render.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_topdown_player(
    const rayengine* const engine,
    const map_pos* const mapPosition,
    const player_obj* const player
);

/// <summary>
/// Renders the scene sprites onto a top down view of the grid scene.
/// </summary>
/// <param name="engine">Engine to render to.</param>
/// <param name="scene">Scene containing the sprites to render.</param>
/// <param name="mapPosition">Position of the map on screen.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_topdown_sprites(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition
);

/// <summary>
/// Renders the player rays onto a top down view of the grid scene.
/// </summary>
/// <param name="engine">Engine to render to.</param>
/// <param name="scene">Scene containing the sprites to render.</param>
/// <param name="mapPosition">Position of the map on screen.</param>
/// <param name="player">Player to render.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_topdown_rays(
    const rayengine* const engine,
    const grid_scene* const scene,
    const map_pos* const mapPosition,
    const player_obj* const player
);

/// <summary>
/// Renders a grid scene in first-person view.
/// </summary>
/// <param name="engine">Egnine to render to.</param>
/// <param name="scene">Scene to render.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_firstperson(
    const rayengine* const engine,
    const grid_scene* const scene
);

/// <summary>
/// Renders a vertical strip of pixels at a given screen column using the
/// proved traversal data.
/// </summary>
/// <param name="engine">Engine to render to.</param>
/// <param name="scene">Scene to render.</param>
/// <param name="traverseResult">The result of the ray projection travserval.</param>
/// <param name="colIndex">The screen column to draw the strip at.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_vertical_strip16(
    const rayengine* const engine,
    const grid_scene* const scene,
    const traverse_result* const traverseResult,
    int colIndex
);

/// <summary>
/// Renders a vertical strip of pixels at a given screen column using the
/// proved traversal data.
/// </summary>
/// <param name="engine">Engine to render to.</param>
/// <param name="scene">Scene to render.</param>
/// <param name="traverseResult">The result of the ray projection travserval.</param>
/// <param name="colIndex">The screen column to draw the strip at.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_vertical_strip32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const traverse_result* const traverseResult,
    int colIndex
);

/// <summary>
/// Iterates through the list of scene sprites and renders them to the screen.
/// </summary>
/// <param name="engine">Engine to render the sprites to.</param>
/// <param name="scene">Scene containing the list of sprites to render.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_sprites(
    const rayengine* const engine,
    const grid_scene* const scene,
    const frame2d* const playerPos
);

/// <summary>
/// Renders the provided sprite to the screen.
/// </summary>
/// <param name="engine">Engine to render the sprite to.</param>
/// <param name="scene">Scene containing the sprite.</param>
/// <param name="sprite">The sprite to render.</param>
/// <param name="x">X offset of the sprite.</param>
/// <param name="height">Height of the sprite.</param>
/// <param name="distanceToSprite">Distance from the player to the sprite.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_sprite16(
    const rayengine* const engine,
    const grid_scene* const scene,
    const sprite_obj* const sprite,
    int x,
    int height,
    float distanceToSprite
);

/// <summary>
/// Renders the provided sprite to the screen.
/// </summary>
/// <param name="engine">Engine to render the sprite to.</param>
/// <param name="scene">Scene containing the sprite.</param>
/// <param name="sprite">The sprite to render.</param>
/// <param name="x">X position of the sprite in screen space.</param>
/// <param name="height">Height of the sprite.</param>
/// <param name="distanceToSprite">Distance from the player to the sprite.</param>
/// <returns>Non-zero if no error occurred.</returns>
extern int gridengine_render_sprite32(
    const rayengine* const engine,
    const grid_scene* const scene,
    const sprite_obj* const sprite,
    int x,
    int height,
    float distanceToSprite
);
