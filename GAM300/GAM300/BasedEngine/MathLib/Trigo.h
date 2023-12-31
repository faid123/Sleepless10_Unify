#pragma once

#include <corecrt_math_defines.h>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

class Trigo
{
public:
	static float Sin(float a)
	{
		return sinf(a);
	}

	static float Cos(float a)
	{
		return cosf(a);
	}

	static float Tan(float a)
	{
		return tanf(a);
	}

	static float aSin(float a)
	{
		return asinf(a);
	}

	static float aCos(float a)
	{
		return acosf(a);
	}

	static float aTan(float a)
	{
		return atanf(a);
	}

	static float aTan2(float b, float a)
	{
		return atan2f(b, a);
	}

	static double Pi()
	{
		return M_PI;
	}

	static int floatToint(double a)
	{
		return static_cast<int>(a);
	}

	static void RegisterLua(sol::state& lua);
};