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
	if (!Agent<State_T>::m_environment->is_final(Agent<State_T>::m_self_pointer))
	{
		std::vector<Action> possible_actions =
			Agent<State_T>::m_environment->possible_actions(
				Agent<State_T>::m_self_pointer);

		const auto actual_state = Agent<State_T>::m_environment->actual_state(Agent<State_T>::m_self_pointer);

		int next_action_index = Utility::random_int_ts(0, possible_actions.size() - 1);


		auto next_state = Agent<State_T>::m_environment->assume_action(Agent<State_T>::m_self_pointer, actual_state, possible_actions[0]);
		auto reward = Agent<State_T>::m_environment->reward(Agent<State_T>::m_self_pointer, next_state[0]);
		Reward highest_reward = reward;

		for (int i = 0; i < possible_actions.size(); i++)
		{
			next_state = Agent<State_T>::m_environment->assume_action(Agent<State_T>::m_self_pointer, actual_state, possible_actions[i]);
			reward = Agent<State_T>::m_environment->reward(Agent<State_T>::m_self_pointer, next_state[0]);

			//std::cout << "actor " << Agent<State_T>::id() << " reward for action : " << i << ", reward: "<< reward << "\n";

			if (reward > highest_reward && possible_actions[i].is_possible)
			{
				//std::cout << "actor " << Agent<State_T>::id() << " best reward = " << highest_reward << "\n";
				next_action_index = i;
				highest_reward = reward;
			}
		}

		//std::cout << "actor " << Agent<State_T>::id() << " performing action: " << next_action_index << "\n\n\n";
		Agent<State_T>::set_action(possible_actions[next_action_index]);
	}
	else
	{
		//std::cout << "actor " << Agent<State_T>::id() << " sleeping\n";
		Agent<State_T>::sleep();
		//TODO
		//chould do things like save his learnig progress (if learning agent)
	}

}