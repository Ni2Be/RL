#pragma once
#include "Agent.h"



class Random_Agent : public Agent
{
public:
	Random_Agent(Eight_Puzzle enviroment)
		:
		Agent(enviroment)
	{}
	void learn() {};
	I_Observable_Environment::Action get_next_action()
	{
		srand(static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count()));
		std::vector<I_Observable_Environment::Action> possible_actions = m_enviroment->possible_actions(m_enviroment->actual_state());
		m_enviroment->apply_action(m_enviroment->actual_state(), possible_actions[rand() % possible_actions.size()]);
	}
};