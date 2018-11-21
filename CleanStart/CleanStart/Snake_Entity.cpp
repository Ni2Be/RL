#include "Snake_Entity.h"
#include "Utility.h"

#include <iostream>

using namespace Ai_Arena;



Snake_Entity::Snake_Entity()
{
}


void Snake_Entity::perform_action(Action action)
{
	//dont change direction if the snake whould crash in to its own body

	if ((body().size() > 1 && body()[1].position().y < body()[0].position().y && action.action == Actions::U)
		|| (body().size() > 1 && body()[1].position().x > body()[0].position().x && action.action == Actions::R)
		|| (body().size() > 1 && body()[1].position().y > body()[0].position().y && action.action == Actions::D)
		|| (body().size() > 1 && body()[1].position().x < body()[0].position().x) && action.action == Actions::L)
		action = body()[0].direction();

	//save tail end position / needed if the snake eats an
	//apple and is extended
	m_prev_tail_end = body().back().position();

	//move all tail parts
	for (int i = body().size() - 1; i > 0; i--)
		body()[i].position() = body()[i - 1].position();

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
}

void Snake_Entity::extend()
{
	body().push_back(Snake_Segment(m_prev_tail_end));
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

Pos_int Snake_Entity::position()
{
	return body().front().position();
}