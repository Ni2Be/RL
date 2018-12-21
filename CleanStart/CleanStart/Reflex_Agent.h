#pragma once
#include "Agent.h"
#include "Agent.cpp"


#include "Environment.h"

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


		using Actor<State_T>::m_self_pointer;
		using Actor<State_T>::m_environment;
		using Actor<State_T>::start_actor_thread;
		using Actor<State_T>::is_sleeping;
		using Actor<State_T>::sleep;
		using Actor<State_T>::wake_up;
		using Actor<State_T>::id;
		using Actor<State_T>::is_human;
		using Actor<State_T>::m_id;
		using Actor<State_T>::m_is_sleeping;
		using Actor<State_T>::m_is_running;

	};
}
