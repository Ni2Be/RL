#pragma once
#include "Actor.h"

#include <iostream>

using namespace Ai_Arena;

Actor::Actor(std::shared_ptr<I_Environment> environment)
	:
	m_environment(environment),
	m_self_pointer(this)
{
}

//TODO 
void Actor::start_actor_thread()
{
	m_actor_thread = std::thread([this]() {
		m_is_running = true;
		m_is_sleeping = false;
		while (m_is_running)
		{
			evaluate_action();
			m_self_pointer->evaluate_action();
		}
	});
}

bool Actor::is_sleeping() 
{
	return m_is_sleeping; 
};

void Actor::wake_up()
{
	std::unique_lock<std::mutex> lock(m_actor_lock);
	m_is_sleeping = false;
	//std::cout << std::endl << "wakeup" << std::endl;
	m_actor_condition.notify_one();
	lock.unlock();
};

void Actor::sleep() 
{
	std::unique_lock<std::mutex> lock(m_actor_lock);
	m_is_sleeping = true;
	//std::cout << std::endl << "sleep" << std::endl;
	m_actor_condition.wait(lock);
	//std::cout << std::endl << "awake" << std::endl;
	lock.unlock();
};