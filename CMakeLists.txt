cmake_minimum_required(VERSION 3.22)
project(labyrinth LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(labgen STATIC)
target_sources(labgen
	PRIVATE
		src/backend/field.cpp
		src/backend/utils.cpp
	PUBLIC
		FILE_SET generatorHeaders
		TYPE HEADERS
		BASE_DIRS
			src/backend
		FILES
			src/backend/field.h
			src/backend/utils.h
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(raylib)

add_executable(labyrinth)
target_sources(labyrinth PRIVATE
	src/frontend/main.cpp
	src/frontend/app.cpp
	src/frontend/ui.cpp
)
target_link_libraries(labyrinth PRIVATE labgen raylib)

add_executable(pathtest)
target_sources(pathtest PRIVATE src/tests/pathtest.cpp)
target_link_libraries(pathtest PRIVATE labgen)

add_executable(labtest)
target_sources(labtest PRIVATE src/tests/labtest.cpp)
target_link_libraries(labtest PRIVATE labgen)
