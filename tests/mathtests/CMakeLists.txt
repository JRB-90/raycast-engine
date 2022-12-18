cmake_minimum_required(VERSION 3.8)

project(mathtests
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Tests for the maths library"
)

add_executable(mathtests
	main.c
)

target_include_directories(mathtests PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
)

target_link_libraries(mathtests PUBLIC
	engine
)
