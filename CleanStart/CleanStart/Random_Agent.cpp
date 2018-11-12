#pragma once
#include "Random_Agent.h"


#include <iostream>

#include "Utility.h"

Random_Agent::Random_Agent(std::shared_ptr<I_Observable_Environment> enviroment)
	:
	Agent(enviroment)
{}

void Random_Agent::learn() {};

void Random_Agent::evaluate_action()
{
	sleep();

	std::vector<Action> possible_actions =
		m_environment->possible_actions(
			m_self_pointer,
			m_environment->actual_state(m_self_pointer));
	
	m_environment->apply_action(
		m_self_pointer,
		possible_actions[Utility::random_int_ts(0, possible_actions.size() - 1)]);

}