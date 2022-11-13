cmake_minimum_required(VERSION 3.8)

project(rendersub
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Demo program for the raycast engine"
)

add_executable(rendersub
	main.c
)

target_include_directories(rendersub PUBLIC
	"${PROJECT_INCLUDE_DIR}/engine"
	"${PROJECT_INCLUDE_DIR}/time"
)

target_link_libraries(rendersub PUBLIC
	engine
	time
)

if(USE_SDL)
	target_link_libraries(rendersub PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(rendersub PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()