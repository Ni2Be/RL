#pragma once
#include "Agent.h"
#include "Agent.cpp"

namespace Ai_Arena
{
	template <class State_T>
	class Random_Agent : public Agent<State_T>
	{
	public:
		Random_Agent(std::shared_ptr<Environment<State_T>> enviroment);
		void learn();
		void evaluate_action();
	};
}