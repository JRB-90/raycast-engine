cmake_minimum_required(VERSION 3.8)

project(rayverts
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Test for vertical raycast strip rendering"
)

add_executable(rayverts
	main.c
)

target_include_directories(rayverts PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(rayverts PUBLIC
	engine
	gridengine
	time
)

if(USE_SDL)
	target_link_libraries(rayverts PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(rayverts PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
