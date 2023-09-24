
#include <crib/Platform/Win>
#include <strsafe.h>

using namespace crib::Platform::Win;



// WideString& WideString::operator=(const std::string& utf8)
WideString& WideString::operator=(std::string_view utf8)
{
	// if (utf8.empty())
	//{
	//	utf16.resize(1);
	//	utf16.front() = 0;
	// }
	// else
	{
		auto bufferSize = MultiByteToWideChar(
			CP_UTF8,
			0,
			utf8.data(),
			(int)utf8.size() + 1,  // also include null-terminator after string to output
			nullptr,
			0);

		if (bufferSize == 0)
			throw Error();

		utf16.resize((size_t)bufferSize);

		MultiByteToWideChar(
			CP_UTF8,
			0,
			utf8.data(),
			(int)utf8.size() + 1,
			utf16.data(),
			capacity());
	}

	return *this;
}


std::string WideString::utf8(const wchar_t* utf16)
{
	// we either have size (like when FormatMessageW returns size, or when using
	// std::wstring)
	// 			   => directly use size+1 to also copy null-terminator
	// or we     have bounds (like when we have vector<> buffer size)
	//			   => use StringCchLengthW to get size?
	// or we     don't have any knowledge (what to do here?)
	//             => use -1? is it safe?

	auto bufferSize = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, nullptr, 0, nullptr, nullptr);

	if (bufferSize == 0 && *utf16 != 0)
		throw Error();

	std::string utf8(
		(size_t)bufferSize - 1,
		0);  // bufferSize includes null-terminator, so we subtract one for string

	WideCharToMultiByte(
		CP_UTF8,
		0,
		utf16,
		-1,
		&utf8[0],
		(int)utf8.size() + 1,  // adding one to output size, to include null-terminator
		nullptr,
		nullptr);

	return utf8;
}


int WideString::length() const
{
	size_t len = 0;

	throwOnError(StringCchLengthW(utf16.data(), utf16.size(), &len));

	return (int)len;
}
