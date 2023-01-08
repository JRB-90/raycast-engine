cmake_minimum_required(VERSION 3.8)

project(rectdemo
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Random rect drawing demo"
)

add_executable(rectdemo
	main.c
)

target_include_directories(linedemo PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(rectdemo PUBLIC
	engine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(rectdemo PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(rectdemo PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
