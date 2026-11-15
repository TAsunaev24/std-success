cmake_minimum_required(VERSION 3.22)
project(labyrinth LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(labgen STATIC)
target_sources(labgen PRIVATE src/labgen/field.cpp src/labgen/utils.cpp)

add_subdirectory(raylib)

add_executable(pathtest)
target_sources(pathtest PRIVATE src/tests/pathtest.cpp)
target_link_libraries(pathtest PRIVATE labgen)

add_executable(labtest)
target_sources(labtest PRIVATE src/tests/labtest.cpp)
target_link_libraries(labtest PRIVATE labgen)

add_executable(tablegen)
target_sources(tablegen PRIVATE src/tests/unitTest.cpp)
target_link_libraries(tablegen PRIVATE labgen)

add_executable(labyrinth)
target_sources(labyrinth PRIVATE
	src/frontend/main.cpp
	src/frontend/app.cpp
	src/frontend/ui.cpp
	src/frontend/renderer.cpp
)
target_link_libraries(labyrinth PRIVATE labgen raylib)

include_directories(include/labgen include/frontend raylib/src)
