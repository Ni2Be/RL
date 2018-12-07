#include "Environment.h"
#include "Actor.h"

using namespace Ai_Arena;

void Environment::set_actor_state(std::shared_ptr<Actor> actor, bool state_is_final)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	m_actors[actor->id()].state_is_final = state_is_final;
}

Action Environment::exchange_action(std::shared_ptr<Actor> actor, Action new_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	Action past_action = m_actors[actor->id()].action;
	m_actors[actor->id()].action = new_action;
	return past_action;
}

void Environment::add_actor(Actor_Representation actor_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	actor_action.actor->id() = m_actors.size();
	m_actors.push_back(actor_action);
	if (actor_action.actor->is_human())
		m_human_actors++;
	m_active_actors++;
}


std::queue<sf::Event> Environment::get_events()
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

void Environment::set_events(std::queue<sf::Event> events)
{
	std::scoped_lock<std::mutex> lock(m_event_lock);
	while (!events.empty())
	{
		m_events.push(events.front());
		events.pop();
	}
}