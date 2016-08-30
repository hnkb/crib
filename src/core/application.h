
#pragma once

#include "window.h"
#include "utility.h"


namespace crib
{
	namespace core
	{

		class application
		{
		public:
			application() {}

			application(const application& other) = delete;
			application(application&& other) = delete;
			application& operator=(const application& other) = delete;
			application& operator=(application&& other) = delete;

			virtual ~application() {}

			int run();

		private:
			utility::com_initialize com_init;
			window main;
		};

	}
}
