#pragma once

#include <random>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	static float randInRange(float min, float max)
	{
		float range = max - min + 1.f;
		return min + std::rand() % (int)range;
	}

	static int randIntInRange(int min, int max)
	{
		float range = max - min + 1.f;
		return min + std::rand() % (int)range;
	}

	static void RegisterLua(sol::state& lua);
private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};