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
		static int random_int_ts(const int& min, const int& max)
		{
			static __declspec(thread) std::mt19937 generator;
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(generator);
		}

		//Thread save random float
		static float random_float_ts(const float& min, const float& max)
		{
			static __declspec(thread) std::mt19937 generator;
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(generator);
		}
	}
}