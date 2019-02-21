#include "Snake_World.h"
#include <iostream>

using namespace Ai_Arena;

Snake_World::Snake_World(int fields)
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


void Snake_World::Apple::respawn(Area_int area)
{
	if (area.lower_right.x == -1)
	{
		return;
	}
	position = {
			Utility::random_int_ts(area.upper_left.x, area.lower_right.x),
			Utility::random_int_ts(area.upper_left.y, area.lower_right.y) };
	is_eaten = false;
}

void Snake_World::check_events()
{
	std::vector<std::pair<Snake_Entity*, Events>> snake_events;
	for (auto& snake : snakes)
	{
		if (!snake.has_lost())
		{
			//save old score
			snake.last_score() = snake.score();

			//ate?
			if (snake.head_position() == apple.position)
			{
				apple.is_eaten = true;
				snake_events.push_back({ &snake, Events::ATE });
			}
			//crashed in other snake or itselfe?
			for (auto& other : snakes)
			{
				for (int i = 0; i < other.body().size(); i++)
				{
					//skip own head
					if (&snake == &other && i == 0)
						i++;
					if ((&snake == &other) && (snake.body().size() == 1))
						break;

					//check all body segments
					if (snake.head_position() == other.body()[i].position())
						snake_events.push_back({ &snake, Events::CRASHED });
				}
			}

			//crashed in wall?
			if (playing_field[snake.head_position().x][snake.head_position().y] == Playing_Field::WALL)
				snake_events.push_back({ &snake, Events::CRASHED });
		}
	}

	handle_events(snake_events);
}

void Snake_World::handle_events(std::vector<std::pair<Snake_Entity*, Events>>& snake_events)
{
	for (int i = 0; i < snake_events.size(); i++)
	{
		//if more than one snake is crashed it must be checked if two
		//snake heads crashed, only the smaller one crashes
		//(or both if of equal size) and the bigger eats
		if (snake_events[i].second == Events::CRASHED)
		{
			for (int n = i; n < snake_events.size(); n++)
			{
				if (snake_events[i].first->head_position() == snake_events[n].first->head_position())
				{
					if (snake_events[i].first->body().size() > snake_events[n].first->body().size())
						snake_events[i].second = Events::ATE;
					if (snake_events[n].first->body().size() > snake_events[i].first->body().size())
						snake_events[n].second = Events::ATE;
				}
			}
		}
	}

	for (auto& snake_event : snake_events)
	{
		//if a snake is crashed while eating it should not have eaten
		if (snake_event.second == Events::ATE)
		{
			for (const auto& other_snake_event : snake_events)
			{
				if (&snake_event.first == &other_snake_event.first
					&& other_snake_event.second == Events::CRASHED)
					snake_event.second = Events::NO_EVENT;
			}
		}
	}

	for (auto& snake_event_pair : snake_events)
	{
		switch (snake_event_pair.second)
		{
		case Events::ATE:
			snake_event_pair.first->extend();
			snake_event_pair.first->score()++;
			break;
		case Events::CRASHED:
			snake_event_pair.first->score() = 0;
			//only respown if lives left
			if (--snake_event_pair.first->lives() <= 0)
				snake_event_pair.first->game_over();
			else
				snake_event_pair.first->respown(find_spawn_area());
			break;
		case Events::NO_EVENT:
			break;
		default:
			std::cerr << std::endl << "invalid event" << std::endl;
			exit(-1);
		}
	}

	if (apple.is_eaten)
		apple.respawn(find_spawn_area());
}

Area_int Snake_World::find_spawn_area()
{
	int area_size = 3;
	int trys = 0;
	while (trys < 1000)
	{
		int x = Utility::random_int_ts(0, playing_field[0].size() - 1 - area_size);
		int y = Utility::random_int_ts(0, playing_field.size() - 1 - area_size);
		if (is_empty(Area_int({ x, y }, { x + area_size, y + area_size })))
			return Area_int({ x, y }, { x + area_size, y + area_size });

		if (trys++ % 20 == 0 && area_size > 0)
			area_size--;
	}
	game_over = true;
	return Area_int({ -1, -1 }, { -1, -1 });

}


bool Snake_World::is_empty(Area_int area)
{
	//is a snake segment in the area?
	for (const auto& snake : snakes)
	{
		for (const auto& segment : snake.body())
		{
			if (area.is_inside(segment.position()))
				return false;
		}
	}
	//is the apple in the area?
	if (!apple.is_eaten && area.is_inside(apple.position))
		return false;
	//is a wall in the area?
	for (int x = area.upper_left.x; x <= area.lower_right.x; x++)
	{
		for (int y = area.upper_left.y; y <= area.lower_right.y; y++)
		{
			if (playing_field[x][y] == Playing_Field::WALL)
			{
				return false;
			}
		}
	}
	return true;
}
