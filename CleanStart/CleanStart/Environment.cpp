#include "Environment.h"
#include "Actor.h"

using namespace Ai_Arena;
Action Environment::exchange_action(std::shared_ptr<Actor> actor, Action new_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	Action past_action = m_actors[actor->id()].second;
	m_actors[actor->id()].second = new_action;
	return past_action;
}

void Environment::add_actor(std::pair<std::shared_ptr<Actor>, Action> actor_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	actor_action.first->id() = m_actors.size();
	m_actors.push_back(actor_action);
	if (actor_action.first->is_human())
		m_human_actors++;
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