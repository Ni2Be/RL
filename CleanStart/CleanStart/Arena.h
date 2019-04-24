#pragma once
#include <memory>
#include <vector>

#define PONG
//#define SNAKE

#ifdef PONG
#include "Pong_Game.h"
#include "Pong_Human_Player.h"
#include "Pong_Human_Player.cpp"
#endif

#ifdef SNAKE
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
#include "TD_Agent.h"
#include "TD_Agent.cpp"

namespace Ai_Arena
{
	class Arena
	{
	public:
		Arena(int humanPlayers, int MCTSAgents, int ReflexAgents, int RandomAgents, int TDAgents)
		{
			this->humanPlayers = humanPlayers;
			this->MCTSAgents = MCTSAgents;
			this->ReflexAgents = ReflexAgents;
			this->RandomAgents = RandomAgents;
			this->TDAgents = TDAgents;
			std::cout << "tst";
		}

		void run();

	protected:

		int humanPlayers, MCTSAgents, ReflexAgents, RandomAgents, TDAgents, addedAgents = 0;

#ifdef SNAKE
		std::shared_ptr<Environment<Snake_World>> m_enviroment;
		std::vector<std::shared_ptr<Actor<Snake_World>>> m_actors;
#endif

#ifdef PONG
		std::shared_ptr<Environment<Pong_World>> m_pong_enviroment;
		std::vector<std::shared_ptr<Actor<Pong_World>>> m_pong_actors;
#endif
	};
}
