
#include <crib/File>
#include <doctest.h>

using namespace crib;
using namespace std;


TEST_CASE("crib::App::executablePath")
{
	CHECK_NOTHROW(App::executablePath());

	CHECK(!App::executablePath().empty());

	CHECK(filesystem::exists(App::executablePath()));

	CHECK(!filesystem::exists(App::executablePath().replace_extension("_invalid!")));
}
