#include "Eight_Puzzle.h"

#include <random>
#include <numeric>
#include <algorithm>

#include "Eight_Puzzle_Graphics.h"

Eight_Puzzle::Eight_Puzzle(int width, int height, int field_pixel)
	:
	m_field_width(width),
	m_field_height(height)
{
	graphics(std::shared_ptr<Eight_Puzzle_Graphics>(new Eight_Puzzle_Graphics(field_pixel * width, field_pixel * height, "Eight Puzzle", field_pixel)));
	do
	{
		build_field();
	} while (!is_solvable());
	m_game_state = convert_to_state(m_playing_field);
}


Eight_Puzzle::~Eight_Puzzle()
{
}

//GAME

void Eight_Puzzle::build_field()
{
	/*
	builds a playing field using numbers from 1 to n - 1 for fields
	and 0 for the empty field
	*/
	m_playing_field = Playing_Field(m_field_height, std::vector<int>(m_field_width));
	std::vector<int> field_values(m_field_width * m_field_height);
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

//TODO
bool Eight_Puzzle::is_solvable()
{
	int inversion_counter = 0;
	console_print(m_playing_field);
	for (int l_row = 0; l_row < m_playing_field.size(); l_row++)
		for (int l_collumn = 0; l_collumn < m_playing_field[l_row].size(); l_collumn++)
			for (int r_row = l_row, r_collumn = l_collumn; r_row < m_playing_field.size(); r_row++)
			{
				for (; r_collumn < m_playing_field[r_row].size(); r_collumn++)
					if ((m_playing_field[l_row][l_collumn] > m_playing_field[r_row][r_collumn]) && (m_playing_field[r_row][r_collumn] != 0))
						inversion_counter++;
				r_collumn = 0;
			}

	/*
	In general, for a given grid of width N, we can find out check if a N*N – 1 puzzle is solvable or not by following below simple rules :
    If N is odd, then puzzle instance is solvable if number of inversions is even in the input state.
    If N is even, puzzle instance is solvable if
        the blank is on an even row counting from the bottom (second-last, fourth-last, etc.) and number of inversions is odd.
        the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.) and number of inversions is even.
    For all other cases, the puzzle instance is not solvable.
	*/
	if (m_field_width % 2 == 0)
		if ((m_field_height - m_emty_position.row) % 2 == 0)
			return !(inversion_counter % 2 == 0);
		else
			return (inversion_counter % 2 == 0);
	else
		return (inversion_counter % 2 == 0);
}

void Eight_Puzzle::update_game()
{
	if (m_counter == 0)
	{
		m_graphics->update_graphics(m_playing_field);
	}
	m_counter++;


	srand(std::chrono::system_clock::now().time_since_epoch().count());
	applay_action(m_game_state, static_cast<Action>(rand() % (Action::L + 1)));
	
	
	if (m_counter % 100000 == 0)
	{
		std::cout << "trys: " << m_counter << std::endl;
		m_graphics->update_graphics(m_playing_field);
	}
	if (prove_victory_condition())
	{
		std::cout << "WIN!!! trys: " << m_counter << std::endl;
		m_graphics->update_graphics(m_playing_field);
		sf::sleep(sf::milliseconds(1000));
		m_counter = 0;
	}
		
}

bool Eight_Puzzle::prove_victory_condition()
{
	int field_value = m_playing_field[0][0];
	for (const auto& row : m_playing_field)
	{
		for (const auto& field : row)
		{
			if ((field_value > field) && (field != 0))
				return false;
			field_value = field;
		}
	}
	if (m_playing_field[m_field_height - 1][m_field_width - 1] == 0)
		return true;
	else
		return false;
}

void Eight_Puzzle::swap_field_values(Pos p1, Pos p2, Playing_Field& f)
{
	int temp = f[p1.row][p1.collumn];
	f[p1.row][p1.collumn] = f[p2.row][p2.collumn];
	f[p2.row][p2.collumn] = temp;
}

Eight_Puzzle::Playing_Field Eight_Puzzle::applay_action(Playing_Field playing_field, Action action)
{
	m_playing_field = assume_action(playing_field, action);
	return m_playing_field;
}

Eight_Puzzle::Playing_Field Eight_Puzzle::assume_action(Playing_Field playing_field, Action action)
{
	switch (action)
	{
	case Action::U:
		if (!(m_emty_position.row == playing_field.size() - 1))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row + 1, m_emty_position.collumn), playing_field);
			m_emty_position.row++;
		}
		break;
	case Action::D:
		if (!(m_emty_position.row == 0))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row - 1, m_emty_position.collumn), playing_field);
			m_emty_position.row--;
		}
		break;
	case Action::R:
		if (!(m_emty_position.collumn == playing_field[m_emty_position.row].size() - 1))
		{
			swap_field_values(m_emty_position, Pos(m_emty_position.row, m_emty_position.collumn + 1), playing_field);
			m_emty_position.collumn++;
		}
		break;
	case Action::L:
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


//CONVERTER
Eight_Puzzle::State Eight_Puzzle::convert_to_state(const Playing_Field playing_field) const
{
	State state(playing_field[0].size() * playing_field.size());
	int i = 0;
	for (const auto& row : playing_field)
		for (const auto& field : row)
			state[i++] = field;
	return state;
}

Eight_Puzzle::Playing_Field Eight_Puzzle::convert_to_playing_field(const State state) const
{
	Playing_Field field(m_playing_field);
	int i = 0;
	for (auto& row : field)
		for (auto& field : row)
			field = state[i++];
	return field;
}


//OBSERVABLE ENVIROMENT
Eight_Puzzle::Reward Eight_Puzzle::reward(State)
{
	if (prove_victory_condition())
		return 100.0;
	return 0.0;
}

std::vector<Action> Eight_Puzzle::possible_actions(State)
{
	return { Action::U, Action::D, Action::R, Action::L};
}

Eight_Puzzle::State Eight_Puzzle::assume_action(State state, Action action)
{
	auto field    = convert_to_playing_field(state);
	auto new_fiel = assume_action(field, action);
	return convert_to_state(new_fiel);
}

Eight_Puzzle::State Eight_Puzzle::applay_action(State state, Action action)
{
	auto field    = convert_to_playing_field(state);
	auto new_fiel = applay_action(field, action);
	m_game_state = convert_to_state(new_fiel);
	return convert_to_state(new_fiel);
}

Eight_Puzzle::State Eight_Puzzle::actual_state()
{
	m_game_state = convert_to_state(m_playing_field);
	return m_game_state;
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