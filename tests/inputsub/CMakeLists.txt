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
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(inputsub PUBLIC
	engine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(inputsub PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(inputsub PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()