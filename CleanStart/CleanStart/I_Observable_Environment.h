#pragma once
#include <iostream>

#include <vector>
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include "Types.h"

class Actor;
class I_Observable_Environment
{
public:
	/*returns the possible actions in a given State*/
	virtual std::vector<Action> possible_actions(std::shared_ptr<Actor>, State) const = 0;

	/*returns all possible States that could result from a given action*/
	virtual State  assume_action(std::shared_ptr<Actor>, State, Action) const = 0;

	/*sets the action an actor wants to perform*/
	virtual void   apply_action(std::shared_ptr<Actor>, Action) = 0;

	/*returns the actual state of the enviroment*/
	virtual State  actual_state(std::shared_ptr<Actor>) const = 0;

	/*returns the reward of a given state*/
	virtual Reward reward(std::shared_ptr<Actor>, State) const = 0;

	/*prooves if a state is a final state*/
	virtual bool   is_final(std::shared_ptr<Actor>, State) const = 0;

	/*returns the update interval of the environment*/
	virtual std::chrono::milliseconds update_interval() const = 0;

	/*adds an actor to the environment*/
	virtual void add_actor(std::shared_ptr<Actor>) = 0;

	virtual void run() = 0;
	virtual void update() = 0;
protected:
	/*the time interval the environments physics are updated*/
	std::chrono::milliseconds m_update_interval;
	mutable std::mutex m_state_lock;
	mutable std::mutex m_actor_lock;
	std::thread m_environment_thread;


	State& environment_state() {
		std::scoped_lock<std::mutex> lock(m_state_lock);
		std::cout << std::endl << "locked: " << std::this_thread::get_id() << std::endl;
		return m_environment_state; 
	}
	//TODO fix bug
	const State environment_state() const {
		//std::unique_lock<std::mutex> lock(m_state_lock);
		//lock.lock();
		std::cout << std::endl << "locked: " << std::this_thread::get_id() << std::endl ;
		//State state = m_environment_state;
		//lock.unlock();
		return { 0.0f };// State(m_environment_state);
	}

	std::vector<std::pair<std::shared_ptr<Actor>, Action>>& actors() {
		std::scoped_lock<std::mutex> lock(m_actor_lock);
		return m_actors;
	}
	const std::vector<std::pair<std::shared_ptr<Actor>, Action>> actors() const {
		std::scoped_lock<std::mutex> lock(m_actor_lock);
		return std::vector<std::pair<std::shared_ptr<Actor>, Action>>(m_actors);
	}
	Action exchange_action(int i, Action new_action)
	{
		std::lock_guard<std::mutex> lock(m_actor_lock);
		Action past_action = m_actors[i].second;
		m_actors[i].second = new_action;
		return past_action;
	}

	I_Observable_Environment() {};
private:
	State m_environment_state;
	std::vector<std::pair<std::shared_ptr<Actor>, Action>> m_actors;
};
