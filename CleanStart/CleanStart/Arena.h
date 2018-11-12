#pragma once
#include <memory>
#include <vector>

#include "Eight_Puzzle.h"
#include "Random_Agent.h"

class Arena
{
public:
	Arena()
	{
		m_enviroment = std::shared_ptr<I_Observable_Environment>(new Eight_Puzzle(3,3,90));
		m_actors.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment)));
		m_enviroment->add_actor(m_actors[0]);
		m_actors[0]->start_actor_thread();
	}

	void run()
	{
		m_enviroment->run();
	}

protected:
	std::shared_ptr<I_Observable_Environment> m_enviroment;
	std::vector<std::shared_ptr<Actor>> m_actors;
};