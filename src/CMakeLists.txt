cmake_minimum_required(VERSION 3.8)

add_subdirectory("time")
add_subdirectory("engine")
add_subdirectory("enginedemo")

if (USE_SDL)
    SET(SDL2_DIR $ENV{SDL2_DIR})
	add_subdirectory("sdl")
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()