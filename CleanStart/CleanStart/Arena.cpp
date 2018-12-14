#include "Arena.h"
#include <chrono>
using namespace Ai_Arena;

void Arena::run()
{
	//SNAKE
	m_enviroment = std::shared_ptr<Environment<State_Type>>(new Snake_Game(15, 40));


	//HUMAN
	m_actors.push_back(std::shared_ptr<Actor<State_Type>>(new Snake_Human_Player(m_enviroment)));
	m_enviroment->add_actor(m_actors[0]);

	//AGENTS
	m_actors.push_back(std::shared_ptr<Actor<State_Type>>(new Random_Agent<State_Type>(m_enviroment)));
	m_enviroment->add_actor(m_actors[1]);
	m_actors.push_back(std::shared_ptr<Actor<State_Type>>(new Random_Agent<State_Type>(m_enviroment)));
	m_enviroment->add_actor(m_actors[2]);

	m_enviroment->update_interval() = std::chrono::milliseconds(200);


	for (auto& actor : m_actors)
		actor->start_actor_thread();
	m_enviroment->run();


	std::cout << "end\n";
	
	//EIGHT PUZZLE
	//m_enviroment_2 = std::shared_ptr<Environment>(new Eight_Puzzle(3, 3, 90));

	////AGENT
	//m_actors_2.push_back(std::shared_ptr<Actor>(new Random_Agent(m_enviroment_2)));
	//m_enviroment_2->add_actor(m_actors_2[0]);

	//m_enviroment_2->update_interval() = std::chrono::milliseconds(0);

	//m_actors_2[0]->start_actor_thread();
	//m_enviroment_2->run();
}
