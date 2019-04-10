#pragma once
#include <memory>
#include <vector>


#include "Eight_Puzzle.h"

//#define PONG
#define SNAKE 1

#if PONG
#include "Pong_Game.h"
#include "Pong_Human_Player.h"
#include "Pong_Human_Player.cpp"
#endif

#if SNAKE
#include "Snake_Game.h"
#include "Snake_Human_Player.h"
#include "Snake_Human_Player.cpp"
#endif

#include "Random_Agent.h"
#include "Random_Agent.cpp"
#include "Reflex_Agent.h"
#include "Reflex_Agent.cpp"
#include "MCTS_Agent.h"
#include "MCTS_Agent.cpp"

namespace Ai_Arena
{
	class Arena
	{
	public:
		Arena()
		{}

		void run();

	protected:
#ifdef PONG
		std::shared_ptr<Environment<Pong_World>> m_pong_enviroment;
		std::vector<std::shared_ptr<Actor<Pong_World>>> m_pong_actors;
#elif SNAKE
		std::shared_ptr<Environment<Snake_World>> m_enviroment;
		std::vector<std::shared_ptr<Actor<Snake_World>>> m_actors;
#endif
	};
}