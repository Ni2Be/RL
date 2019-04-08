#pragma once
#include <memory>
#include <vector>


#include "Eight_Puzzle.h"

#include "Pong_Game.h"
#include "Pong_Human_Player.h"
#include "Pong_Human_Player.cpp"

//#include "Snake_Game.h"
//#include "Snake_Human_Player.h"
//#include "Snake_Human_Player.cpp"

#include "Random_Agent.h"
#include "Random_Agent.cpp"
#include "Reflex_Agent.h"
#include "Reflex_Agent.cpp"
#include "MCTS_Agent.h"
#include "MCTS_Agent.cpp"
#include "TD_Agent.h"
#include "TD_Agent.cpp"

namespace Ai_Arena
{
	class Arena
	{
	public:
		Arena()
		{}

		void run();

	protected:

		//std::shared_ptr<Environment<Snake_World>> m_enviroment;
		//std::vector<std::shared_ptr<Actor<Snake_World>>> m_actors;
		
		std::shared_ptr<Environment<Pong_World>> m_pong_enviroment;
		std::vector<std::shared_ptr<Actor<Pong_World>>> m_pong_actors;
	};
}