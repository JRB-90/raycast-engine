cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

project(raycast
    VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Experiments in raycast 2.5D graphics"
)

add_subdirectory("tests")
add_subdirectory("src")

if (MSVC)
    add_compile_options(/W3)

    if (IS_REL)
        add_compile_options(/O2)
    endif()
elseif (UNIX)
    if (RPI)
        add_compile_definitions(_RPI)
        add_compile_options(-lrt -mfloat-abi=hard -mfpu=vfp)
    endif()

    add_compile_options(-Wall)

    if (IS_REL)
        add_compile_options(-o6)
    else()
        add_compile_options(-g)
    endif()
endif()
