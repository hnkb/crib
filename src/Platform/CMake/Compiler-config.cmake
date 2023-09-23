
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
			/Zc:__cplusplus
			/permissive- /W4 /WX
			/fp:fast
	)

	if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		target_compile_options(${TARGET_NAME} PRIVATE /Qpar)
	endif()
else()
	target_compile_options(${TARGET_NAME}
		PRIVATE
			-Wall -Wextra -pedantic -Werror
			-ffast-math  # includes -fno-math-errno, -funsafe-math-optimizations, -ffinite-math-only, -fno-rounding-math, -fno-signaling-nans, -fcx-limited-range and -fexcess-precision=fast
	)
endif()

# enable more aggressive optimization which are supported by Visual C++ 2019
if (MSVC_VERSION GREATER_EQUAL 1920 AND NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	string(REGEX REPLACE "\\/Ob[1-2]" "/Ob3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REGEX REPLACE "\\/Ob[1-2]" "/Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	string(REGEX REPLACE "\\/Ob[1-2]" "/Ob3" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()



#
# processor architecture
#

if (TARGET_NAME STREQUAL "crib") #<====== or use ${TARGET_NAME} instead of crib (but they are all PUBLIC)
	if (CRIB_USE_AVX)
		# AVX settings are applied globally so the whole program uses VEX instruction
		# coding, avoiding SSE2/AVX transition penalty between different modules.
		if (MSVC)
			target_compile_options(crib PUBLIC /arch:AVX)
			#target_compile_options(crib PUBLIC /arch:AVX2)
		else()
			target_compile_options(crib PUBLIC -mavx)
			#target_compile_options(crib PUBLIC -mavx2 -mfma)
		endif()
		message(STATUS "AVX SIMD instruction sets enabled")
	else()
		if (MSVC)
			# 64-bit MSVC does enable SSE by default, but does not define __SSE__ flag
			target_compile_definitions(crib PUBLIC __SSE__)
		else()
			target_compile_options(crib PUBLIC -msse2)
			#target_compile_options(crib PUBLIC -msse4.1)
		endif()
	endif()

	# what about ARM NEON ...?
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
