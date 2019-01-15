#include "Environment.h"
#include "Actor.h"

using namespace Ai_Arena;

template <class State_T>
void Environment<State_T>::set_actor_state(std::shared_ptr<Actor<State_T>> actor, bool state_is_final)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	m_actors[actor->id()].state_is_final = state_is_final;
}

template <class State_T>
Action Environment<State_T>::exchange_action(std::shared_ptr<Actor<State_T>> actor, Action new_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	Action past_action = m_actors[actor->id()].action;
	m_actors[actor->id()].action = new_action;
	return past_action;
}

template <class State_T>
void Environment<State_T>::add_actor(Actor_Representation actor_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	actor_action.actor->id() = m_actors.size();
	m_actors.push_back(actor_action);
	if (actor_action.actor->is_human())
		m_human_actors++;

}


template <class State_T>
std::queue<sf::Event> Environment<State_T>::get_events()
{
	std::scoped_lock<std::mutex> lock(m_event_lock);
	std::queue<sf::Event> event_que;
	while (!m_events.empty())
	{
		event_que.push(m_events.front());
		m_events.pop();
	}
	return event_que;
}

template <class State_T>
void Environment<State_T>::set_events(std::queue<sf::Event> events)
{
	std::scoped_lock<std::mutex> lock(m_event_lock);
	while (!events.empty())
	{
		m_events.push(events.front());
		events.pop();
	}
}

template <class State_T>
std::chrono::system_clock::time_point Environment<State_T>::next_execution_time() const
{ 
	std::scoped_lock<std::mutex> lock(m_next_execution_time_lock);
	return m_next_execution_time; 
}
template <class State_T>
void Environment<State_T>::set_next_execution_time(std::chrono::system_clock::time_point time) 
{
	std::scoped_lock<std::mutex> lock(m_next_execution_time_lock);
	m_next_execution_time = time; 
}




//ACTOR COMMUNICATION
/*returns the possible actions in the current State*/
template <class State_T>
std::vector<Action> Environment<State_T>::possible_actions(std::shared_ptr<Actor<State_T>> actor) const
{
	return possible_actions(actor, actual_state(actor));
};

/*returns the reward of a given state*/
template <class State_T>
Reward Environment<State_T>::reward(std::shared_ptr<Actor<State_T>> actor)
{
	return reward(actor, actual_state(actor));
}

/*prooves if the current state is a final state*/
template <class State_T>
bool   Environment<State_T>::is_final(std::shared_ptr<Actor<State_T>> actor, bool is_simulation) const
{
	return is_final(actor, actual_state(actor), is_simulation);
};


//ACTOR GENERAL

/*returns the number of humen actors*/
template <class State_T>
const int Environment<State_T>::human_actors() const { std::unique_lock<std::mutex>(m_human_actor_lock); return m_human_actors; }
/*retruns the number of active actors*/
template <class State_T>
const int& Environment<State_T>::active_actors() const 
{
	std::unique_lock<std::mutex>(m_actor_lock);
	int active_actors_counter = 0;
	for (const auto& actor : m_actors)
		if (actor.actor->is_active())
			active_actors_counter++;
	return active_actors_counter;
}




//GET/SET
	/*returns the update interval of the environment*/
template <class State_T>
std::chrono::milliseconds& Environment<State_T>::update_interval() { return m_update_interval; }
template <class State_T>
std::chrono::milliseconds  Environment<State_T>::update_interval() const { return m_update_interval; }


//GET/SET
//THREADSAFE
template <class State_T>
void Environment<State_T>::set_environment_state(const State_T& new_state) {
	std::scoped_lock<std::mutex> lock(m_state_lock);
	m_environment_state = new_state;
}
//THREADSAFE
template <class State_T>
const State_T Environment<State_T>::environment_state() const {
	std::scoped_lock<std::mutex> lock(m_state_lock);
	return State(m_environment_state);
}

//THREADSAFE
template <class State_T>
const std::vector<typename Environment<State_T>::Actor_Representation> Environment<State_T>::actors() const {
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	return std::vector<Actor_Representation>(m_actors);
}