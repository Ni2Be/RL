#pragma once
#include <memory>
#include <vector>


#include "Eight_Puzzle.h"
#include "Snake_Game.h"
#include "Random_Agent.h"
#include "Random_Agent.cpp"
#include "Reflex_Agent.h"
#include "Reflex_Agent.cpp"
#include "Snake_Human_Player.h"
#include "Snake_Human_Player.cpp"
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
		std::shared_ptr<Environment<Snake_World>> m_enviroment;
		std::vector<std::shared_ptr<Actor<Snake_World>>> m_actors;
	};
}