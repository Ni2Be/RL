#pragma once
#include <memory>
#include <vector>

#include "I_Observable_Environment.h"
#include "Actor.h"

class Scene
{
public:
	std::shared_ptr<I_Observable_Environment> m_game;
	std::vector<std::shared_ptr<Actor>> m_actors;
};