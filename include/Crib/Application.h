
#pragma once

#include "Window.h"
#include "../../src/core/settings.h"
#include "Platform/Windows.h"


namespace crib
{
	namespace core
	{

		class application
		{
		public:
			application() : main(settings) {}

			application(const application& other) = delete;
			application(application&& other) = delete;
			application& operator=(const application& other) = delete;
			application& operator=(application&& other) = delete;

			virtual ~application() {}

			int run();

		private:
			utility::com_initialize com_init;
			settings settings;
			window main;
		};

	}
}
