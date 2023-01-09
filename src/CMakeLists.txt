cmake_minimum_required(VERSION 3.8)

add_subdirectory("crossplatform")
add_subdirectory("utils")
add_subdirectory("engine")
add_subdirectory("gridengine")
add_subdirectory("enginedemo")

if (USE_SDL)
	add_subdirectory("sdl")
elseif(RPI)
	add_subdirectory("rpi")
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
