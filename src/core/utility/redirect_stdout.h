
#pragma once

#include <stdexcept>
#include <stdio.h>


namespace crib
{
	namespace core
	{
		namespace utility
		{

			class redirect_stdout
			{
			public:
				redirect_stdout(char* filename)
				{
					if (freopen_s(&stream, filename, "w", stdout))
						throw std::runtime_error("Cannot redirect stdout");
				}

				~redirect_stdout()
				{
					fclose(stream);
				}

			private:
				FILE* stream;
			};

		}
	}
}
