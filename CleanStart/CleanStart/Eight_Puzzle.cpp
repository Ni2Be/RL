#include "Eight_Puzzle.h"

#include <random>
#include <numeric>
#include <algorithm>



Eight_Puzzle::Eight_Puzzle(int width, int height, int field_width)
	:
	Game::Game(field_width * width, field_width * height, "Eight Puzzle"),
	m_width(width),
	m_height(height),
	m_field_width(field_width)
{
	m_arial.loadFromFile("arial.ttf");
	build_field();
	m_game_state = m_playing_field;
}


Eight_Puzzle::~Eight_Puzzle()
{
}


void Eight_Puzzle::build_field()
{
	/*
	builds a playing field using numbers from 1 to n - 1 for fields
	and 0 for the empty field
	*/
	m_playing_field = Playing_Field(m_height, std::vector<int>(m_width));
	std::vector<int> field_values(m_width * m_height);
	std::iota(field_values.begin(), field_values.end(), 0);
	std::shuffle(field_values.begin(), field_values.end(), std::mt19937{ std::random_device{}() });

	int i = 0;
	for (int row = 0; row < m_playing_field.size(); row++)
	{
		for (int collumn = 0; collumn < m_playing_field[row].size(); collumn++)
		{
			if (field_values[i] == 0)
			{
				m_emty_position.row = row;
				m_emty_position.collumn = collumn;
			}
			m_playing_field[row][collumn] = field_values[i++];
		}
	}
}


void Eight_Puzzle::update_game()
{
	applay_action(m_playing_field, static_cast<Actions>(rand() % (Actions::L + 1)));

	update_graphics();
	sf::sleep(sf::milliseconds(10));

}


//OBSERVABLE ENVIROMENT

double Eight_Puzzle::reward(Playing_Field)
{
	//if ()
		return 100.0;
	return 0.0;
}

std::vector<Actions> Eight_Puzzle::possible_actions(Playing_Field)
{
	return { Actions::U, Actions::D, Actions::R, Actions::L};
}

void Eight_Puzzle::swap_field_values(Pos p1, Pos p2, Playing_Field& f)
{
	int temp = f[p1.row][p1.collumn];
	f[p1.row][p1.collumn] = f[p2.row][p2.collumn];
	f[p2.row][p2.collumn] = temp;
}

Playing_Field Eight_Puzzle::assume_action(Playing_Field playing_field, Actions action)
{
	switch (action)
	{
	case Actions::U:
		std::cout << "--------UP--------\n";
		if (!(m_emty_position.row == playing_field.size() - 1))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row + 1, m_emty_position.collumn), playing_field);
			m_emty_position.row++;
		}
		break;
	case Actions::D:
		std::cout << "--------DOWN--------\n";
		if (!(m_emty_position.row == 0))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row - 1, m_emty_position.collumn), playing_field);
			m_emty_position.row--;
		}
		break;
	case Actions::R:
		std::cout << "--------RIGHT--------\n";
		if (!(m_emty_position.collumn == playing_field[m_emty_position.row].size() - 1))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row, m_emty_position.collumn + 1), playing_field);
			m_emty_position.collumn++;
		}
			break;
	case Actions::L:
		std::cout << "--------LEFT--------\n";
		if (!(m_emty_position.collumn == 0))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row, m_emty_position.collumn - 1), playing_field);
			m_emty_position.collumn--;
		}
		break;
	default:
		std::cerr << "invalid action" << std::endl;
	}
	return playing_field;
}

Playing_Field Eight_Puzzle::applay_action(Playing_Field playing_field, Actions action)
{
	m_game_state = assume_action(playing_field, action);
	m_playing_field = m_game_state;
	return m_game_state;
}

Playing_Field Eight_Puzzle::actual_state()
{
	return m_game_state;
}



//GRAPHICS

void Eight_Puzzle::update_graphics()
{
	graphics.drawables().clear();
	for (int row = 0; row < m_playing_field.size(); row++)
	{
		for (int collumn = 0; collumn < m_playing_field[row].size(); collumn++)
		{
			sf::RectangleShape field(sf::Vector2f(m_field_width, m_field_width));
			field.setOutlineThickness(2);
			field.setOutlineColor(sf::Color::Black);
			field.setPosition(sf::Vector2f(m_field_width * collumn, m_field_width * row));
			graphics.drawables().push_back(std::make_unique<sf::RectangleShape>(field));

			if (m_playing_field[row][collumn] != 0)
			{
				sf::Text field_label;
				field_label.setString(std::to_string(m_playing_field[row][collumn]));
				field_label.setCharacterSize(m_field_width - 25);
				field_label.setFont(m_arial);
				field_label.setOrigin(field_label.getLocalBounds().left + field_label.getLocalBounds().width / 2, field_label.getLocalBounds().top + field_label.getLocalBounds().height / 2);
				field_label.setPosition(sf::Vector2f(m_field_width * collumn + m_field_width / 2.0f, m_field_width * row + m_field_width / 2.0f));
				field_label.setFillColor(sf::Color::Black);
				graphics.drawables().push_back(std::make_unique<sf::Text>(field_label));
			}
		}
	}
}

//DEBUG

void Eight_Puzzle::console_print(Playing_Field playing_field)
{
	for (auto& row : playing_field)
	{
		for (auto& field : row)
			std::cout << field << " ";
		std::cout << std::endl;
	}
}