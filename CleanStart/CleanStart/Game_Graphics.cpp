#include "Game_Graphics.h"
#include <iostream>


Game_Graphics::Game_Graphics(int width, int height, const std::string title)
	:
	m_window(sf::VideoMode(width, height), title)
{
	m_window.display();
}


Game_Graphics::~Game_Graphics()
{
	
	m_window.close();
}

void Game_Graphics::update()
{
	if (m_window.isOpen())
	{
		m_window.clear();
		for (auto& drawable : m_drawables)
			m_window.draw(*drawable);
		m_window.display();
	}
}
