cmake_minimum_required(VERSION 3.8)

project(enginedemo
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Library for supporting a grid-based scene"
)

add_executable(enginedemo
	main.c
)

target_include_directories(enginedemo PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(enginedemo PUBLIC
	engine
	utils
)
