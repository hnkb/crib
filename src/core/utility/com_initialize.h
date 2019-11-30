
#pragma once

#include "windows_error.h"


namespace crib
{
	namespace core
	{
		namespace utility
		{

			class com_initialize
			{
			public:
				com_initialize()
				{
					throw_if_failed(CoInitialize(nullptr), "COM initialization");
				}

				~com_initialize()
				{
					CoUninitialize();
				}
			};

		}
	}
}
