
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest.h")
		message("Downloading doctest library from GitHub...")
		file(DOWNLOAD
			"https://github.com/doctest/doctest/releases/latest/download/doctest.h"
			"${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest.h"
		)
	endif()

	add_library(doctest INTERFACE)
	target_include_directories(doctest INTERFACE "doctest")

endif()


add_library(glad STATIC "glad/src/glad.c")
if (WIN32)
	target_sources(glad PRIVATE "glad/src/glad_wgl.c")
elseif (APPLE)
else()
	target_sources(glad PRIVATE "glad/src/glad_glx.c")
endif()
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/include")




find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

#if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/repo/CMakeLists.txt")
#    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#endif()

add_library (ttf2mesh STATIC "ttf2mesh/ttf2mesh.c")
target_include_directories(ttf2mesh PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ttf2mesh")



if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/overpass-font/Overpass-Bold.ttf")
	message("Downloading Overpass font from GitHub...")
	file(DOWNLOAD
		"https://raw.githubusercontent.com/RedHatOfficial/Overpass/master/fonts/ttf/Overpass-Bold.ttf"
		"${CMAKE_CURRENT_SOURCE_DIR}/overpass-font/Overpass-Bold.ttf"
	)
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/overpass-font/Overpass-Bold.ttf"
	DESTINATION "${CMAKE_BINARY_DIR}/assets/")
