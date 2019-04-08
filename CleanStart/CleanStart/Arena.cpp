#include "Arena.h"
#include <chrono>
using namespace Ai_Arena;

void Arena::run()
{

	//Pong
	m_pong_enviroment = std::shared_ptr<Environment<Pong_World>>(new Pong_Game(10, 150, 80, 10));

	//HUMAN
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Snake_Human_Player(m_enviroment)));
	//m_enviroment->add_actor(m_actors[0]);

	//AGENTS
	m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new TD_Agent<Pong_World>(m_pong_enviroment)));
	m_pong_enviroment->add_actor(m_pong_actors[0]);
	m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new Random_Agent<Pong_World>(m_pong_enviroment)));
	m_pong_enviroment->add_actor(m_pong_actors[1]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Random_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[2]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new MCTS_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[3]);

	m_pong_enviroment->update_interval() = std::chrono::milliseconds(1);


	for (auto& actor : m_pong_actors)
		actor->start_actor_thread();
	m_pong_enviroment->run();

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	std::cout << "end\n";
	}


/*
	//Pong
	m_pong_enviroment = std::shared_ptr<Environment<Pong_World>>(new Pong_Game(10, 150, 80, 10));

	//HUMAN
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Snake_Human_Player(m_enviroment)));
	//m_enviroment->add_actor(m_actors[0]);

	//AGENTS
	m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new MCTS_Agent<Pong_World>(m_pong_enviroment)));
	m_pong_enviroment->add_actor(m_pong_actors[0]);
	m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new MCTS_Agent<Pong_World>(m_pong_enviroment)));
	m_pong_enviroment->add_actor(m_pong_actors[1]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Random_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[2]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new MCTS_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[3]);

	m_pong_enviroment->update_interval() = std::chrono::milliseconds(50);


	for (auto& actor : m_pong_actors)
		actor->start_actor_thread();
	m_pong_enviroment->run();

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	std::cout << "end\n";
*/

/*
void Arena::snake()
{
	//SNAKE
	m_enviroment = std::shared_ptr<Environment<Snake_World>>(new Snake_Game(10, 40));


	//HUMAN
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Snake_Human_Player(m_enviroment)));
	//m_enviroment->add_actor(m_actors[0]);

	//AGENTS
	m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new MCTS_Agent<Snake_World>(m_enviroment)));
	m_enviroment->add_actor(m_actors[0]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Reflex_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[1]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Random_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[2]);
	//m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new MCTS_Agent<Snake_World>(m_enviroment)));
	//m_enviroment->add_actor(m_actors[3]);

	m_enviroment->update_interval() = std::chrono::milliseconds(50);


	for (auto& actor : m_actors)
		actor->start_actor_thread();
	m_enviroment->run();

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	std::cout << "end\n";
}
*/