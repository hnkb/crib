
#include <crib/File>

using namespace crib;
using namespace std;


#if defined(_WIN32)

//#	include <Crib/Platform/Windows.h>
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>

//using namespace Crib::Platform;


struct MemMapData
{
	size_t size = 0;
	HANDLE fileHandle = nullptr;
	HANDLE fileMapHandle = nullptr;
};

template <>
File::MemoryMapped<uint8_t>::MemoryMapped(const std::filesystem::path& filename)
{
	auto data = new MemMapData;
	base = data;

	auto action = "opening file '" + filename.u8string() + "'";

	data->fileHandle = CreateFileW(
		filename.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN /*0*/,
		nullptr);
	if (data->fileHandle == INVALID_HANDLE_VALUE)
		// throw CribError("opening '" + filename.u8string() + "'", "CreateFileW");
		// throw Windows::Error(action.c_str(), "CreateFileW", CurrentSourceLocation);
		throw system_error(error_code(GetLastError(), system_category()));

	// read file size
	{
		LARGE_INTEGER temp;
		GetFileSizeEx(data->fileHandle, &temp);
		data->size = static_cast<size_t>(temp.QuadPart);
	}

	data->fileMapHandle =
		CreateFileMappingW(data->fileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (data->fileMapHandle == nullptr)
		//throw Windows::Error(action.c_str(), "CreateFileMappingW", CurrentSourceLocation);
		throw system_error(error_code(GetLastError(), system_category()));

	startAddress = (uint8_t*)MapViewOfFile(data->fileMapHandle, FILE_MAP_READ, 0, 0, 0);
	if (startAddress == nullptr)
		//throw Windows::Error(action.c_str(), "MapViewOfFile", CurrentSourceLocation);
		throw system_error(error_code(GetLastError(), system_category()));
}

template <>
File::MemoryMapped<uint8_t>::~MemoryMapped()
{
	auto data = reinterpret_cast<MemMapData*>(base);

	if (startAddress != nullptr)
		UnmapViewOfFile(startAddress);

	if (data->fileMapHandle != nullptr)
		CloseHandle(data->fileMapHandle);

	if (data->fileHandle != INVALID_HANDLE_VALUE)
		CloseHandle(data->fileHandle);

	delete data;
}

#else

// #	include <Crib/Platform/POSIX.h>
#	include <fcntl.h>
#	include <unistd.h>
#	include <sys/mman.h>
#	include <sys/stat.h>


struct MemMapData
{
	size_t size = 0;
	int file = -1;
};

template <>
File::MemoryMapped<uint8_t>::MemoryMapped(const std::filesystem::path& filename)
{
	auto data = new MemMapData;
	base = data;

	// auto action = "opening file '" + filename.u8string() + "'";

	data->file = open(filename.c_str(), O_RDONLY);
	if (data->file == -1)
		// throw Platform::POSIX::Error(action.c_str(), "open", CurrentSourceLocation);
		throw system_error(error_code(errno, generic_category()));

	struct stat sb;
	if (fstat(data->file, &sb) == -1)
		// throw Platform::POSIX::Error(action.c_str(), "fstat", CurrentSourceLocation);
		throw system_error(error_code(errno, generic_category()));

	data->size = sb.st_size;

	startAddress = (uint8_t*)mmap(nullptr, data->size, PROT_READ, MAP_PRIVATE, data->file, 0);

	if (startAddress == MAP_FAILED)
		// throw Platform::POSIX::Error(action.c_str(), "mmap", CurrentSourceLocation);
		throw system_error(error_code(errno, generic_category()));

	// TODO: we can close file here
}

template <>
File::MemoryMapped<uint8_t>::~MemoryMapped()
{
	auto data = reinterpret_cast<MemMapData*>(base);

	if (startAddress != MAP_FAILED)
		munmap(startAddress, data->size);

	if (data->file != -1)
		close(data->file);

	delete data;
}

#endif


template <>
size_t File::MemoryMapped<uint8_t>::size() const
{
	return reinterpret_cast<MemMapData*>(base)->size;
}
