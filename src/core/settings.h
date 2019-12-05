
#pragma once

#include <map>
#include <string>


namespace Crib
{

	class PersistentSettings
	{
	public:
		PersistentSettings();

		~PersistentSettings();

		int get(const std::wstring key, const int predefined) const { auto f = values.find(key); return f == values.end() ? predefined : std::stoi(f->second); }
		float get(const std::wstring key, const float predefined) const { auto f = values.find(key); return f == values.end() ? predefined : std::stof(f->second); }
		std::wstring get(const std::wstring key, const std::wstring predefined) const { auto f = values.find(key); return f == values.end() ? predefined : f->second; }

		void set(const std::wstring key, const int value) { values[key] = std::to_wstring(value); }
		void set(const std::wstring key, const float value) { values[key] = std::to_wstring(value); }
		void set(const std::wstring key, const std::wstring value) { values[key] = value; }

	private:
		std::map<std::wstring, std::wstring> values;
	};

}
