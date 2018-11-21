/*
I_Environment.h
	Das Interface I_Environment stellt alle Funktionen
	zur Verfügung damit ein Actor (vorallem ein Agent)
	mit dem Environment kommunizieren kann.

std::vector<Action> possible_actions(std::shared_ptr<Actor>, State)
	Wird von planenden Agenten aufgerufen um die möglichen Aktionen 
	für einen bestimmten State zu erfahren sollte alle Actions zurückgeben 
	und bei den nicht ausführbaren die bool is_possible auf false stellen.
std::vector<Action> possible_actions(std::shared_ptr<Actor>)
	Wird von Actors aufgerufen um die aktuell möglichen Actionen zu erfahren
	sollte alle Actions zurückgeben und bei den nicht ausführbaren
	die bool is_possible auf false stellen.

State assume_action(std::shared_ptr<Actor>, State, Action)
	Wird von planenden Agenten aufgerufen um für einen State und eine Action 
	den	daraus folgenden State zu erfahren für einen bestimmten State zu 
	erfahren.
	Sind nach einer Action mehrere Stats möglich werden alle übertragen.

void apply_action(std::shared_ptr<Actor>, Action)
	Wird von Actors aufgerufen um dem Environment mitzuteilen welche Action
	er ausführen will.

State actual_state(std::shared_ptr<Actor>)
	Wird von Agenten aufgerufen um den aktuellen State des Environment zu erfahren.

Reward reward(std::shared_ptr<Actor>, State)
	Wird von planenden Agenten aufgerufen um für einen State einen Reward
	zu bekommen.
Reward reward(std::shared_ptr<Actor>)
	Wird von Agenten aufgerufen um für den aktuellen State einen Reward
	zu bekommen.

bool is_final(std::shared_ptr<Actor>, State)
	Wird von planenenden Agenten aufgerufen um zu erfahren ob von einem State
	aus kein weiterer State mehr erreicht werden kann.



void set_up()
	Wird automatisch aufgerufen bevor die update loop
	das erste Mal aufgerufen wird.

*/
#pragma once
#include <vector>
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include <numeric>
#include <algorithm>
#include <queue>

#include <SFML/Graphics.hpp>

#include "Types.h"

namespace Ai_Arena
{
	class Actor;
	class I_Environment
	{
	public:
	//ACTOR COMMUNICATION

		/*returns the possible actions in a given State*/
		virtual std::vector<Action> possible_actions(std::shared_ptr<Actor>, State) const = 0;
		/*returns the possible actions in the current State*/
		virtual std::vector<Action> possible_actions(std::shared_ptr<Actor> actor) const
		{ return possible_actions(actor, actual_state(actor)); };

		/*returns all possible States that could result from a given action*/
		virtual std::vector<State> assume_action(std::shared_ptr<Actor>, State, Action) const = 0;

		/*sets the action an actor wants to perform*/
		virtual void   apply_action(std::shared_ptr<Actor>, Action) = 0;

		/*returns the actual state of the enviroment*/
		virtual State  actual_state(std::shared_ptr<Actor>) const = 0;

		/*returns the reward of a given state*/
		virtual Reward reward(std::shared_ptr<Actor>, State) const = 0;
		/*returns the reward of a given state*/
		virtual Reward reward(std::shared_ptr<Actor> actor)
		{ return reward(actor, actual_state(actor)); }

		/*prooves if a state is a final state*/
		virtual bool   is_final(std::shared_ptr<Actor>, State) const = 0;


	//GENERAL

		/*starts the environment*/
		virtual void run() = 0;
		/*adds an actor to the environment*/
		virtual void add_actor(std::shared_ptr<Actor>) = 0;
		/*returns the number of humen actors*/
		const int human_actors() const { std::unique_lock<std::mutex>(m_actor_lock); return m_human_actors; }

	//GET/SET
		/*returns the update interval of the environment*/
		std::chrono::milliseconds& update_interval() { return m_update_interval; }
		std::chrono::milliseconds  update_interval() const { return m_update_interval; }

		/*returns all Events like key/mouse input and emptys the queue*/
		virtual std::queue<sf::Event> get_events() = 0;
	protected:
		virtual void set_events(std::queue<sf::Event>) = 0;

		/*the time interval the environments physics are updated*/
		std::chrono::milliseconds m_update_interval;
		mutable std::mutex m_state_lock;
		mutable std::mutex m_actor_lock;
		std::thread m_environment_thread;

		mutable std::mutex m_execution_lock;
		mutable std::condition_variable m_environment_condition;
		mutable std::condition_variable m_actors_condition;
		mutable int m_unexecuted_actions = 0;

	//GET/SET
		//THREADSAFE
		void set_environment_state(const State& new_state) {
			std::scoped_lock<std::mutex> lock(m_state_lock);
			m_environment_state = new_state;
		}
		//THREADSAFE
		const State environment_state() const {
			std::scoped_lock<std::mutex> lock(m_state_lock);
			return State(m_environment_state);
		}

		//THREADSAFE
		const std::vector<std::pair<std::shared_ptr<Actor>, Action>> actors() const {
			std::scoped_lock<std::mutex> lock(m_actor_lock);
			return std::vector<std::pair<std::shared_ptr<Actor>, Action>>(m_actors);
		}
		//THREADSAFE returns the old action
		Action exchange_action(std::shared_ptr<Actor> actor, Action new_action);
		//THREADSAFE
		void add_actor(std::pair<std::shared_ptr<Actor>, Action> actor_action);

		I_Environment() {};
	private:
		//std::queue<sf::Event> m_events;
		//std::mutex            m_event_lock;
		State m_environment_state;
		std::vector<std::pair<std::shared_ptr<Actor>, Action>> m_actors;
		int m_human_actors;
	};
}