cmake_minimum_required(VERSION 3.8)

project(intrinsics
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Test app for exploring intrinsics use"
)

add_executable(intrinsics
	main.c
)

target_include_directories(intrinsics PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(intrinsics PUBLIC
	engine
)
