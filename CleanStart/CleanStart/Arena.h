#pragma once
#include <memory>
#include <vector>

#include "Eight_Puzzle.h"
#include "Snake_Game.h"
#include "Random_Agent.h"
#include "Snake_Human_Player.h"

namespace Ai_Arena
{
	class Arena
	{
	public:
		Arena()
		{}

		void run();

	protected:
		std::shared_ptr<Environment> m_enviroment;
		std::vector<std::shared_ptr<Actor>> m_actors;
		//std::thread m_environment_thread;

		std::shared_ptr<Environment> m_enviroment_2;
		std::vector<std::shared_ptr<Actor>> m_actors_2;
	};
}