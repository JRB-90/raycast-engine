cmake_minimum_required(VERSION 3.8)

project(textureconv
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Generates engine compatible texture files from images"
)

find_package(SDL2 REQUIRED)

SET(SDL2_IMAGE_DIR $ENV{SDL2_IMAGE_DIR})
SET(SDL2_IMAGE_INCLUDE_DIRS $ENV{SDL2_IMAGE_INCLUDE_DIRS})
SET(SDL2_IMAGE_LIBRARIES $ENV{SDL2_IMAGE_LIBRARIES})

add_executable(textureconv
	main.c
)

target_include_directories(textureconv PUBLIC
	${PROJECT_INCLUDE_DIR}
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_directories(textureconv PUBLIC
	${SDL2_IMAGE_LIBRARIES}
)

target_link_libraries(textureconv PUBLIC
	engine
	crossplatform
	${SDL2_LIBRARIES}
	SDL2_image
)
