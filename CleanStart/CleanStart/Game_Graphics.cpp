#include "Game_Graphics.h"
#include <iostream>


using namespace Ai_Arena;


sf::Texture Game_Graphics::head_u;
sf::Texture Game_Graphics::body_ur;
sf::Texture Game_Graphics::body_ul;
sf::Texture Game_Graphics::body_dr;
sf::Texture Game_Graphics::body_dl;
sf::Texture Game_Graphics::head_d;
sf::Texture Game_Graphics::head_l;
sf::Texture Game_Graphics::head_r;
sf::Texture Game_Graphics::body_h;
sf::Texture Game_Graphics::body_v;
sf::Texture Game_Graphics::body_n;
sf::Texture Game_Graphics::tail_u;
sf::Texture Game_Graphics::tail_d;
sf::Texture Game_Graphics::tail_l;
sf::Texture Game_Graphics::tail_r;
sf::Texture Game_Graphics::wall;
sf::Texture Game_Graphics::ground;
sf::Texture Game_Graphics::apple_g;


Game_Graphics::Game_Graphics(int width, int height, const std::string title)
	:
	m_window(sf::VideoMode(width, height), title)
{
	
	load_resources();
	

	m_arial.loadFromFile("arial.ttf");
	m_window.setActive(false);
	m_render_thread = std::thread(&Game_Graphics::start_rendering_thread, this);
}


Game_Graphics::~Game_Graphics()
{}

void Game_Graphics::render()
{
	sf::sleep(sf::milliseconds(33));
	std::scoped_lock<std::mutex> lock(m_drawables_lock);
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
	while (m_is_rendering)
		render();

	m_window.setVisible(false);
	m_window.close();
}

void Game_Graphics::load_resources()
{
	m_arial.loadFromFile("arial.ttf");
	if (!body_ul.loadFromFile("graphics/body_ul.png"))
	{
		std::cout << "Load failed" << std::endl;
	}
	if (!body_ur.loadFromFile("graphics/body_ur.png"))
	{
		std::cout << "Load failed" << std::endl;
	}
	if (!body_dl.loadFromFile("graphics/body_dl.png"))
	{
		std::cout << "Load failed" << std::endl;
	}
	if (!body_dr.loadFromFile("graphics/body_dr.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!head_u.loadFromFile("graphics/head_u.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!head_d.loadFromFile("graphics/head_d.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!head_l.loadFromFile("graphics/head_l.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!head_r.loadFromFile("graphics/head_r.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!body_v.loadFromFile("graphics/body_v.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!body_h.loadFromFile("graphics/body_h.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!body_n.loadFromFile("graphics/body_n.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!tail_u.loadFromFile("graphics/tail_u.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!tail_d.loadFromFile("graphics/tail_d.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!tail_l.loadFromFile("graphics/tail_l.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!tail_r.loadFromFile("graphics/tail_r.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	if (!wall.loadFromFile("graphics/wall.png"))
	{
		std::cout << "Load failed" << std::endl;
	}
	if (!ground.loadFromFile("graphics/ground.png"))
	{
		std::cout << "Load failed" << std::endl;
	}
	if (!apple_g.loadFromFile("graphics/apple.png"))
	{
		std::cout << "Load failed" << std::endl;
	}
}