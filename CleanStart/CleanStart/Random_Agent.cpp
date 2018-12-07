#pragma once
#include "Random_Agent.h"


#include <iostream>

#include "Utility.h"

using namespace Ai_Arena;

Random_Agent::Random_Agent(std::shared_ptr<Environment> enviroment)
	:
	Agent(enviroment)
{}

void Random_Agent::learn() {};

void Random_Agent::evaluate_action()
{
	if (!m_environment->is_final(m_self_pointer))
	{
		std::vector<Action> possible_actions =
			m_environment->possible_actions(
				m_self_pointer);

		set_action(possible_actions[Utility::random_int_ts(0, possible_actions.size() - 1)]);
	}
	else 
	{
		std::cout << "actor " << id() << " sleeping\n";
		sleep();
		//TODO
		//chould do things like save his learnig progress (if learning agent)
	}

}