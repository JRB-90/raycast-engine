cmake_minimum_required(VERSION 3.8)

project(time
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Cross platform time tools"
)

add_library(time STATIC
    time_helper.c
)

target_include_directories(time PUBLIC
	"${PROJECT_INCLUDE_DIR}/time"
)
