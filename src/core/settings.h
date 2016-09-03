
#pragma once

#include <map>
#include <string>


namespace crib
{
	namespace core
	{

		class settings
		{
		public:
			settings();

			~settings();

			int get(const std::wstring key, const int predefined) const { auto f = values.find(key); return f == values.end() ? predefined : std::stoi(f->second); }
			void set(const std::wstring key, const int value) { values[key] = std::to_wstring(value); }

		private:
			std::map<std::wstring, std::wstring> values;
		};

	}
}
