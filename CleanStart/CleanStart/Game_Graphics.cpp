#include "Game_Graphics.h"
#include <iostream>

Game_Graphics::Game_Graphics(int width, int height, const std::string title)
	:
	m_window(sf::VideoMode(width, height), title),
	m_render_thread(&Game_Graphics::start_rendering_thread, this)
{
	m_window.setActive(false);
	m_render_thread.launch();
}


Game_Graphics::~Game_Graphics()
{
}

void Game_Graphics::update()
{
	sf::sleep(sf::milliseconds(33)); 
	std::lock_guard<std::mutex> lock(m_drawables_lock);
	if (m_window.isOpen())
	{
		m_window.clear();
		for (auto& drawable : m_drawables)
			m_window.draw(*drawable);
		m_window.display();
	}
}

void Game_Graphics::start_rendering_thread()
{
	m_window.setActive(true);

	while (m_window.isOpen())
	{
		update();
	}
}