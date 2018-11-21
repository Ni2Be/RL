#pragma once
#include "Utility.h"
#include "Snake_Entity.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace Ai_Arena
{
	class Snake_World
	{
	public:

		class Apple
		{
		public:
			bool is_eaten = true;
			Pos_int position;
			void respawn(Area_int area);
		};

		//TODO is playingfield needed?
		class Playing_Field : public std::vector<std::vector<int>>
		{
		public:
			enum Field_Types
			{
				WALL,
				EMPTY
			};

			Playing_Field() {}			
			Playing_Field(int x, std::vector<int> y)
				:
				std::vector<std::vector<int>>(x, y)
			{}
		};

		//TODO
		Snake_World(int fields)
			:
			playing_field(fields, std::vector<int>(fields, Playing_Field::EMPTY))
		{
			std::fill(std::begin(playing_field[0]), std::end(playing_field[0]), Playing_Field::WALL);
			std::fill(std::begin(playing_field[fields - 1]), std::end(playing_field[fields - 1]), Playing_Field::WALL);
			for (auto& field_vec : playing_field)
			{
				field_vec[0] = Playing_Field::WALL;
				field_vec[fields - 1] = Playing_Field::WALL;
			}

			apple.respawn({ {fields / 2, fields / 2},{fields / 2, fields / 2} });
		}

		enum Events
		{
			ATE, CRASHED, NO_EVENT
		};

		void check_events();
		void handle_events(std::vector<std::pair<Snake_Entity*, Events>>& snake_events);

		void add_snake()
		{
			snakes.push_back(Snake_Entity());
			snakes[snakes.size() - 1].respown(find_spawn_area());
		};

		Area_int find_spawn_area();
		bool is_empty(Area_int);

		Apple apple;
		std::vector<Snake_Entity> snakes;

		Playing_Field playing_field;
	};
}