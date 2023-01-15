cmake_minimum_required(VERSION 3.8)

project(engine
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Library for all common engine components"
)

add_library(engine STATIC
	engine_color.c
	engine_rayengine.c
    engine_draw.c
	engine_math.c
    engine_resource.c
)

target_include_directories(engine PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(engine PUBLIC
	crossplatform
)

if (UNIX)
	target_link_libraries(engine PUBLIC
		m
	)
endif()