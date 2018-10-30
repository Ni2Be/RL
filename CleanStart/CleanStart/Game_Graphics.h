#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Game_Graphics
{
public:
	Game_Graphics(int width, int height, const std::string title);
	~Game_Graphics();

	void update();
	sf::RenderWindow& window() { return m_window; }
	std::vector<std::unique_ptr<sf::Drawable>>& drawables() { return m_drawables; }
private:
	sf::RenderWindow m_window;
	std::vector<std::unique_ptr<sf::Drawable>> m_drawables;
};

