#pragma once
#include "Human_Player.h"
#include "Human_Player.cpp"
#include "Snake_Entity.h"

namespace Ai_Arena
{
	template <class State_T>
	class Snake_Human_Player : public Human_Player<State_T>
	{
	public:
		Snake_Human_Player(std::shared_ptr<Environment<State_T>> enviroment);
		void set_action();

	private:
		Snake::Actions m_last_action;
		using Human_Player<State_T>::m_next_action;
	};
}