#pragma once
#include <memory>
#include <vector>

#include "Eight_Puzzle.h"
#include "Multi_Snake.h"
#include "Random_Agent.h"
#include "Snake_Human_Player.h"

namespace Ai_Arena
{
	class Arena
	{
	public:
		Arena()
		{
		//SNAKE
			m_enviroment = std::shared_ptr<Environment>(new Multi_Snake(20, 30));
			
			
			//HUMAN
			m_actors.push_back(std::shared_ptr<Actor>(new Snake_Human_Player(m_enviroment)));
			m_enviroment->add_actor(m_actors[0]);
			
			//AGENTS
			m_actors.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment)));
			m_enviroment->add_actor(m_actors[1]);
			m_actors.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment)));
			m_enviroment->add_actor(m_actors[2]);
			m_actors.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment)));
			m_enviroment->add_actor(m_actors[3]);
			m_actors.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment)));
			m_enviroment->add_actor(m_actors[4]);
			
			m_enviroment->update_interval() = std::chrono::milliseconds(100);


		//EIGHT PUZZLE
			//m_enviroment_2 = std::shared_ptr<Environment>(new Eight_Puzzle(3, 3, 90));
			//
			////AGENT
			//m_actors_2.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment_2)));
			//m_enviroment_2->add_actor(m_actors_2[0]);
			//
			//m_enviroment_2->update_interval() = std::chrono::milliseconds(0);
		}

		void run();

	protected:
		std::shared_ptr<Environment> m_enviroment;
		std::vector<std::shared_ptr<Actor>> m_actors;
		//std::thread m_environment_thread;

		std::shared_ptr<Environment> m_enviroment_2;
		std::vector<std::shared_ptr<Actor>> m_actors_2;
	};
}