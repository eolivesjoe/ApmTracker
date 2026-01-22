set(SOURCES
    main.cpp
    tracker/ApmTracker.cpp
    window/Window.cpp
	logger/Logger.cpp
	app/App.cpp
)

set(HEADERS
    tracker/ApmTracker.h
    window/Window.h
	app/App.h
	logger/Logger.h
	Constants.h
)

add_executable(ApmTracker ${SOURCES} ${HEADERS})

target_link_libraries(ApmTracker
    user32
    gdi32
    kernel32
)
