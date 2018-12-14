#pragma once
#include "Actor.h"

namespace Ai_Arena
{
	//TODO not int
	template <class State_T>
	class Agent : public Actor<State_T>
	{
	public:
		Agent(std::shared_ptr<Environment<State_T>> environment);
		virtual void evaluate_action() = 0;
		bool is_human() { return false; }
	protected:
	};
}