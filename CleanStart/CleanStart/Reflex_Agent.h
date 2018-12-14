#pragma once
#include "Agent.h"


namespace Ai_Arena
{
	class Reflex_Agent : public Agent
	{
	public:
		Reflex_Agent(std::shared_ptr<Environment> enviroment);
		void learn();
		void evaluate_action();
	};
}