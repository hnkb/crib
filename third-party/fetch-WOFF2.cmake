
if (TARGET woff2dec AND TARGET woff2common)
	return()
endif()

include("fetch-Brotli.cmake")


FetchContent_Declare(
	woff2
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/woff2"
	GIT_REPOSITORY "https://github.com/google/woff2.git"
	GIT_TAG "v1.0.2"
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL)

# Pass Brotli paths to WOFF2
if (TARGET brotlidec)
	get_target_property(BROTLI_INCLUDE_DIRS brotlidec INTERFACE_INCLUDE_DIRECTORIES)
	set(BROTLIDEC_INCLUDE_DIRS ${BROTLI_INCLUDE_DIRS} CACHE STRING "Brotli include directories")
	set(BROTLIDEC_LIBRARIES brotlidec CACHE STRING "Brotli libraries")
endif()
if (TARGET brotlienc)
	get_target_property(BROTLI_INCLUDE_DIRS brotlienc INTERFACE_INCLUDE_DIRECTORIES)
	set(BROTLIENC_INCLUDE_DIRS ${BROTLI_INCLUDE_DIRS} CACHE STRING "Brotli include directories")
	set(BROTLIENC_LIBRARIES brotlienc CACHE STRING "Brotli libraries")
endif()

# Disable developer and deprecated warnings before including the subdirectory
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "Suppress developer warnings" FORCE)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "Disable deprecated warnings" FORCE)

FetchContent_MakeAvailable(woff2)

# Re-enable developer and deprecated warnings for the rest of the project
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS OFF CACHE BOOL "Suppress developer warnings" FORCE)
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "Disable deprecated warnings" FORCE)

# Add the missing include directory to woff2 target
target_include_directories(woff2common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/woff2/include")
