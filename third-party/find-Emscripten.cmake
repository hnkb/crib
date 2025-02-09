
if (NOT USE_EMSCRIPTEN)
	return()
endif()

# We want to make sure the Emscripten toolchain file is set
# Otherwise, the project will be compiled with the default toolchain and not as a WASM module

if (NOT CMAKE_TOOLCHAIN_FILE)
	# First, try the EMSDK environment variable (set by emsdk_env.sh)
	# See https://emscripten.org/docs/tools_reference/emsdk.html#how-do-i-change-the-currently-active-sdk-version
	if (DEFINED ENV{EMSDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")

	# If EMSDK is not set, try to find Emscripten via Homebrew
	# This is probably the most common way to install Emscripten on macOS
	else()
		execute_process(
			COMMAND brew --prefix emscripten
			OUTPUT_VARIABLE HOMEBREW_EMSCRIPTEN_PREFIX
			OUTPUT_STRIP_TRAILING_WHITESPACE)
		if (HOMEBREW_EMSCRIPTEN_PREFIX)
			set(CMAKE_TOOLCHAIN_FILE "${HOMEBREW_EMSCRIPTEN_PREFIX}/libexec/cmake/Modules/Platform/Emscripten.cmake")
		endif()
	endif()
endif()

if (CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Using Emscripten toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else()
	message(FATAL_ERROR "Emscripten toolchain file not found. Please ensure Emscripten is installed and EMSDK environment variable is set, or Emscripten is installed via Homebrew.")
endif()
