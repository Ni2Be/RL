#include "Game.h"
#include "SFML/Graphics.hpp"

#include <thread>

Game::Game(int width, int height, const std::string window_name)
	:
	m_graphics(width, height, window_name)
{
}

Game::~Game()
{
}

void Game::run()
{
	while (m_is_running)
	{
		sf::Event event;
		while (m_graphics.window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_is_running = false;
		}
		update_game();
	}
	m_graphics.window().close();
}
