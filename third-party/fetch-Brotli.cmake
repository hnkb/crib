
if (TARGET brotlidec AND TARGET brotlienc)
	return()
endif()

include(FetchContent)


FetchContent_Declare(
	brotli
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/brotli"
	GIT_REPOSITORY "https://github.com/google/brotli.git"
	GIT_TAG "v1.1.0"
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(BROTLI_DISABLE_TESTS ON CACHE BOOL "No need to configure brotli tests" FORCE)

FetchContent_MakeAvailable(brotli)
