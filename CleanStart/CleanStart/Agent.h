#pragma once
#include "Actor.h"

namespace Ai_Arena
{
	class Agent : public Actor
	{
	public:
		Agent(std::shared_ptr<I_Environment> environment);
		virtual void evaluate_action() = 0;
		bool is_human() { return false; }
	protected:
	};
}