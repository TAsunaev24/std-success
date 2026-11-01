cmake_minimum_required(VERSION 4.2)
project(labyrinth)

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

add_subdirectory(raylib)

add_executable(pathtest)
target_sources(pathtest PRIVATE src/tests/pathtest.cpp)
target_link_libraries(pathtest PRIVATE labgen)

add_executable(labtest)
target_sources(labtest PRIVATE src/tests/labtest.cpp)
target_link_libraries(labtest PRIVATE labgen)
