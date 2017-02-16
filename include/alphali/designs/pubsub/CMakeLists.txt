# Set source files
SET(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/death_publisher.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/death_subscriber.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/publisher.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/subscriber.cpp
	PARENT_SCOPE
)

# Set header files
SET(HEADERS
	${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/death_publisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/death_subscriber.h
	${CMAKE_CURRENT_SOURCE_DIR}/publisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/subscriber.h
	PARENT_SCOPE
)
