cmake_minimum_required(VERSION 3.8)

project(rpisubsystem
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "SDL implementation of the engine backend"
)

add_library(rpisubsystem STATIC
	rpi_subsystem.c
)

target_include_directories(rpisubsystem PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
)

target_link_libraries(rpisubsystem PUBLIC
	engine
	pthread
)