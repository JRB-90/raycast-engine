cmake_minimum_required(VERSION 3.8)

project(transformtest
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Test for frame transformations"
)

add_executable(transformtest
	main.c
)

target_include_directories(transformtest PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(transformtest PUBLIC
	engine
	gridengine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(transformtest PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(transformtest PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
