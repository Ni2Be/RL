#pragma once
#include "Game_Graphics.h"

#include <memory>
#include <thread>

#include "SFML/Graphics.hpp"

template<class Graphics>
class Game_Base
{
	static_assert(std::is_base_of<Game_Graphics, Graphics>::value, "Game_Base template class must be derived from Game_Graphics");
public:
	Game_Base();
	~Game_Base();

	void run();
	Graphics& graphics() { return *m_graphics; };
	void      graphics(std::shared_ptr<Graphics> graphics) { m_graphics = graphics; }
protected:
	virtual void update_game() = 0;
	bool         m_is_running  = true;
	std::shared_ptr<Graphics> m_graphics;
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
	while (m_is_running)
	{
		sf::Event event;
		while (m_graphics->window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_is_running = false;
				m_graphics->is_rendering() = false;
			}
		}
		update_game();
	}
}
