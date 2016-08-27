
#pragma once

#include "window.h"
#include "utility.h"


class application
{
public:
	application() {}

	application(const application& other) { throw std::exception("Copy constructor not supported for application."); }
	application(application&& other) { throw std::exception("Move constructor not supported for application."); }
	application& operator=(const application& other) { throw std::exception("Copy assignment not supported for application."); }
	application& operator=(application&& other) { throw std::exception("Move assignment not supported for application."); }

	~application() {}

	int run();

private:
	com_initialize com_init;
	window main;
};
