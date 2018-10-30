#include "Game.h"
#include "SFML/Graphics.hpp"


Game::Game(int width, int height, const std::string window_name)
	:
	graphics(width, height, window_name)
{
}

Game::~Game()
{
}

void Game::run()
{
	while (is_running)
	{
		sf::Event event;
		while (graphics.window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				is_running = false;
		}
		update_game();
		graphics.update();
	}
	graphics.window().close();
}
