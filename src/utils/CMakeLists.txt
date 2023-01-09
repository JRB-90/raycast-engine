cmake_minimum_required(VERSION 3.8)

project(utils
    VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Common C utility functions"
)

add_library(utils STATIC
    utils_string.c
    utils_list.c
)

target_include_directories(utils PUBLIC
    ${PROJECT_INCLUDE_DIR}
)
