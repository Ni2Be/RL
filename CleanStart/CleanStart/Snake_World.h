#pragma once
#include "Utility.h"
#include "Types.h"
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
		Snake_World(){}



		//apple represents the eatable item
		struct Apple
		{
			bool is_eaten = true;
			Pos_int position;
			//changes the position of the apple to a random pos in the given area
			void respawn(Area_int area);
		};

		//playingfield defines which fields are walls
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

		//Sets up a standard playingfield and sporns the apple the first time 
		Snake_World(int fields);
		//copy
		Snake_World(const Snake_World&) = default;

		enum Events
		{
			ATE, CRASHED, NO_EVENT
		};

		/*
		checks if an event happened. 
		atm: It's possible that two events like
		two snakes eat the apple happen. this function just collects all events
		and does not check if they contradict each other.
		That's done in handle_events(std::vector<std::pair<Snake_Entity*, Events>>& snake_events)
		*/
		//TODO check evetns sollte prüfen ob sich events widersprechen, vielleicht neue funktion check_contradictions() 
		void check_events();
		/*
		handles events. if something was eaten the snake will be extended etc
		atm handles what event will be executed if two events contradict each other
		*/
		void handle_events(std::vector<std::pair<Snake_Entity*, Events>>& snake_events);

		void add_snake()
		{
			snakes.push_back(Snake_Entity());
			snakes[snakes.size() - 1].respown(find_spawn_area());
		};		
		
		void add_snake(Pos_int pos)
		{
			snakes.push_back(Snake_Entity());
			snakes[snakes.size() - 1].respown({pos, pos});
		};

		/*
		returns an random area which is empty.
		will try to find an 3x3 area first and after 20 trys be looking for a 2x2, 
		after 40 a 1 tile area
		*/
		Area_int find_spawn_area();
		//checks if there is a snake, a wall, or an apple in the given area.
		bool is_empty(Area_int);

		Apple apple;
		std::vector<Snake_Entity> snakes;


		bool game_over = false;
		

		Playing_Field playing_field;
	};
}
