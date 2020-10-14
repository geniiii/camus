set(nuklear_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/nuklear/)

mark_as_advanced(nuklear_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(nuklear
	REQUIRED_VARS nuklear_INCLUDE_DIR
	VERSION_VAR nuklear_VERSION
)

if(nuklear_FOUND AND NOT TARGET nuklear::nuklear)
	add_library(nuklear::nuklear INTERFACE IMPORTED)
	set_target_properties(nuklear::nuklear PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${nuklear_INCLUDE_DIR}
	)
endif()
