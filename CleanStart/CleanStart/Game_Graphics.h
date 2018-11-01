#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include <iostream>
class Game_Graphics
{
public:
	Game_Graphics(int width, int height, const std::string title);
	~Game_Graphics();

	void start_rendering_thread();
	sf::RenderWindow& window() { return m_window; }

	typedef std::vector<std::shared_ptr<sf::Drawable>> Draw_Container;
	void update_drawables(Draw_Container& draw_vec) { std::lock_guard<std::mutex> lock(m_drawables_lock); m_drawables = draw_vec; }
	const Draw_Container& drawables() { std::lock_guard<std::mutex> lock(m_drawables_lock); return m_drawables; }
	bool& is_rendering() { return m_is_rendering; }
protected:
	std::mutex       m_drawables_lock;
	sf::RenderWindow m_window;
	std::thread      m_render_thread;
	Draw_Container   m_drawables;
	bool             m_is_rendering = true;

	sf::Font m_arial;

	void update();
};

