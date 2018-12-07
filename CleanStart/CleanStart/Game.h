#pragma once
#include "Game_Graphics.h"

#include <memory>
#include <thread>
#include "SFML/Graphics.hpp"
#include "Environment.h"

namespace Ai_Arena
{
	template<class Graphics>
	class Game_Base : public Environment
	{
		static_assert(std::is_base_of<Game_Graphics, Graphics>::value, "Game_Base template class \"Graphics\" must be derived from Game_Graphics");
	public:
		Game_Base();
		~Game_Base();

		//is called in run immediately before the game loop stats
		virtual void set_up() {};
		virtual void run();
		virtual void execute_actions() {};

		//GET/SET
		std::shared_ptr<Graphics>& graphics() { return m_graphics; };
		void      graphics(std::shared_ptr<Graphics> graphics) { m_graphics = graphics; }

	protected:
		std::shared_ptr<Graphics> m_graphics;

		virtual void update() = 0;
	};

	typedef Game_Base<Game_Graphics> Game;

	template<class Graphics>
	Game_Base<Graphics>::Game_Base()
	{
		if constexpr (std::is_same<Graphics, Game_Graphics>::value)
		{
			graphics(std::shared_ptr<Game_Graphics>(new Game_Graphics(200, 200, "Game")));
		}
	}

	template<class Graphics>
	Game_Base<Graphics>::~Game_Base()
	{
	}

	template<class Graphics>
	void Game_Base<Graphics>::run()
	{
		//call the possibly overwriten set up function
		set_up();

		//set the end time when the game will be updated
		auto start_time = std::chrono::system_clock::now();
		auto end_time   = start_time + update_interval();

		while (m_is_running)
		{
			//get all input events
			sf::Event event;
			std::queue<sf::Event> events;
			while (m_graphics->window().pollEvent(event))
			{
				events.push(event);
				if (event.type == sf::Event::Closed)
				{
					m_is_running = false;
					m_graphics->is_rendering() = false;
				}
			}
			set_events(events);
			
			//if one update_interval is passed, update the physics
			if (std::chrono::system_clock::now() > end_time)
			{
				//set the new end time for the game update
				start_time = std::chrono::system_clock::now();
				end_time   = start_time + update_interval();

				update();
			}
		}
		
		//when the game is finished shut down the actor threads
		for (auto& actor : actors())
			actor.actor->shut_down();
	}
}