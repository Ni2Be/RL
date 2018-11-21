#include "Eight_Puzzle_Graphics.h"

using namespace Ai_Arena;

void Eight_Puzzle_Graphics::update_graphics(const Playing_Field& playing_field)
{
	Game_Graphics::Draw_Container drawables;
	for (int row = 0; row < playing_field.size(); row++)
	{
		for (int collumn = 0; collumn < playing_field[row].size(); collumn++)
		{
			Labeled_Rect l_rect(
				sf::Vector2f(m_field_pixel, m_field_pixel), 
				sf::Vector2f(m_field_pixel * collumn, m_field_pixel * row),
				sf::Color::White,
				arial(),
				std::to_string(playing_field[row][collumn]),
				m_field_pixel - 25);

			if (playing_field[row][collumn] == 0)
				l_rect.label.setString("");

			drawables.push_back(std::make_unique<Labeled_Rect>(l_rect));
		}
	}
	update_drawables(drawables);
}

Eight_Puzzle_Graphics::Labeled_Rect::Labeled_Rect(
	sf::Vector2f size, 
	sf::Vector2f pos, 
	sf::Color    color, 
	const sf::Font& font, 
	std::string     text, 
	int             text_size)
	:
	rect(size),
	label()
{	 
	rect.setFillColor       (color);
	rect.setOutlineThickness(2);
	rect.setOutlineColor    (sf::Color::Black);
	rect.setPosition        (pos);
	

	label.setString       (text);
	label.setCharacterSize(text_size);
		
	label.setFont(font);
	label.setOrigin(
		label.getLocalBounds().left + label.getLocalBounds().width  / 2, 
		label.getLocalBounds().top  + label.getLocalBounds().height / 2);
	label.setPosition (pos + sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
	label.setFillColor(sf::Color::Black);
}

void Eight_Puzzle_Graphics::Labeled_Rect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect,  states);
	target.draw(label, states);
}