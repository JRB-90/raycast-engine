cmake_minimum_required(VERSION 3.8)

project(engine
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Library for all common engine components"
)

add_library(engine STATIC
	engine_types.c
)

target_include_directories(engine PUBLIC
	${PROJECT_INCLUDE_DIR}
)
