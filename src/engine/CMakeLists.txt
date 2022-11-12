cmake_minimum_required(VERSION 3.8)

project(engine
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Library for all common engine components"
)

add_library(engine STATIC
    engine_screen.c
	engine_color.c
	engine_logic.c
)

target_include_directories(engine PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
)
