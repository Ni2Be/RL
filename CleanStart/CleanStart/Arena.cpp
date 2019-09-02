#include "Arena.h"
#include <chrono>
using namespace Ai_Arena;

void Arena::run()
{

#ifdef SNAKE
	//SNAKE NEW
		//Snake
		m_enviroment = std::shared_ptr<Environment<Snake_World>>(new Snake_Game(12, 40));
	for (int i = 0; i < humanPlayers; i++) {
		m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Snake_Human_Player(m_enviroment)));
		m_enviroment->add_actor(m_actors[addedAgents]);
		addedAgents++;
	}
	//AGENTS
	//MCTS
	for (int i = 0; i < MCTSAgents; i++) {
		m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new MCTS_Agent<Snake_World>(m_enviroment)));
		m_enviroment->add_actor(m_actors[addedAgents]);
		addedAgents++;
	}
	//Reflex
	for (int i = 0; i < ReflexAgents; i++) {
		m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Reflex_Agent<Snake_World>(m_enviroment)));
		m_enviroment->add_actor(m_actors[addedAgents]);
		addedAgents++;
	}
	//Random
	for (int i = 0; i < RandomAgents; i++) {
		m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new Random_Agent<Snake_World>(m_enviroment)));
		m_enviroment->add_actor(m_actors[addedAgents]);
		addedAgents++;
	}
	//TD
	for (int i = 0; i < TDAgents; i++) {
		m_actors.push_back(std::shared_ptr<Actor<Snake_World>>(new TD_Agent<Snake_World>(m_enviroment)));
		m_enviroment->add_actor(m_actors[addedAgents]);
		addedAgents++;
	}

	if (m_trainings_mode)
		m_enviroment->update_interval() = std::chrono::milliseconds(0);
	else
		m_enviroment->update_interval() = std::chrono::milliseconds(200);




	for (auto& actor : m_actors)
		actor->start_actor_thread();

	m_enviroment->run();
#endif

#ifdef PONG
	//NEW PONG
		//Pong
		m_pong_enviroment = std::shared_ptr<Environment<Pong_World>>(new Pong_Game(10, 150, 80, 10));
	for (int i = 0; i < humanPlayers; i++) {
		m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new Pong_Human_Player(m_pong_enviroment)));
		m_pong_enviroment->add_actor(m_pong_actors[addedAgents]);
		addedAgents++;
	}
	//AGENTS
	//MCTS
	for (int i = 0; i < MCTSAgents; i++) {
		m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new MCTS_Agent<Pong_World>(m_pong_enviroment)));
		m_pong_enviroment->add_actor(m_pong_actors[addedAgents]);
		addedAgents++;
	}
	//Reflex
	for (int i = 0; i < ReflexAgents; i++) {
		m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new Reflex_Agent<Pong_World>(m_pong_enviroment)));
		m_pong_enviroment->add_actor(m_pong_actors[addedAgents]);
		addedAgents++;
	}
	//Random
	for (int i = 0; i < RandomAgents; i++) {
		m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new Random_Agent<Pong_World>(m_pong_enviroment)));
		m_pong_enviroment->add_actor(m_pong_actors[addedAgents]);
		addedAgents++;
	}
	//TD
	for (int i = 0; i < TDAgents; i++) {
		m_pong_actors.push_back(std::shared_ptr<Actor<Pong_World>>(new TD_Agent<Pong_World>(m_pong_enviroment)));
		m_pong_enviroment->add_actor(m_pong_actors[addedAgents]);
		addedAgents++;
	}

	if(m_trainings_mode)
		m_pong_enviroment->update_interval() = std::chrono::milliseconds(0);
	else
		m_pong_enviroment->update_interval() = std::chrono::milliseconds(30);

	for (auto& actor : m_pong_actors)
		actor->start_actor_thread();
	m_pong_enviroment->run();
#endif

	//TODO should wait more properly for agents
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "end\n";
}
