#pragma once
#include "Random_Agent.h"


#include <iostream>

#include "Utility.h"

using namespace Ai_Arena;

template <class State_T>
Random_Agent<State_T>::Random_Agent(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Agent<State_T>::Agent(enviroment)
{}

template <class State_T>
void Random_Agent<State_T>::evaluate_action()
{
	if (!m_environment->is_final(m_self_pointer))
	{
		std::vector<Action> possible_actions =
			Agent<State_T>::m_environment->possible_actions(
				Agent<State_T>::m_self_pointer);


		Agent<State_T>::m_environment->apply_action(Agent<State_T>::m_self_pointer, possible_actions[Utility::random_int_ts(0, possible_actions.size() - 1)]);


	}
	else
	{
		std::cout << "actor " << Agent<State_T>::id() << " deactivated\n";
		deactivate();
		sleep();
	}

}