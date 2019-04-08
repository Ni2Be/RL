#include "Pong_World.h"
#include <iostream>
#include <ctime> //Für Random Zahl

using namespace Ai_Arena;
int Pong_World::MOVE_RANGE;
void Pong_World::Ball::reset(Pos_int pos)
{
	position = pos;
	is_shot = false;
	hit_by = Sides::R;
	step_x = 0;
	step_y = 0;
	accel_x = 1;
	accel_y = 1;

	srand((int)time(0));

	if ((rand() % 100) < 50)
		accel_x *= 1;
	else accel_x *= -1;
	if ((rand() % 100) < 50)
		accel_y *= 1;
	else accel_y *= -1;

}
void Pong_World::Ball::travel()
{
	
	step_x += accel_x;
	step_y += accel_y;
	if (step_x >= 1)
	{
		step_x = 0;
		position.x = position.x + 1;
	}

	if (step_x <= -1)
	{
		step_x = 0;
		position.x = position.x - 1;
		
	}
	if (step_y >= 1)
	{
		step_y = 0;
		position.y = position.y + 1;
	}

	if (step_y <= -1)
	{
		step_y = 0;
		position.y = position.y - 1;
	}
}

void Pong_World::Ball::flip_trajectory_x()
{

	accel_x = accel_x * -1;
}
void Pong_World::Ball::flip_trajectory_y()
{

	accel_y = accel_y * -1;

}
void Pong_World::check_events()
{
	std::vector<std::pair<Paddle*, Events>> pong_events;
	for (auto& paddle : paddles)
	{
		//Ball landet im Ziel
		if (playing_field[ball.position.x][ball.position.y] == Playing_Field::GOAL && ball.hit_by == paddle.get_side())
		{

			pong_events.push_back({ &paddle, Events::GOAL_HIT });

		}
		//Ball hits Paddle
		for (int i = 0; paddle.body().size()> i; i++)
		{
			if (paddle.body()[i].position().x + 1 == ball.position.x && paddle.body()[i].position().y == ball.position.y && paddle.get_side() == Sides::L ||
				paddle.body()[i].position().x - 1 == ball.position.x && paddle.body()[i].position().y == ball.position.y && paddle.get_side() == Sides::R
				)
			{
				ball.flip_trajectory_x();
				pong_events.push_back({ &paddle, Events::PADDLE_BOUNCE });
			}
		}
		
	}

	//Wait for shoot
	if (!ball.is_shot)
		ball.is_shot = true;

	if (playing_field[ball.position.x][ball.position.y + 1] == Playing_Field::WALL || playing_field[ball.position.x][ball.position.y - 1] == Playing_Field::WALL)
		ball.flip_trajectory_y();

	if (ball.is_shot)
		ball.travel();
	handle_events(pong_events);
}

void Pong_World::handle_events(std::vector<std::pair<Paddle*, Events>>& pong_events)
{
		for (auto& pong_event_pair : pong_events)
		{
			switch (pong_event_pair.second)
			{
			case Events::WAIT_FOR_BALL:
					ball.is_shot = true;
				break;
			case Events::PADDLE_BOUNCE:
				//std::cout << "paddle hit: " << pong_event_pair.first->get_side() << std::endl;
				//std::cout << "Ball hits: " << pong_event_pair.first->hits() << std::endl;
				pong_event_pair.first->hits()++;
				ball.hit_by = pong_event_pair.first->get_side();
				break;
			case Events::GOAL_HIT:
				pong_event_pair.first->increase_Score(1);
				//std::cout.flush();// Flush the output stream
				//system("cls"); // Clear the console with the "system" function
				
				/*
				if (pong_event_pair.first->get_side() == Sides::L)
					std::cout <<  "Left Scored a Point!" << std::endl;
				else
					std::cout << "Right Scored a Point!" << std::endl;

				std::cout << "Scores \nLeft: " << paddles[Sides::L].score() << "\nRight: " << paddles[Sides::R].score() << std::endl;
				*/
				//std::cout << "Ball misses: " << paddles[Sides::R].score() << std::endl;
				
				ball.reset({ m_fields_x / 2, m_fields_y / 2 });

				break;
			case Events::NO_EVENT:
				break;
			default:
				std::cerr << std::endl << "invalid event" << std::endl;
				exit(-1);
			}
		}
}
