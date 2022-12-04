cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PROJECT_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

project(raycast
    VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Experiments in raycast 2.5D graphics"
)

if(MSVC)
    message("Using MSVC compiler")
    add_compile_options(/W3)

    if(IS_REL)
        add_compile_options(/O2)
    endif()
elseif (UNIX)
    message("Using GCC compiler")
    add_compile_options(-Wall -lm)
    
    if(RPI)
        message("RPI subsystem selected")
        add_compile_options(-lrt -mfloat-abi=hard -mfpu=vfp)
    endif()    

    if (IS_REL)
        add_compile_options(-O6)
    else()
        add_compile_options(-g)
    endif()
endif()

if(USE_SDL)
    message("SDL subsystem selected")
    SET(SDL2_DIR $ENV{SDL2_DIR})
    add_compile_definitions(_USE_SDL)
elseif(RPI)
    add_compile_definitions(_RPI)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()

add_subdirectory("tests")
add_subdirectory("src")
