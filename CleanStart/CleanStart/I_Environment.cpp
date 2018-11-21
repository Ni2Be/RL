#include "I_Environment.h"
#include "Actor.h"

using namespace Ai_Arena;
Action I_Environment::exchange_action(std::shared_ptr<Actor> actor, Action new_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	Action past_action = m_actors[actor->id()].second;
	m_actors[actor->id()].second = new_action;
	return past_action;
}

void I_Environment::add_actor(std::pair<std::shared_ptr<Actor>, Action> actor_action)
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	actor_action.first->id() = m_actors.size();
	m_actors.push_back(actor_action);
	if (actor_action.first->is_human())
		m_human_actors++;
}
