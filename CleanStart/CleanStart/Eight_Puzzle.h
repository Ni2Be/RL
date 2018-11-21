#pragma once
#include "Game.h"
#include "Eight_Puzzle_Graphics.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <atomic>

namespace Ai_Arena
{
	class Eight_Puzzle : public Game_Base<Eight_Puzzle_Graphics>
	{
	public:
		enum Actions
		{
			U, D, R, L, NO_ACTION
		};

		struct Playing_Field : public std::vector<std::vector<int>>
		{
			Playing_Field() {}
			Playing_Field(int i, std::vector<int> v)
				:
				std::vector<std::vector<int>>(i, v)
			{}
			Pos_int emty_position;
		};

		Eight_Puzzle(int width = 3, int height = 3, int field_width = 90);
		~Eight_Puzzle();

	private:
		int m_field_width;
		int m_field_height;

		Playing_Field m_playing_field;

		//Game
		int m_counter = 0;
		std::atomic<int> m_action_counter = 0;
		void build_field();
		bool is_solvable();
		void update_puzzle();

		void execute_actions();
		Playing_Field execute_action(Playing_Field, Action) const;
		void swap_field_values(Pos_int p1, Pos_int p2, Playing_Field& f) const;

		bool prove_victory_condition(Playing_Field) const;

		//Converter
		State convert_to_state(const Playing_Field playing_field) const;
		Playing_Field convert_to_playing_field(const State game_state) const;

	public:
		//Observable Enviroment
		std::vector<Action> possible_actions(std::shared_ptr<Actor>, State) const;
		std::vector<State>  assume_action(std::shared_ptr<Actor>, State, Action) const;
		void   apply_action(std::shared_ptr<Actor>, Action);
		State  actual_state(std::shared_ptr<Actor>) const;
		Reward reward(std::shared_ptr<Actor>, State) const;
		bool   is_final(std::shared_ptr<Actor>, State) const;
		void add_actor(std::shared_ptr<Actor>);
		void update();
	private:
		//DEBUG
		void console_print(Playing_Field playing_field);
	};
}