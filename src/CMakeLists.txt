cmake_minimum_required(VERSION 3.8)

add_subdirectory("time")
add_subdirectory("engine")
add_subdirectory("gridengine")
add_subdirectory("sceneview")
add_subdirectory("enginedemo")
add_subdirectory("shapefilegen")
add_subdirectory("linedemo")
add_subdirectory("rectdemo")

if (USE_SDL)
	add_subdirectory("sdl")
elseif(RPI)
	add_subdirectory("rpi")
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
