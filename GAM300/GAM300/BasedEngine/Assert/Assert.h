#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

#if defined NDEBUG
#define MY_ASSERT(x) void(0)
#else
#define MY_ASSERT(x) \
	((x)?  void(0) : debugInfo(__FILE__, __LINE__, #x))
#endif

void debugInfo(const char* file, int line, std::string expression);