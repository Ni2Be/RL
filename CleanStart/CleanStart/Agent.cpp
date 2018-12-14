#include "Agent.h"

using namespace Ai_Arena;

template <class State_T>
Agent<State_T>::Agent(std::shared_ptr<Environment<State_T>> environment)
		:
		Actor<State_T>::Actor(environment)
	{}