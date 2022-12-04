cmake_minimum_required(VERSION 3.8)

project(drawprofile
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Demo program for the raycast engine"
)

add_executable(drawprofile
	main.c
)

target_include_directories(drawprofile PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
	"${PROJECT_INCLUDE_DIR}/time"
)

target_link_libraries(drawprofile PUBLIC
	engine
	time
)

if(USE_SDL)
	target_link_libraries(drawprofile PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(drawprofile PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()

add_custom_command(
	TARGET drawprofile POST_BUILD
    COMMAND 
		${CMAKE_COMMAND} -E copy_directory
		${PROJECT_DATA_DIR}/shapefiles
        ${CMAKE_CURRENT_BINARY_DIR}/shapefiles
)