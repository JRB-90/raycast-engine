cmake_minimum_required(VERSION 3.8)

project(sdlsubsystem
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "SDL implementation of the engine backend"
)

find_package(SDL2 REQUIRED)

add_library(sdlsubsystem STATIC
	sdl_subsystem.c
)

target_include_directories(sdlsubsystem PUBLIC
	${PROJECT_INCLUDE_DIR}
	${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sdlsubsystem PUBLIC
	engine
	${SDL2_LIBRARIES}
)