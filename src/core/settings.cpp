
#include "settings.h"
#include <Crib/Platform/Windows.h>
#include <vector>

using Crib::PersistentSettings;
using Crib::Platform::Windows::Error;


PersistentSettings::PersistentSettings()
{
	HKEY key;
	LSTATUS err = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\crib", 0, KEY_READ, &key);

	if (err == ERROR_SUCCESS)
	{
		std::vector<wchar_t> name, value;
		DWORD count, maxname, maxval;

		err = RegQueryInfoKeyW(key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &count, &maxname, &maxval, nullptr, nullptr);
		if (err != ERROR_SUCCESS) throw Error(err, "Reading settings from registry");

		name.resize(maxname + 1);
		value.resize(maxval / sizeof(wchar_t) + 1);
		value.back() = 0;

		for (DWORD i = 0; i < count; i++)
		{
			maxname = DWORD(name.size());
			maxval = DWORD((value.size() - 1) * sizeof(wchar_t));

			err = RegEnumValueW(key, i, name.data(), &maxname, nullptr, nullptr, LPBYTE(value.data()), &maxval);
			if (err != ERROR_SUCCESS) throw Error(err, "Reading settings from registry");

			values.emplace(std::make_pair(name.data(), value.data()));
		}

		RegCloseKey(key);
	}
	else if (err != ERROR_FILE_NOT_FOUND)
	{
		throw Error(err, "Reading settings from registry");
	}
}

PersistentSettings::~PersistentSettings()
{
	HKEY key;

	if (RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\crib", 0, NULL, 0, KEY_WRITE, nullptr, &key, nullptr) == ERROR_SUCCESS)
	{
		for (const auto& v : values)
			RegSetValueExW(key, v.first.c_str(), 0, REG_SZ, (BYTE*)v.second.c_str(), DWORD(sizeof(wchar_t) * (v.second.size() + 1)));

		RegCloseKey(key);
	}
}
