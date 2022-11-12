cmake_minimum_required(VERSION 3.8)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(raycast
    VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Experiments in raycast 2.5D graphics"
)

add_subdirectory("src/enginedemo")