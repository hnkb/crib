

add_library (doctest INTERFACE)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doctest/")

	target_include_directories (doctest INTERFACE "doctest")

else()

	message ("Fetching doctest from github...")

	include (FetchContent)
	FetchContent_Declare (doctest GIT_REPOSITORY "https://github.com/onqtam/doctest.git")
	FetchContent_GetProperties (doctest)

	if (NOT doctest_POPULATED)
		FetchContent_Populate (doctest)
		target_include_directories (doctest INTERFACE "${doctest_SOURCE_DIR}/doctest/")
	endif()

endif()



function (SETUP_TARGET TARGET_NAME)

	add_executable (${TARGET_NAME}
		"Application.cpp")

	target_link_libraries (${TARGET_NAME} PRIVATE Crib doctest)

	if (MSVC)
		target_compile_options (${TARGET_NAME} PRIVATE /utf-8)
	elseif (WIN32)
		target_link_libraries (${TARGET_NAME} PRIVATE -static)
	endif()

	if (WIN32)
		target_sources (${TARGET_NAME} PRIVATE
			"Windows.cpp")
	endif()

	add_test (${TARGET_NAME} ${TARGET_NAME})

endfunction()



setup_target (CribTests_cpp17)

setup_target (CribTests_cpp20)
target_compile_features (CribTests_cpp20 PRIVATE cxx_std_20)  # cxx_std_20 requires CMake 3.12 and later
