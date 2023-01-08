cmake_minimum_required(VERSION 3.8)

project(teartest
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Demo program for the raycast engine"
)

add_executable(teartest
	main.c
)

target_include_directories(teartest PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(teartest PUBLIC
	engine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(teartest PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(teartest PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
