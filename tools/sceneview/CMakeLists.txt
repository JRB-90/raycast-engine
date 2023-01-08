cmake_minimum_required(VERSION 3.8)

project(sceneview
	VERSION 0.1
    LANGUAGES C
    DESCRIPTION "Tool to view scenes"
)

add_executable(sceneview
	main.c
)

target_include_directories(sceneview PUBLIC
	"${PROJECT_INCLUDE_DIR}"
)

target_link_libraries(sceneview PUBLIC
	engine
	gridengine
	crossplatform
)

if(USE_SDL)
	target_link_libraries(sceneview PUBLIC
		sdlsubsystem
	)
elseif(RPI)
	target_link_libraries(sceneview PUBLIC
		rpisubsystem
	)
else()
	message(FATAL_ERROR
		"No subsystem implementation configured, cannot configure"
	)
endif()
