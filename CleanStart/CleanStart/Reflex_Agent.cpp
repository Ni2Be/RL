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
	std::cout << "actor " << Agent<State_T>::id() << " created\n";
}


template <class State_T>
void Reflex_Agent<State_T>::set_up()
{
	std::cout << "actor " << Agent<State_T>::id() << " set up\n";
}


template <class State_T>
void Reflex_Agent<State_T>::learn() {};

template <class State_T>
void Reflex_Agent<State_T>::evaluate_action()
{
	//std::cout << "\n\n\nevaluate_action:\n";
	if (!m_environment->is_final(m_self_pointer))
	{
		std::vector<Action> possible_actions =
			m_environment->possible_actions(
				m_self_pointer);
		
		auto perc = m_environment->get_perception(m_self_pointer, SEE_THE_WHOLE_STATE);
		
		
		const auto actual_state = m_environment->actual_state(m_self_pointer);

		//randomly choose one action
		int next_action_index = Utility::random_int_ts(0, possible_actions.size() - 1);

		//get the value for the state of that action and set the highest_reaward the first time
		auto next_state = m_environment->assume_action(m_self_pointer, actual_state, possible_actions[next_action_index]);
		auto reward = m_environment->reward(m_self_pointer, next_state[0]);
		Reward highest_reward = reward;

		//get for all future states the reward and set the next action to the action that will lead to the highest reward
		for (int i = 0; i < possible_actions.size(); i++)
		{
			next_state = m_environment->assume_action(m_self_pointer, actual_state, possible_actions[i]);
			reward = m_environment->reward(m_self_pointer, next_state[0]);

			//std::cout << "actor " << Agent<State_T>::id() << " reward for action : " << i << ", reward: "<< reward << "\n";

			if (reward > highest_reward && possible_actions[i].is_possible)
			{
				//std::cout << "actor " << Agent<State_T>::id() << " best reward = " << highest_reward << "\n";
				next_action_index = i;
				highest_reward = reward;
			}
		}

		//std::cout << "actor " << Agent<State_T>::id() << " performing action index: " << next_action_index << ", action: " << possible_actions[next_action_index].action << "\n";
		
		m_environment->apply_action(m_self_pointer, possible_actions[next_action_index]);
	}
	else
	{
		//std::cout << "actor " << Agent<State_T>::id() << " sleeping\n";
		sleep();
		//TODO
		//chould do things like save his learnig progress (if learning agent)
	}

}

template <class State_T>
void Reflex_Agent<State_T>::shut_down()
{
	//TODO save to file


	m_is_running = false;
}