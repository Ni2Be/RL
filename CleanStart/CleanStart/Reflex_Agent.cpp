#pragma once
#include "Reflex_Agent.h"

#include <iostream>

#include "Utility.h"

using namespace Ai_Arena;

Reflex_Agent::Reflex_Agent(std::shared_ptr<Environment> enviroment)
	:
	Agent(enviroment)
{}

void Reflex_Agent::learn() {};

void Reflex_Agent::evaluate_action()
{
	if (!m_environment->is_final(m_self_pointer))
	{
		std::vector<Action> possible_actions =
			m_environment->possible_actions(
				m_self_pointer);

		auto actual_state = m_environment->actual_state(m_self_pointer);

		int next_action_index = 0;
		float highest_reward = 0;
		for (int i = 0; i < possible_actions.size(); i++)
		{
			auto next_state = m_environment->assume_action(m_self_pointer, actual_state, possible_actions[i]);
			auto reward = m_environment->reward(m_self_pointer, next_state[0]);
			if (reward > highest_reward)
			{
				next_action_index = i;
				highest_reward = reward;
			}
		}

		set_action(possible_actions[next_action_index]);
	}
	else
	{
		std::cout << "actor " << id() << " sleeping\n";
		sleep();
		//TODO
		//chould do things like save his learnig progress (if learning agent)
	}

}