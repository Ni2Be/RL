#pragma once
#include "Agent.h"
#include "Agent.cpp"

namespace Ai_Arena
{
	template <class State_T>
	class Reflex_Agent : public Agent<State_T>
	{
	public:
		Reflex_Agent(std::shared_ptr<Environment<State_T>> enviroment);
		void learn();
		void evaluate_action();
		void set_up();
		void shut_down();


		using Agent<State_T>::m_self_pointer;
		using Agent<State_T>::m_environment;
		using Agent<State_T>::start_actor_thread;
		using Agent<State_T>::is_sleeping;
		using Agent<State_T>::sleep;
		using Agent<State_T>::wake_up;
		using Agent<State_T>::id;
		using Agent<State_T>::is_human;
		using Agent<State_T>::m_id;
		using Agent<State_T>::m_is_sleeping;
		using Agent<State_T>::m_is_running;
	};
}