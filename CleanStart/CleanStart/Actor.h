#pragma once
#include "Types.h"
#include "I_Environment.h"
#include <memory>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace Ai_Arena
{
	class Actor
	{
	public:
		Actor(std::shared_ptr<I_Environment> environment);
		virtual void evaluate_action() = 0;

		void start_actor_thread();
		bool is_sleeping();
		void sleep();
		void wake_up();
		void shut_down() { m_is_running = false; };

		int& id() { return m_id; }
		const int& id() const { return m_id; }
		virtual bool is_human() = 0;
	protected:
		int m_id;

		std::mutex  m_actor_lock;
		std::thread m_actor_thread;
		std::condition_variable m_actor_condition;
		std::atomic<bool> m_is_sleeping = true;
		std::shared_ptr<Actor> m_self_pointer;
		std::shared_ptr<I_Environment> m_environment;

		std::atomic<bool> m_is_running = false;
	};
}