
#include <crib/File>

using crib::File;


File::File(const std::filesystem::path& filename, const std::string& mode)
{
	// make sure path exists so we can actually create file
	if (mode.find('w') != std::string::npos)
		std::filesystem::create_directories(filename.parent_path());

#ifdef _WIN32
	std::wstring wmode(mode.begin(), mode.end());

	if (auto err = _wfopen_s(&handle, filename.c_str(), wmode.c_str()))
		throw std::system_error(std::error_code(err, std::generic_category()));
#else
	handle = fopen(filename.c_str(), mode.c_str());
#endif

	if (!handle)
		throw std::system_error(std::error_code(errno, std::generic_category()));
}

File::~File()
{
	fclose(handle);
}


#ifdef _WIN32

size_t File::tell() const
{
	return _ftelli64(handle);
}

int File::seek(size_t offset, int origin)
{
	return _fseeki64(handle, offset, origin);
}

#else

size_t File::tell() const
{
	// either long is 64-bit or we are in 32-bit environment
	return ftell(handle);
}

int File::seek(size_t offset, int origin)
{
	return fseek(handle, offset, origin);
}

#endif


size_t File::size()
{
	auto current = tell();
	seek(0, SEEK_END);
	auto length = tell();
	seek(current, SEEK_SET);
	return length;
}
