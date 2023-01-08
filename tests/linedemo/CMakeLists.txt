cmake_minimum_required(VERSION 3.8)

project(linedemo
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Random line drawing demo"
)

add_executable(linedemo
	main.c
)

target_include_directories(linedemo PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(linedemo PUBLIC
	engine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(linedemo PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(linedemo PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
