
if (NOT DEFINED TARGET_NAME)
	set(TARGET_NAME crib)
endif()



#
# compiler flags
#

if (MSVC)
	target_compile_options(${TARGET_NAME}
		PUBLIC
			/utf-8
		PRIVATE
			/permissive- /W4 /WX
	)
else()
	target_compile_options(${TARGET_NAME}
		PRIVATE
			-Wall -Wextra -pedantic -Werror
	)
endif()



#
# output file and directory
#

get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

if (TARGET_TYPE STREQUAL "EXECUTABLE" OR TARGET_TYPE STREQUAL "SHARED_LIBRARY")
	if (WIN32 AND NOT MSVC)
		target_link_libraries(${TARGET_NAME} PRIVATE -static)
	endif()
endif()
