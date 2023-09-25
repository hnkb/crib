
foreach (CXX_STD 17 20)

	set(TARGET_NAME "crib-test-cpp${CXX_STD}")

	add_executable(${TARGET_NAME}
		"unit/Graphics/Camera.cpp"
		"unit/Math/Vec_int.cpp"
		"unit/main.cpp"
	)

	target_compile_features(${TARGET_NAME} PRIVATE "cxx_std_${CXX_STD}")
	include("${crib_SOURCE_DIR}/src/Platform/CMake/Compiler-config.cmake")

	target_link_libraries(${TARGET_NAME}
		PRIVATE
			crib
			doctest
	)

	add_test(
		NAME ${TARGET_NAME}
		COMMAND ${TARGET_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)

endforeach()
