#pragma once
#include "Utility.h"
#include "Paddle.h"
#include "Types.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace Ai_Arena
{
	class Pong_World
	{
	public:
		static int MOVE_RANGE;
		Pong_World() {}
		class Ball
		{
		public:
			bool is_shot = true;
			int hit_by = Sides::NONE;
			Pos_int position;
			float step_x = 0, step_y = 0, accel_x = 1, accel_y = 1;
			void reset(Pos_int pos);
			void travel();
			void flip_trajectory_x();
			void flip_trajectory_y();
		};
		class Playing_Field : public std::vector<std::vector<int>>
		{
		public:
			enum Field_Types
			{
				WALL, EMPTY, GOAL
			};

			Playing_Field() {}
			Playing_Field(int x, std::vector<int> y) 
				: 
				std::vector<std::vector<int>>(x,y)
			{}
		};

		Pong_World(int fields_x, int fields_y, int paddle_length)
			:
			playing_field(fields_x, std::vector<int>(fields_y, Playing_Field::EMPTY)),
			m_fields_x(fields_x),
			m_fields_y(fields_y),
			m_paddle_length(paddle_length)
			
		{
			MOVE_RANGE = fields_y;
			//Vertical
			std::fill(std::begin(playing_field[0]), std::end(playing_field[0]), Playing_Field::GOAL);
			std::fill(std::begin(playing_field[fields_x - 1]), std::end(playing_field[fields_x - 1]), Playing_Field::GOAL);

			//Horizontal
			for (auto& field_vec : playing_field)
			{

				field_vec[0] = Playing_Field::WALL;
				field_vec[fields_y - 1] = Playing_Field::WALL;

			}

			ball.reset({fields_x / 2, fields_y / 2});
		}

		enum Events
		{
			NO_EVENT, WALL_BOUNCE, PADDLE_BOUNCE, GOAL_HIT, WAIT_FOR_BALL
		};

		void check_events();
		void handle_events(std::vector<std::pair<Paddle*, Events>>& pong_events);

		void add_paddle()
		{
			
			paddles.push_back(Paddle());
			if (paddles.size() == 1)
			{
				paddles[0].set_values({ 2,(int)m_fields_y / 2 , 0 }, m_paddle_length, Sides::L);
			}
			else if (paddles.size() == 2)
			{
				paddles[1].set_values({m_fields_x - 3,(int)m_fields_y / 2 , 0 }, m_paddle_length, Sides::R);
			}
			else std::cout << "Reached Paddles Maximum" << std::endl;
		};

		std::vector<Paddle> paddles;
		Ball ball;

		Playing_Field playing_field;
		int m_fields_x, m_fields_y;
		int m_paddle_length;
	};
}