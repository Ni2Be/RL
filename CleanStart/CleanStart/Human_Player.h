#pragma once
#include "Actor.h"

namespace Ai_Arena
{
	class Human_Player : public Actor
	{
	public:
		/*enum Input_Mode
		{
			AUTO_RELEASE_KEY,
			AUTO_HOLD_KEY
		};*/

		Human_Player(std::shared_ptr<Environment> enviroment);
		
		bool is_human() { return true; }
		//Input_Mode m_input_mode = AUTO_RELEASE_KEY;
	};
}
