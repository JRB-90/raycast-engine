cmake_minimum_required(VERSION 3.8)

add_subdirectory("shapefilegen")
add_subdirectory("sceneview")

if (USE_SDL)
	add_subdirectory("textureconv")
endif()
