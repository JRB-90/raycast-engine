cmake_minimum_required(VERSION 3.8)

project(steprender
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Test app for viewing the rendering process step by step"
)

add_executable(steprender
	main.c
)

target_include_directories(steprender PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(steprender PUBLIC
	engine
	gridengine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(steprender PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(steprender PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()

add_custom_command(
	TARGET steprender POST_BUILD
    COMMAND 
		${CMAKE_COMMAND} -E copy_directory
		${PROJECT_DATA_DIR}/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures
)
