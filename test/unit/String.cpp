
#include <doctest.h>
#include <string>

using namespace std::string_literals;


TEST_CASE("Ensure source files are treated as UTF-8-encoded by compiler")
{
	CHECK("سلام"s == "\u0633\u0644\u0627\u0645"s);
	CHECK(L"سلام"s == L"\u0633\u0644\u0627\u0645"s);
	CHECK(u8"سلام"s == u8"\u0633\u0644\u0627\u0645"s);
	CHECK(u"سلام"s == u"\u0633\u0644\u0627\u0645"s);
	CHECK(U"سلام"s == U"\u0633\u0644\u0627\u0645"s);

	// these fail to link on clang-14 due to compiler bug
	// see https://github.com/llvm/llvm-project/issues/55560

	// because current GitHub Actions runner has clang-14 by default I need a fix:
	//  - manually override the runner action to use clang-15
	//  - use the workaround given in the link:
	//    static u16string workaround(const char16_t* a, const char16_t* b) { return {a, b}; }
	//  - don't use ""s
	//  - remove clang build action for  ubuntu

	// my current solution is to postpone merging this test for now, hopefully in a few days
	// GitHub Actions ubuntu runner will use newer clang version.
}


#if defined(_WIN32)
#	include <crib/Platform/Win>

TEST_CASE("Ensure Windows console codepage is automatically set to UTF-8")
{
	CHECK(GetConsoleCP() == CP_UTF8);
	CHECK(GetConsoleOutputCP() == CP_UTF8);
}
#endif
