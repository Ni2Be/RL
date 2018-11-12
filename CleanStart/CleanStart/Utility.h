#pragma once
#include <random>
#include <time.h>
#include <thread>

namespace Utility
{
	//Thread save random int
	int random_int_ts(const int& min, const int& max)
	{
		//static __declspec(thread) std::mt19937 generator;
		//std::uniform_int_distribution<int> distribution(min, max);
		//return distribution(generator);
		return 0;
	}

	//Thread save random float
	float random_float_ts(const float& min, const float& max)
	{
		//static __declspec(thread) std::mt19937 generator;
		//std::uniform_real_distribution<float> distribution(min, max);
		//return distribution(generator);
		return 0;
	}
}