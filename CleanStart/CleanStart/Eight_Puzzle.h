#pragma once
#include "Game.h"
#include "I_Observable_Environment.h"
#include "Eight_Puzzle_Graphics.h"


#include <iostream>
#include <SFML/Graphics.hpp>

enum Actions
{
	U, D, R, L
};

typedef std::vector<std::vector<int>> Playing_Field;
class Eight_Puzzle : public Game_Base<Eight_Puzzle_Graphics>, public I_Observable_Environment
{
public:
	Eight_Puzzle(int width = 3, int height = 3, int field_width = 40);
	~Eight_Puzzle();

private:
	int m_field_width;
	int m_field_height;

	Playing_Field m_playing_field;
	struct Pos { int row; int collumn; Pos(int r = 0, int c = 0) : row(r), collumn(c) {} };
	void swap_field_values(Pos p1, Pos p2, Playing_Field& f);
	Pos m_emty_position;

	//Game
	int m_counter = 0;
	void build_field();
	bool is_solvable();
	void update_game();

	bool prove_victory_condition();

	//Observable Enviroment
	//double reward(Playing_Field);
	//std::vector<Actions> possible_actions(Playing_Field);
	//Playing_Field assume_action(Playing_Field, Actions);
	//Playing_Field applay_action(Playing_Field, Actions);
	//Playing_Field actual_state();
	

	//DEBUG
	void console_print(Playing_Field playing_field);
};
