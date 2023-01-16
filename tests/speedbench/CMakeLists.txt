cmake_minimum_required(VERSION 3.8)

project(speedbench
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Test bench for testing speed improvements"
)

add_executable(speedbench
	main.c
)

target_include_directories(speedbench PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(speedbench PUBLIC
	engine
	gridengine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(speedbench PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(speedbench PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()

add_custom_command(
	TARGET speedbench POST_BUILD
    COMMAND 
		${CMAKE_COMMAND} -E copy_directory
		${PROJECT_DATA_DIR}/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures
)
