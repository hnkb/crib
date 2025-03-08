
#pragma once

#include <filesystem>
#include <string>
#include <vector>


class File
{
public:
	File(const std::filesystem::path& filename, const std::string& mode)
	{
		// make sure path exists so we can actually create file
		if (mode.find('w') != std::string::npos)
			std::filesystem::create_directories(filename.parent_path());
		handle = fopen(filename.u8string().c_str(), mode.c_str());
	}
	~File()
	{
		fclose(handle);
		printf("File destructor called.\n");
	}

	size_t size()
	{
		auto current = ftell(handle);
		fseek(handle, 0, SEEK_END);
		auto length = ftell(handle);
		fseek(handle, current, SEEK_SET);
		return length;
	}

	operator FILE*() { return handle; }

private:
	FILE* handle = nullptr;



public:
	template <class Type = uint8_t>
	static std::vector<Type> readAll(const std::filesystem::path& filename)
	{
		File file(filename, "rb");
		std::vector<Type> buffer(file.size() / sizeof(Type));
		fread(buffer.data(), sizeof(Type), buffer.size(), file);
		return buffer;
	}

	template <class Type>
	static void writeAll(const Type* data, size_t size, const std::filesystem::path& filename)
	{
		std::filesystem::create_directories(filename.parent_path());
		File file(filename, "wb");
		fwrite(data, sizeof(Type), size, file);
	}

	template <class Type, size_t size>
	static void writeAll(const Type (&array)[size], const std::filesystem::path& filename)
	{
		return writeAll(array, size, filename);
	}

	template <class Type>
	static void writeAll(const Type& data, const std::filesystem::path& filename)
	{
		return writeAll(data.data(), data.size(), filename);
	}



	class Pack
	{
	public:
		Pack(const std::filesystem::path& filename, const char mode, const char signature[6]);
		~Pack() { flush(); }

		void flush();

		const char mode;
		const std::filesystem::path filename;


		template <typename Type>
		void add(
			const std::string& name,
			const Type* data,
			size_t size,
			const char* typeinfo = nullptr)
		{
			add(name, (uint8_t*)data, size * sizeof(Type), typeid(Type).name());
		}

		template <class Type, size_t size>
		void add(const std::string& name, const Type (&array)[size])
		{
			add(name, array, size);
		}

		// template <class Type, template <class, class...> class Container, class... Rest>
		// void add(const std::string& name, const Container<Type, Rest...>& container)
		template <class Container>
		void add(const std::string& name, const Container& container)
		{
			add(name, container.data(), container.size());
		}

	private:
		std::unique_ptr<File> file;

		struct
		{
			char signature[6];
			uint16_t version = 1;
			uint64_t descriptorOffset = 0;
		} header;

		bool dirty = false;
	};
};


template <>
void File::Pack::add(const std::string&, const uint8_t*, size_t, const char*);


#include <iostream>
using namespace std;





#if defined(__GNUC__) || defined(__clang__)
#	include <cxxabi.h>
#	include <memory>

std::string demangle(const char* name)
{
	int status;
	std::unique_ptr<char, void (*)(void*)>
		res { abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free };
	return (status == 0) ? res.get() : name;
}
#elif defined(_MSC_VER)
#include <regex>
std::string demangle(const char* name)
{
	std::regex structRegex(R"(^(struct |class |enum ))");
	return std::regex_replace(name, structRegex, "");
}
#endif


File::Pack::
	Pack(const std::filesystem::path& filename, const char mode, const char signature[6])
	: filename(filename),
	  mode(mode)
{
	// modes to support: r, w, a, x
	// r: read, w: write, a: append, x: write but fail if exists

	if (mode == 'x' && std::filesystem::exists(filename))
		throw std::runtime_error("File exists");

	file = std::make_unique<File>(filename, string(1, mode));
	if (!*file)
		throw std::runtime_error("Unable to open file");

	// if (mode == 'r' || mode == 'a')
	// {
	// 	// TODO: append to empty file should be valid
	// 	if (file->size() < sizeof(header))
	// 		throw std::runtime_error("Invalid file size");
	// 	fread(&header, sizeof(header), 1, *file);
	// }

	if (mode == 'w' || mode == 'x')
	{
		copy_n(signature, 6, header.signature);
		fwrite(&header, sizeof(header), 1, *file);
	}

	cout << "Signatue: " << signature << endl;
}

void File::Pack::flush()
{
	if (dirty)
	{
		fseek(*file, 0, SEEK_SET);
		fwrite(&header, sizeof(header), 1, *file);
		dirty = false;
	}
	cout << "Flushing\n";
}


template <>
void File::Pack::
	add(const std::string& name, const uint8_t* data, size_t size, const char* typeinfo)
{
	cout << "Pack '" << name << "' with type '" << demangle(typeinfo);
	cout << "' and size " << size << " bytes is added: " << data << "\n";
}


struct float2
{
	float x;
	float y;
	float2(float x, float y) : x(x), y(y) {}
};



int main()
{
	cout << "Hello\n";
	File::Pack p("C:\\Users\=hani\\OneDrive\\Desktop\\test.bin", 'w', "123456");
	cout << p.filename << ", " << p.mode << endl;

	vector<float2> values;
	values.push_back({ 1, 2 });
	values.push_back({ 3, 4 });

	p.add("test", "Hello", 5);
	p.add("test", string("Tschuss!"));
	p.add("test", "Auf wiedersehen...");
	p.add("test", vector<int>({ 65, 66, 67 }));
	p.add("test", values);

	return 0;
}
