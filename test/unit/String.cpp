
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
}


#if defined(_WIN32)
#	include <crib/Platform/Win>

TEST_CASE("Ensure Windows console codepage is automatically set to UTF-8")
{
	CHECK(GetConsoleCP() == CP_UTF8);
	CHECK(GetConsoleOutputCP() == CP_UTF8);
}
#endif
