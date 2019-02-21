#include "Pong_Graphics.h"
#include "Pong_World.h"
#include <iostream>

using namespace Ai_Arena;

void Pong_Graphics::update_graphics(Pong_World world)
{
	Game_Graphics::Draw_Container drawables;

	for (int x = 0; x < world.playing_field.size(); x++)
	{
		for (int y = 0; y < world.playing_field[0].size(); y++)
		{
			sf::RectangleShape rect(sf::Vector2f(m_field_pixel, m_field_pixel));
			rect.setPosition(x * m_field_pixel, y * m_field_pixel);
			if (world.playing_field[x][y] == Pong_World::Playing_Field::WALL)
				rect.setFillColor(sf::Color::White);
			else if (world.playing_field[x][y] == Pong_World::Playing_Field::GOAL)
				rect.setFillColor(sf::Color(50, 50, 50, 255));
			else
				rect.setFillColor(sf::Color::Black);

			drawables.push_back(std::make_shared<sf::RectangleShape>(rect));
		}
	}


	int i = 0;
	for (auto& paddle : world.paddles)
	{
		switch (i++)
		{
		case 0:
			drawables.push_back(std::make_shared<Paddle_G>(Paddle_G(paddle, m_field_pixel, sf::Color::Cyan)));
			//std::cout << paddle.position().x << " " << paddle.position().y << std::endl;
			break;
		case 1:
			drawables.push_back(std::make_shared<Paddle_G>(Paddle_G(paddle, m_field_pixel, sf::Color::Green)));
			break;
		default:
			drawables.push_back(std::make_shared<Paddle_G>(Paddle_G(paddle, m_field_pixel, sf::Color::White)));
			break;
		}
	}
	sf::RectangleShape ball(sf::Vector2f(m_field_pixel, m_field_pixel));
	ball.setPosition(world.ball.position.x * m_field_pixel, world.ball.position.y * m_field_pixel);
	ball.setFillColor(sf::Color::White);
	drawables.push_back(std::make_shared<sf::RectangleShape>(ball));

	update_drawables(drawables);
}

Pong_Graphics::Paddle_G::Paddle_G(const Paddle& paddle, int field_pixel, const sf::Color color)
	:
	m_color(color)
{
	for (const auto& segment : paddle.body())
	{
			segments.push_back(Paddle_Segment_G(segment, field_pixel, m_color));
	}

};


void Pong_Graphics::Paddle_G::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto segment : segments)
		target.draw(segment, states);
}

Pong_Graphics::Paddle_Segment_G::Paddle_Segment_G(const Paddle_Segment& segment, int field_pixel, sf::Color color)
	:
	rect(sf::Vector2f(field_pixel, field_pixel))

{
	rect.setFillColor(color);


	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Black);


	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Color(20, 20, 20));




	rect.setPosition(sf::Vector2f(segment.position().x * field_pixel, segment.position().y * field_pixel));
}

void Pong_Graphics::Paddle_Segment_G::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
}

Pong_Graphics::Ball_G::Ball_G(const Ball& ball)
{
	//TODO
}

void Pong_Graphics::Ball_G::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//TODO
}
