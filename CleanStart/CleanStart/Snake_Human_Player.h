#pragma once
#include "Human_Player.h"
#include "Snake_Entity.h"

namespace Ai_Arena
{
	class Snake_Human_Player : public Human_Player
	{
	public:
		Snake_Human_Player(std::shared_ptr<Environment> enviroment);
		void evaluate_action();

	private:
		Snake::Actions m_last_action;
	};
}