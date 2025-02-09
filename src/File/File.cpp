#include <utils/File.h>

using namespace std::filesystem;


#if defined(__EMSCRIPTEN__)

path File::transform(const path& filename)
{
	return filename;
}

#else

extern path fileSystemRoot;

path File::transform(const path& filename)
{
	return fileSystemRoot / filename;
	// return "../../src/assets" / filename;
}

#endif
