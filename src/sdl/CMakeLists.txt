cmake_minimum_required(VERSION 3.8)

project(sdlsubsystem
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "SDL implementation of the engine backend"
)

add_library(sdlsubsystem STATIC
	sdl_subsystem.c
)

target_include_directories(sdlsubsystem PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
)