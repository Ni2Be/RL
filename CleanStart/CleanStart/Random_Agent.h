#pragma once
#include "Agent.h"
#include "Agent.cpp"

namespace Ai_Arena
{
	template <class State_T>
	class Random_Agent : public Agent<State_T>
	{
	public:
		Random_Agent(std::shared_ptr<Environment<State_T>> enviroment);
		void evaluate_action();



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
		using Actor<State_T>::is_active;
		using Actor<State_T>::activate;
		using Actor<State_T>::deactivate;
	};
}