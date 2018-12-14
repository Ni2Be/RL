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
		
		bool is_human() { return true; }
		//Input_Mode m_input_mode = AUTO_RELEASE_KEY;
	};
}
