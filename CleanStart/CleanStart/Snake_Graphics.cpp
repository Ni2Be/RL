#include "Snake_Graphics.h"
#include "Snake_World.h"
#include <iostream>

using namespace Ai_Arena;


void Snake_Graphics::update_graphics(Snake_World world)
{
	Game_Graphics::Draw_Container drawables;


	
	//TODO background_g
	for (int x = 0; x < world.playing_field.size(); x++)
	{
		for (int y = 0; y < world.playing_field[0].size(); y++)
		{
			
			sf::RectangleShape rect(sf::Vector2f(m_field_pixel, m_field_pixel));
			rect.setPosition(x * m_field_pixel, y * m_field_pixel);
			if (world.playing_field[x][y] == Snake_World::Playing_Field::WALL)
			{
				//rect.setFillColor(sf::Color::Red);
				rect.setTexture(&wall);
			}
			else
				
				rect.setTexture(&ground);
			//rect.setFillColor(sf::Color::Red);
			
			drawables.push_back(std::make_shared<sf::RectangleShape>(rect));
		}
	}

	//snakes
	int i = 0;
	for (auto& snake : world.snakes)
	{
		switch (i++)
		{
		case 0:
			drawables.push_back(std::make_shared<Snake_Entity_G>(Snake_Entity_G(snake, m_field_pixel, sf::Color::Cyan, sf::Color::Cyan)));
			break;
		case 1:
			drawables.push_back(std::make_shared<Snake_Entity_G>(Snake_Entity_G(snake, m_field_pixel, sf::Color::Green, sf::Color::Green)));
			break;
		case 2:
			drawables.push_back(std::make_shared<Snake_Entity_G>(Snake_Entity_G(snake, m_field_pixel, sf::Color::Yellow, sf::Color::Yellow)));
			break;
		case 3:
			drawables.push_back(std::make_shared<Snake_Entity_G>(Snake_Entity_G(snake, m_field_pixel, sf::Color::Magenta, sf::Color::Magenta)));
			break;
		default:
			drawables.push_back(std::make_shared<Snake_Entity_G>(Snake_Entity_G(snake, m_field_pixel, sf::Color::Black, sf::Color::Black)));
			break;
		}
	}
	//TODO apple
	sf::RectangleShape apple(sf::Vector2f(m_field_pixel, m_field_pixel));
	apple.setPosition(world.apple.position.x * m_field_pixel, world.apple.position.y * m_field_pixel);
	//apple.setFillColor(sf::Color::Green);
	apple.setTexture(&apple_g);
	drawables.push_back(std::make_shared<sf::RectangleShape>(apple));

	update_drawables(drawables);
}

Snake_Graphics::Snake_Entity_G::Snake_Entity_G(const Snake_Entity& snake, int field_pixel, const sf::Color head_color, const sf::Color color)
	:
	m_color(color),
	m_head_color(head_color)


{
	
	
		for (const auto& segment : snake.body())
		{
			if (segment.is_head())
				segments.push_back(Snake_Segment_G(segment, field_pixel, m_head_color));
			else
				segments.push_back(Snake_Segment_G(segment, field_pixel, m_color));
		}
	
};


void Snake_Graphics::Snake_Entity_G::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto segment : segments)
		target.draw(segment, states);
}

Snake_Graphics::Snake_Segment_G::Snake_Segment_G(const Snake_Segment& segment, int field_pixel, sf::Color color)
	:
	rect(sf::Vector2f(field_pixel, field_pixel))
	
{

		rect.setFillColor(color);
		rect.setTexture(&body_n);

	if (segment.is_head())
	{
		//rect.setOutlineThickness(2);
		//rect.setOutlineColor(sf::Color::Black);
		
		if (segment.direction() == U)
			rect.setTexture(&head_u);
		if (segment.direction() == D)
			rect.setTexture(&head_d);
		
		if (segment.direction() == L)
			rect.setTexture(&head_l);
		if (segment.direction() == R)
			rect.setTexture(&head_r);
		
			

		//rect.rotate(90);
	}
	else
	{
		//rect.setOutlineThickness(1);
		//rect.setOutlineColor(sf::Color::Color(20,20,20));
		
		

		if (segment.is_tail())
		{
			if (segment.direction() == U)
				rect.setTexture(&tail_u);
			if (segment.direction() == D)
				rect.setTexture(&tail_d);

			if (segment.direction() == L)
				rect.setTexture(&tail_l);
			if (segment.direction() == R)
				rect.setTexture(&tail_r);
			
		}
		else 
		{
			//if ()
		
			if (segment.direction() == NO_ACTION)
					rect.setTexture(&body_n);

			if (segment.direction() == U || segment.direction() == D)
			{

				if (segment.turn() == DL)
					rect.setTexture(&body_dl);
				else if (segment.turn() == DR)
					rect.setTexture(&body_dr);
				else if (segment.turn() == UR)
					rect.setTexture(&body_ur);
				else if (segment.turn() == UL) 
					rect.setTexture(&body_ul);
				else
				rect.setTexture(&body_v);
			}
		if (segment.direction() == L || segment.direction() == R)

			if (segment.turn() == LD)
				rect.setTexture(&body_ur);
			else if (segment.turn() == RD)
				rect.setTexture(&body_ul);
			else if (segment.turn() == RU)
				rect.setTexture(&body_dl);
			else if (segment.turn() == LU) 
				rect.setTexture(&body_dr);
			else
			rect.setTexture(&body_h);
		}

		
	}
	rect.setPosition(
		sf::Vector2f(segment.position().x * field_pixel, segment.position().y * field_pixel));  
}

void Snake_Graphics::Snake_Segment_G::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
}

Snake_Graphics::Apple_G::Apple_G(const Apple& apple)
{
	//TODO
}

void Snake_Graphics::Apple_G::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//TODO
}