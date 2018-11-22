#pragma once
#include "Agent.h"


namespace Ai_Arena
{
	class Random_Agent : public Agent
	{
	public:
		Random_Agent(std::shared_ptr<Environment> enviroment);
		void learn();
		void evaluate_action();
	};
}