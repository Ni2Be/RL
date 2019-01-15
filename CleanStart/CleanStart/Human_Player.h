#pragma once
#include "Actor.h"

namespace Ai_Arena
{
	template <class State_T>
	class Human_Player : public Actor<State_T>
	{
	public:
		/*enum Input_Mode
		{
			AUTO_RELEASE_KEY,
			AUTO_HOLD_KEY
		};*/

		Human_Player(std::shared_ptr<Environment<State_T>> enviroment);
		
		virtual void set_action();
		void evaluate_action();
		Action m_next_action;

		bool is_human() { return true; }

		using Actor<State_T>::m_environment;
	};
}
