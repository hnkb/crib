
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
