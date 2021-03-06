/*
Utility.h
	Enth�lt gemeinsam genutze statische Funktionen.

int random_int_ts(const int& min, const int& max)
	Gibt einen zuf�lligen Wert zwischen min und max zur�ck.
	thread safe.

float random_float_ts(const float& min, const float& max)
	Gibt einen zuf�lligen Wert zwischen min und max zur�ck.
	thread safe.
*/
#pragma once
#include <random>
#include <time.h>
#include <thread>

namespace Ai_Arena
{
	namespace Utility
	{
		//Thread save random int
		static int random_int_ts(int min, int max)
		{
			static __declspec(thread) std::random_device r;
			static __declspec(thread) std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
			static __declspec(thread) std::mt19937 generator(seed);
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(generator);
		}

		//Thread save random float
		static float random_float_ts(float min, float max)
		{
			static __declspec(thread) std::random_device r;
			static __declspec(thread) std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
			static __declspec(thread) std::mt19937 generator(seed);
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(generator);
		}

		//Thread save random double
		static double random_double_ts(double min, double max)
		{
			static __declspec(thread) std::random_device r;
			static __declspec(thread) std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
			static __declspec(thread) std::mt19937 generator(seed);
			std::uniform_real_distribution<double> distribution(min, max);
			return distribution(generator);
		}
	}
}