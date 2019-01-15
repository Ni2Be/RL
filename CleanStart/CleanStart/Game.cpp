#include "Game.h"

using namespace Ai_Arena;

template<class Graphics, class State_T>
Game_Base<Graphics, State_T>::Game_Base()
{
	if constexpr (std::is_same<Graphics, Game_Graphics>::value)
	{
		graphics(std::shared_ptr<Game_Graphics>(new Game_Graphics(200, 200, "Game")));
	}
}

template<class Graphics, class State_T>
Game_Base<Graphics, State_T>::~Game_Base()
{
}

template<class Graphics, class State_T>
void Game_Base<Graphics, State_T>::run()
{
	//call the possibly overwriten set up function
	set_up();

	//set the end time when the game will be updated
	auto start_time = std::chrono::system_clock::now();
	this->set_next_execution_time(start_time + Environment<State_T>::update_interval());
	
	while (Environment<State_T>::m_is_running)
	{
		//get all input events
		sf::Event event;
		std::queue<sf::Event> events;
		while (m_graphics->window().pollEvent(event))
		{
			events.push(event);
			if (event.type == sf::Event::Closed)
			{
				Environment<State_T>::m_is_running = false;
				m_graphics->is_rendering() = false;
			}
		}
		Environment<State_T>::set_events(events);

		//if one update_interval is passed, update the physics
		if (std::chrono::system_clock::now() > this->next_execution_time())
		{
			//set the new end time for the game update
			start_time = std::chrono::system_clock::now();
			this->set_next_execution_time(start_time + Environment<State_T>::update_interval());

			update();
		}
	}

	//when the game is finished shut down the actor threads
	for (auto& actor : Environment<State_T>::actors())
	{
		actor.actor->shut_down();
	}
}