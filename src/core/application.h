
#pragma once

#include "window.h"
#include "utility.h"


class application
{
public:
	application() {}

	application(const application& other) { throw "Copy constructor not supported."; }
	application(application&& other) { throw "Move constructor not supported."; }
	application& operator=(const application& other) { throw "Copy assignment not supported."; }
	application& operator=(application&& other) { throw "Move assignment not supported."; }

	~application() {}

	int run();

private:
	com_initialize com_init;
	window main;
};
