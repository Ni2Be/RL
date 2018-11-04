#pragma once
#include <memory>
#include "I_Observable_Environment.h"
#include "Eight_Puzzle.h"
#include "Actor.h"

class Agent : public Actor
{
public:
	//TODO should live in an enviroment not in a Game
	Agent(Eight_Puzzle enviroment)
		:
		m_enviroment(std::make_shared<Eight_Puzzle>(enviroment))
	{}
	virtual void learn() {};
	virtual I_Observable_Environment::Action get_next_action() = 0;
protected:
	std::shared_ptr<Eight_Puzzle> m_enviroment;
};