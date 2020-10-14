if(GIT_FOUND)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		WORKING_DIRECTORY ${camus_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_SHA1
		OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

configure_file(${camus_SOURCE_DIR}/include/util/config.h.in
			   ${camus_SOURCE_DIR}/include/util/config.h)

file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS ${camus_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS ${camus_SOURCE_DIR}/include/*.h)

add_executable(camus ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(camus PUBLIC ../include)

target_link_libraries(camus PRIVATE sdl2 nuklear::nuklear OpenGL::GL OpenGL::GLU GLEW::glew_s nfd::nfd)

source_group(TREE ${camus_SOURCE_DIR}/include
			 PREFIX "Header Files"
			 FILES ${HEADER_LIST})