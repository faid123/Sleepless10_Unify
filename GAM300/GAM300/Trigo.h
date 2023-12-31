#pragma once

#include <corecrt_math_defines.h>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

class Trigo
{
public:
	float sin(float a)
	{
		return sinf(a);
	}

	float cos(float a)
	{
		return cosf(a);
	}

	float tan(float a)
	{
		return tanf(a);
	}

	float asin(float a)
	{
		return asinf(a);
	}

	float acos(float a)
	{
		return acosf(a);
	}

	float atan(float a)
	{
		return atanf(a);
	}

	static void RegisterLua(sol::state& lua);
};