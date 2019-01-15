#include "Human_Player.h"
#include <SFML/Graphics.hpp>
#include <chrono>

using namespace Ai_Arena;

template <class State_T>
Human_Player<State_T>::Human_Player(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Actor<State_T>(enviroment)
{}

template <class State_T>
void Human_Player<State_T>::set_action()
{}


template <class State_T>
void Human_Player<State_T>::evaluate_action()
{	
	auto next_action_time =	m_environment->next_execution_time();

	while (std::chrono::system_clock::now() < next_action_time)
	{
		set_action();
	}
	Actor<State_T>::m_environment->apply_action(Human_Player<State_T>::m_self_pointer, m_next_action);

}
