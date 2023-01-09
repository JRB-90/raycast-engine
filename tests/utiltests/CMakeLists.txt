cmake_minimum_required(VERSION 3.8)

project(utiltests
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Tests for the utils library"
)

add_executable(utiltests
	main.c
)

target_include_directories(utiltests PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(utiltests PUBLIC
	utils
)
