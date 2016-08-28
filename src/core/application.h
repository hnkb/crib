
#pragma once

#include "window.h"
#include "utility.h"


class application
{
public:
	application() {}

	application(const application& other) { throw std::logic_error("Copy constructor not supported for application."); }
	application(application&& other) { throw std::logic_error("Move constructor not supported for application."); }
	application& operator=(const application& other) { throw std::logic_error("Copy assignment not supported for application."); }
	application& operator=(application&& other) { throw std::logic_error("Move assignment not supported for application."); }

	~application() {}

	int run();

private:
	com_initialize com_init;
	window main;
};
