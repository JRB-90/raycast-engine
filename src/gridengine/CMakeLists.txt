cmake_minimum_required(VERSION 3.8)

project(gridengine
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Library grid based raycast engine"
)

add_library(gridengine STATIC
    gridengine_scene.c
    gridengine_render.c
)

target_include_directories(gridengine PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(gridengine PUBLIC
	engine
)
