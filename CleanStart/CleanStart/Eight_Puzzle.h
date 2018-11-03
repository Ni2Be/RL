#pragma once
#include "Game.h"
#include "I_Observable_Environment.h"
#include "Eight_Puzzle_Graphics.h"


#include <iostream>
#include <SFML/Graphics.hpp>


enum Action
{
	U, D, R, L
};
class Eight_Puzzle : public Game_Base<Eight_Puzzle_Graphics>, public I_Observable_Environment<Action>
{
public:

	typedef std::vector<std::vector<int>> Playing_Field;
	Eight_Puzzle(int width = 3, int height = 3, int field_width = 40);
	~Eight_Puzzle();

private:
	int m_field_width;
	int m_field_height;

	Playing_Field m_playing_field;
	struct Pos { int row; int collumn; Pos(int r = 0, int c = 0) : row(r), collumn(c) {} };
	Pos m_emty_position;

	//Game
	int m_counter = 0;
	void build_field();
	bool is_solvable();
	void update_game();

	Playing_Field applay_action(Playing_Field, Action);
	Playing_Field assume_action(Playing_Field, Action);
	void swap_field_values(Pos p1, Pos p2, Playing_Field& f);

	bool prove_victory_condition();


	//Converter
	State convert_to_state(const Playing_Field playing_field) const;
	Playing_Field convert_to_playing_field(const State game_state) const;

public:
	//Observable Enviroment
	Reward reward(State);
	std::vector<Action> possible_actions(State);
	State assume_action(State, Action);
	State applay_action(State, Action);
	State actual_state();
	
private:
	//DEBUG
	void console_print(Playing_Field playing_field);
};
