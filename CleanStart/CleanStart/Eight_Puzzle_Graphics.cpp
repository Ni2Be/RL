#include "Eight_Puzzle_Graphics.h"

void Eight_Puzzle_Graphics::update_graphics(const Playing_Field& playing_field)
{
	Game_Graphics::Draw_Container drawables;
	for (int row = 0; row < playing_field.size(); row++)
	{
		for (int collumn = 0; collumn < playing_field[row].size(); collumn++)
		{
			sf::RectangleShape field(sf::Vector2f(m_field_pixel, m_field_pixel));
			field.setOutlineThickness(2);
			field.setOutlineColor(sf::Color::Black);
			field.setPosition(sf::Vector2f(m_field_pixel * collumn, m_field_pixel * row));
			drawables.push_back(std::make_unique<sf::RectangleShape>(field));

			if (playing_field[row][collumn] != 0)
			{
				sf::Text field_label;
				field_label.setString(std::to_string(playing_field[row][collumn]));
				field_label.setCharacterSize(m_field_pixel - 25);
				field_label.setFont(m_arial);
				field_label.setOrigin(field_label.getLocalBounds().left + field_label.getLocalBounds().width / 2, field_label.getLocalBounds().top + field_label.getLocalBounds().height / 2);
				field_label.setPosition(sf::Vector2f(m_field_pixel * collumn + m_field_pixel / 2.0f, m_field_pixel * row + m_field_pixel / 2.0f));
				field_label.setFillColor(sf::Color::Black);
				drawables.push_back(std::make_unique<sf::Text>(field_label));
			}
		}
	}
	update_drawables(drawables);
}
