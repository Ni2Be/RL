#pragma once
#include "Game.h"
#include "I_Observable_Environment.h"

#include <iostream>
#include <SFML/Graphics.hpp>

enum Actions
{
	U, D, R, L
};

typedef std::vector<std::vector<int>> Playing_Field;
class Eight_Puzzle : public Game, public I_Observable_Environment<Playing_Field, Actions, double>
{
public:
	Eight_Puzzle(int width = 3, int height = 3, int field_width = 40);
	~Eight_Puzzle();

private:
	int m_width;
	int m_height;

	int m_field_width;
	Playing_Field m_playing_field;
	struct Pos { int row; int collumn; Pos(int r = 0, int c = 0) : row(r), collumn(c) {} };
	void swap_field_values(Pos p1, Pos p2, Playing_Field& f);
	Pos m_emty_position;

	//Game
	int m_counter = 0;
	void build_field();
	void update_game();

	bool prove_victory_condition();

	//Observable Enviroment
	double reward(Playing_Field);
	std::vector<Actions> possible_actions(Playing_Field);
	Playing_Field assume_action(Playing_Field, Actions);
	Playing_Field applay_action(Playing_Field, Actions);
	Playing_Field actual_state();
	

	//graphics
	sf::Font m_arial;
	void update_graphics();

	//DEBUG
	void console_print(Playing_Field playing_field);
};
