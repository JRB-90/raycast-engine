cmake_minimum_required(VERSION 3.8)

project(maptest
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Test for the map drawing functions"
)

add_executable(maptest
	main.c
)

target_include_directories(maptest PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(maptest PUBLIC
	engine
	gridengine
	time
)

if(USE_SDL)
	target_link_libraries(maptest PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(maptest PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
