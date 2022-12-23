

cmake_minimum_required(VERSION 3.8)

project(textureconv
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Generates engine compatible texture files from images"
)

add_executable(textureconv
	main.c
)
