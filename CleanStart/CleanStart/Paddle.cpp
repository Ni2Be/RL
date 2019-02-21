#include "Paddle.h"
#include "Utility.h"
#include "Pong_World.h"
#include <iostream>

using namespace Ai_Arena;



Paddle::Paddle()
{
}


void Paddle::perform_action(Action action)
{
		if (has_lost())
			return;
		m_current_action.action = action.action;
		for (int i = body().size() - 1; i >= 0; i--)
		{
			switch (action.action)
			{
			case Actions::U:
				if (body()[0].position().y != 0 + 1)
				{
					body()[i].position().y--;
					body()[i].direction() = Actions::U;
				}

				break;
			case Actions::D:

				if (body()[body().size() - 1].position().y != Pong_World::MOVE_RANGE - 2)
				{
					//Erst die oberen Teile nach unten verschieben, da letztes Glied sonst zuerst nach vorne geht und die anderen nicht mitgehen
					body()[body().size() - 1-i].position().y++;
					body()[body().size() - 1-i].direction() = Actions::D;
				}

				break;

			case Actions::NO_ACTION:
				
				body()[i].direction() = Actions::NO_ACTION;
				break;
			default:
				std::cerr << std::endl << "invalid action" << std::endl;
				exit(-1);
			}
		}
	
}




void Paddle::reset_position()
{
	body().clear();
	for (int i = 0; m_length > i; i++)
	{
		body().push_back(Paddle_Segment(m_start_position.x, m_start_position.y + i));
		//std::cout << "Parts: " << i << std::endl;
	}
	
}

void Paddle::set_values(Pos_int pos, int paddle_length, int side)
{
	m_start_position = pos;
	m_length = paddle_length;
	m_side = side;
	reset_position();

}

Action Paddle::get_action()
{
	//std::cout << "Action: " << m_current_action.action << std::endl;
	return m_current_action;
}


Pos_int Paddle::position()
{
	
	return body().front().position();
}

void Paddle::increase_Score(int score)
{
	m_score += score;
}


void Paddle::game_over()
{
	reset_position();
	has_lost() = true;
}