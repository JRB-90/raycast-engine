cmake_minimum_required(VERSION 3.8)

project(shapefilegen
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Generates shape files for the drawing profiler"
)

add_executable(shapefilegen
	main.c
)
