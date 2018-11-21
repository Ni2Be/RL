#pragma once
#include "Game_Graphics.h"

#include <memory>
#include <thread>
#include "SFML/Graphics.hpp"
#include "I_Environment.h"

namespace Ai_Arena
{
	template<class Graphics>
	class Game_Base : public I_Environment
	{
		static_assert(std::is_base_of<Game_Graphics, Graphics>::value, "Game_Base template class must be derived from Game_Graphics");
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

		std::queue<sf::Event> get_events();
	protected:
		void set_events(std::queue<sf::Event> events);
	private:
		std::queue<sf::Event> m_events;
		std::mutex            m_event_lock;
	protected:
		bool m_is_running = true;
		
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
		set_up();

		auto start_time = std::chrono::system_clock::now();
		auto end_time   = start_time + update_interval();

		while (m_is_running)
		{
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
			
			if (std::chrono::system_clock::now() > end_time)
			{
				start_time = std::chrono::system_clock::now();
				end_time   = start_time + update_interval();

				update();
			}
		}
	}

	template<class Graphics>
	std::queue<sf::Event> Game_Base<Graphics>::get_events()
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

	template<class Graphics>
	void Game_Base<Graphics>::set_events(std::queue<sf::Event> events)
	{
		std::scoped_lock<std::mutex> lock(m_event_lock);
		while (!events.empty())
		{
			m_events.push(events.front());
			events.pop();
		}
	}
}