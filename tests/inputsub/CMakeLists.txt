cmake_minimum_required(VERSION 3.8)

project(inputsub
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Demo program for the raycast engine"
)

add_executable(inputsub
	main.c
)

target_include_directories(inputsub PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
	"${PROJECT_INCLUDE_DIR}/time"
)

target_link_libraries(inputsub PUBLIC
	engine
	time
)

if(USE_SDL)
	target_link_libraries(inputsub PUBLIC
		sdlsubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()