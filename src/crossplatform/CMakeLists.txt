cmake_minimum_required(VERSION 3.8)

project(crossplatform
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Common cross platform tools"
)

add_library(crossplatform STATIC
    crossplatform_file.c
    crossplatform_time.c
)

target_include_directories(crossplatform PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)
