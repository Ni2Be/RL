#pragma once
#include "Reflex_Agent.h"

#include <iostream>

#include "Utility.h"

using namespace Ai_Arena;


template <class State_T>
Reflex_Agent<State_T>::Reflex_Agent(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Agent<State_T>::Agent(enviroment)
{
}


template <class State_T>
void Reflex_Agent<State_T>::set_up()
{
}

template <class State_T>
void Reflex_Agent<State_T>::evaluate_action()
{
	if (!m_environment->is_final(m_self_pointer))
	{
		std::vector<Action> possible_actions =
			m_environment->possible_actions(
				m_self_pointer);
		
		const auto state = m_environment->actual_state(m_self_pointer);

		//randomly choose one action
		int next_action_index = Utility::random_int_ts(0, possible_actions.size() - 1);

		//get the value for the state of that action and set the highest_reaward the first time
		auto next_state = m_environment->assume_action(m_self_pointer, state, possible_actions[next_action_index]);
		auto reward = m_environment->reward(m_self_pointer, next_state[0]);
		Reward highest_reward = reward;

		//get for all future states the reward and set the next action to the action that will lead to the highest reward
		for (int i = 0; i < possible_actions.size(); i++)
		{
			next_state = m_environment->assume_action(m_self_pointer, state, possible_actions[i]);
			reward = m_environment->reward(m_self_pointer, next_state[0]);
			if (reward > highest_reward && possible_actions[i].is_possible)
			{
				next_action_index = i;
				highest_reward = reward;
			}
		}
		m_environment->apply_action(m_self_pointer, possible_actions[next_action_index]);
	}
	else
	{
		deactivate();
		sleep();
	}

}

template <class State_T>
void Reflex_Agent<State_T>::shut_down()
{
	//TODO save to file
	std::cout << "shut down\n";

	m_is_running = false;
}