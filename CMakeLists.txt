cmake_minimum_required(VERSION 3.12...3.17)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(camus_VERSION_MAJOR 0)
set(camus_VERSION_MINOR 1)
set(camus_VERSION_PATCH 0)
set(camus_VERSION ${camus_VERSION_MAJOR}.${camus_VERSION_MINOR}.${camus_VERSION_PATCH})

project(camus
	VERSION ${camus_VERSION}
	DESCRIPTION "CHIP-8 Emulator"
	LANGUAGES C)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

set(CMAKE_C_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

find_package(Git)

set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(nuklear REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(src)
