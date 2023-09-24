
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>
#include <filesystem>


int main(int argc, char** argv)
{
	std::cout << "OS: ";
#if defined(_WIN32)
	std::cout << "Windows ";
#elif defined(__APPLE__)
	std::cout << "Apple ";
#elif defined(__linux__)
	std::cout << "Linux ";
#elif defined(__unix__)
	std::cout << "UNIX ";
#endif
#if defined(_POSIX_VERSION)
	std::cout << " (POSIX version: " << _POSIX_VERSION << ")";
#endif


#ifdef _MSC_VER
#	ifdef __clang__
	const auto compilerName = "clang-cl";
	const auto version = __clang_major__;
#	else
	const auto compilerName = "msvc";
	const auto version = _MSC_VER;
#	endif
#else
#	ifdef __clang__
	const auto compilerName = "clang";
	const auto version = __clang_major__;
#	else
	const auto compilerName = "gcc";
	const auto version = __GNUC__;
#	endif
#endif

	std::cout << "\nCompiler: " << compilerName << " " << version << "  (";

	if constexpr (__cplusplus >= 2023'00L)
		std::cout << "C++23";
	else if constexpr (__cplusplus >= 2020'00L)
		std::cout << "C++20";
	else if constexpr (__cplusplus >= 2017'00L)
		std::cout << "C++17";
	else if constexpr (__cplusplus >= 2014'00L)
		std::cout << "C++14";
	else if constexpr (__cplusplus >= 2010'00L)
		std::cout << "C++11";
	else if constexpr (__cplusplus >= 1997'00L)
		std::cout << "C++98";
	else
		std::cout << "pre-standard C++";
	std::cout << ")\n";

#ifndef NDEBUG
	std::cout << "DEBUG\n";
#endif


	std::cout << "Current Path: " << std::filesystem::current_path().string() << "\n\n";


	return doctest::Context(argc, argv).run();
}
