
#include <crib/File>

using namespace crib;
using namespace std;


#ifdef _WIN32

#	include <crib/Platform/Win>

filesystem::path App::executablePath()
{
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, path, MAX_PATH);
	// TODO: handle error
	return path;
}

#elif 1

#	include <unistd.h>
#	include <limits.h>

filesystem::path App::executablePath()
{
	char result[PATH_MAX];
	auto count = readlink("/proc/self/exe", result, PATH_MAX);
	return string(result, (count > 0) ? count : 0);
}

#else

#	include <unistd.h>
#	include <sys/stat.h>
#	include <limits.h>
#	include <vector>

// this is unnecessarily too complicated
// as lstat will return 0 for "/proc/self/exe"

filesystem::path App::executablePath()
{
	const auto link = "/proc/self/exe";

	struct stat sb;
	if (lstat(link, &sb) == -1)
		throw std::system_error(errno, std::generic_category());

	vector<char> buffer(
		sb.st_size ?
			// Add one to the link size, so that we can determine whether the buffer returned by
			// readlink() was truncated
			sb.st_size + 1 :
			// Some magic symlinks under (for example) /proc and /sys report 'st_size' as zero.
			// In that case, take PATH_MAX as a "good enough" estimate.
			PATH_MAX);

	auto count = readlink(link, buffer.data(), buffer.size());

	if (count == -1)
		throw std::system_error(errno, std::generic_category());

	if (count != (signed)buffer.size())
		printf(
			"\n\n => Buffer may have been truncated!!!  %zd != %zd  <==\n",
			count,
			buffer.size());

	return string(buffer.data(), count);
}

#endif
