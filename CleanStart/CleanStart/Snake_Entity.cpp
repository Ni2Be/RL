#include "Snake_Entity.h"
#include "Utility.h"

#include <iostream>

using namespace Ai_Arena;



Snake_Entity::Snake_Entity()
{
}


void Snake_Entity::perform_action(Action action)
{
	if (has_lost())
		return;
	//dont change direction if the snake whould crash in to its own body

	if ((body().size() > 1 && body()[1].position().y < body()[0].position().y && action.action == Actions::U)
		|| (body().size() > 1 && body()[1].position().x > body()[0].position().x && action.action == Actions::R)
		|| (body().size() > 1 && body()[1].position().y > body()[0].position().y && action.action == Actions::D)
		|| (body().size() > 1 && body()[1].position().x < body()[0].position().x) && action.action == Actions::L)
		action = body()[0].direction();

	//save tail end position / needed if the snake eats an
	//apple and is extended
	//ToDo: body().back(), oder body().back().position()?
	m_prev_tail_end = body().back().position();
	
	//std::cout.flush();// Flush the output stream
	//system("cls"); // Clear the console with the "system" function

	//move all tail parts
	//std::cout << body().size() - 1 << std::endl;
	for (int i = body().size() - 1; i > 0; i--)
	{
			
			body()[i].position() = body()[i - 1].position();
			body()[i].direction() = body()[i - 1].direction();

			if (i == body().size() - 1 && body().size() > 2)
				body()[i].direction() = body()[i - 2].direction();

	}

	//move head
	switch (action.action)
	{
	case Actions::U:
		body()[0].position().y--;
		body()[0].direction() = Actions::U;
		break;
	case Actions::D:
		body()[0].position().y++;
		body()[0].direction() = Actions::D;
		break;
	case Actions::R:
		body()[0].position().x++;
		body()[0].direction() = Actions::R;
		break;
	case Actions::L:
		body()[0].position().x--;
		body()[0].direction() = Actions::L;
		break;
	case Actions::NO_ACTION:
		break;
	default:
		std::cerr << std::endl << "invalid action" << std::endl;
		exit(-1);
	}

	//check for direction switch
	for (int i = body().size() - 1; i >= 0; i--)
	{
		if (body()[i].is_head() == false && body()[i].is_tail() == false)
		{
			body()[i].turn() = NO_TURN;


			if (body()[i].direction() == Actions::U && body()[i - 1].direction() == Actions::L)
				body()[i].turn() = UL;
			if (body()[i].direction() == Actions::U && body()[i - 1].direction() == Actions::R)
				body()[i].turn() = UR;
			if (body()[i].direction() == Actions::D && body()[i - 1].direction() == Actions::L)
				body()[i].turn() = DL;
			if (body()[i].direction() == Actions::D && body()[i - 1].direction() == Actions::R)
				body()[i].turn() = DR;
			if (body()[i].direction() == Actions::L && body()[i - 1].direction() == Actions::U)
				body()[i].turn() = LU;
			if (body()[i].direction() == Actions::R && body()[i - 1].direction() == Actions::U)
				body()[i].turn() = RU;
			if (body()[i].direction() == Actions::L && body()[i - 1].direction() == Actions::D)
				body()[i].turn() = LD;
			if (body()[i].direction() == Actions::R && body()[i - 1].direction() == Actions::D)
				body()[i].turn() = RD;
	
		}
		

	}
}

void Snake_Entity::extend()
{
	body().push_back(Snake_Segment(m_prev_tail_end));
	if (m_body.size() > 1)
	{
		m_body[m_body.size() - 2].m_is_tail = false;
		m_body[m_body.size() - 1].m_is_tail = true;
	}
	body().back().is_head() = false;
	
}

void Snake_Entity::respown(Area_int area)
{
	//delete body
	body().clear();
	//new head
	body().push_back(
		Snake_Segment(
					Utility::random_int_ts(area.upper_left.x, area.lower_right.x),
					Utility::random_int_ts(area.upper_left.y, area.lower_right.y)));
	body()[0].is_head() = true;

}

void Snake_Entity::game_over()
{
	body().clear();
	has_lost() = true;
}

Pos_int Snake_Entity::position()
{
	return body().front().position();
}

