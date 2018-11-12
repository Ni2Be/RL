#pragma once
#include "Types.h"
#include "I_Observable_Environment.h"
#include <memory>
#include <thread>
#include <condition_variable>
#include <atomic>

class Actor
{
public:
	Actor(std::shared_ptr<I_Observable_Environment> environment);
	virtual void evaluate_action() = 0;

	void start_actor_thread();
	bool is_sleeping();
	void wake_up();
protected:
	void sleep();
	std::mutex  m_actor_lock;
	std::thread m_actor_thread;
	std::condition_variable m_actor_condition;
	std::atomic<bool> m_is_sleeping = true;
	std::shared_ptr<Actor> m_self_pointer;
	std::shared_ptr<I_Observable_Environment> m_environment;
};