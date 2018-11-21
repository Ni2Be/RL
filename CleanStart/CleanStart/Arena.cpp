#include "Arena.h"
#include <chrono>

using namespace Ai_Arena;

void Arena::run()
{
	
	for (auto& actor : m_actors)
		actor->start_actor_thread();
	m_enviroment->run();


	//m_actors_2[0]->start_actor_thread();
	//m_enviroment_2->run();
}
