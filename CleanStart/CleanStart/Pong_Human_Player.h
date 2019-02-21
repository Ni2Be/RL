#pragma once
#include "Human_Player.h"
#include "Human_Player.cpp"
#include "Paddle.h"

namespace Ai_Arena
{
	template <class State_T>
	class Pong_Human_Player : public Human_Player<State_T>
	{
	public:
		Pong_Human_Player(std::shared_ptr<Environment<State_T>> enviroment);
		void set_action();

	private:
		Pong::Actions m_last_action;
		using Human_Player<State_T>::m_next_action;
	};
}