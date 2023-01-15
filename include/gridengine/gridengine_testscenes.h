#pragma once

/*
    Helper functions for creating some example scenes for use in testing.
*/

#include "gridengine_scene.h"
#include "engine/engine_rayengine.h"

/// <summary>
/// Creates a new instance of a grid scene, with simple geometry.
/// </summary>
/// <param name="name">Name to assign the scene.</param>
/// <param name="screenFormat">Format of the rendering screen.</param>
/// <returns>New scene instance. NULL if an error occcurred.</returns>
extern grid_scene* gridengine_create_test_scene1(
    const char* const name,
    const screen_format* const screenFormat
);

/// <summary>
/// Creates a new instance of a grid scene, with complex geometry.
/// </summary>
/// <param name="name">Name to assign the scene.</param>
/// <param name="screenFormat">Format of the rendering screen.</param>
/// <returns>New scene instance. NULL if an error occcurred.</returns>
extern grid_scene* gridengine_create_test_scene2(
    const char* const name,
    const screen_format* const screenFormat
);

/// <summary>
/// Destroys a scene instance.
/// </summary>
/// <param name="scene">Scene to destroy.</param>
extern void gridengine_destroy_test_scene(grid_scene* scene);
