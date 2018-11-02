#pragma once
#include "I_Observable_Environment.h"

#include <memory>



template <class State, class Action, class Reward>
class I_Agent
{
public:
	std::shared_ptr<I_Observable_Environment<State, Action, Reward>> m_enviroment;
	void learn();
	Action get_next_action();
};