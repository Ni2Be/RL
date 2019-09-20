#include "Snake_Game.h"
#include "Actor.h"
#include "Snake_World.h"
using namespace Ai_Arena;
using namespace Snake;
#include <fstream>

Snake_Game::Snake_Game(int fields, int field_pixel, int max_Score_Limit)
	:
	m_fields_width_count(fields),
	world(fields, max_Score_Limit)
{
	graphics(
		std::shared_ptr<Snake_Graphics>(
			new Snake_Graphics(
				fields,
				"Snake",
				field_pixel)));

	this->name = "Snake";


	m_graphics->update_graphics(world);

	//set_environment_state(convert_to_state(***));
}

void Snake_Game::update_world()
{
	if (active_actors() == 0)
	{
		m_is_running = false;
		return;
	}

	std::unique_lock<std::mutex> lock(m_execution_lock);
	//wait for all actors to place an action
	m_environment_condition.wait(lock, [this]()
	{
		return (m_unexecuted_actions == active_actors());
	});

	//emty events
	for (int i = 0; i < m_actor_events.size(); i++)
	{
		m_actor_last_events[i] = m_actor_events[i];
		m_actor_events[i] = Snake_World::Events::NO_EVENT;
	}

	//Do everything that changes the state
	execute_actions();
	//wait untill all actors sleep
	for (auto& actor : actors())
		while (!actor.actor->is_sleeping());

	//wakeup the active actors
	for (auto& actor : actors())
	{
		if (actor.actor->is_active())
			actor.actor->wake_up();
	}
	lock.unlock();
}
void Snake_Game::set_up()
{
	std::unique_lock<std::mutex> lock(m_execution_lock);

	//save for learning debug
	m_old_lifes = world.snakes[0].lifes();

	for (auto& actor : actors())
	{
		actor.actor->activate();

		actor.actor->m_sensor = Sensor::SEE_THE_WHOLE_STATE;
	}
}



void Snake_Game::add_actor(std::shared_ptr<Actor<Snake_World>> actor)
{
	Environment::add_actor(Actor_Representation(actor, { Actions::NO_ACTION }));
	m_actor_events.push_back(Snake_World::Events::NO_EVENT);
	m_actor_last_events.push_back(Snake_World::Events::NO_EVENT);
	world.add_snake();
	world.snakes.back().agent_id = actor->id();
	action_counter.push_back(0);
	action_counter.shrink_to_fit();
}

void Snake_Game::execute_actions()
{
	//execute all actions
	for (auto actor_representation : actors())
	{

		if (world.snakes[actor_representation.actor->id()].has_lost())
			set_actor_state(actor_representation.actor, true);
		else if (actor_representation.actor->is_active())
		{
			//the actor Id should allways be the position of the snake in this array
			Snake_Entity* controlled_snake = &(world.snakes[actor_representation.actor->id()]);

			action_counter[actor_representation.actor->id()]++;

			if (!controlled_snake->has_lost())
			{
				controlled_snake->perform_action(actor_representation.action.action);
			}
			else
				actor_representation.actor->deactivate();

			m_unexecuted_actions--;
		}
	}

	//handle events like ate apple or crashed
	world.check_events(m_actor_events);

}

void Snake_Game::update()
{
	update_world();

	if (m_old_lifes != world.snakes[0].lifes())
	{
		std::ofstream ofs;
		ofs.open("snakelog.txt", std::ios::app);
		if (!ofs)
		{
			std::cerr << "could not open snakelog.txt\n";
			exit(-1);
		}
		m_old_lifes = world.snakes[0].lifes();
		ofs << "lifes: " << m_old_lifes << "\nscore: " << world.snakes[0].last_score() - m_old_lifes << "\naction count: " <<  action_counter[0] << "\n\n";

		action_counter[0] = 0;
		ofs.close();
	}

	if (world.snakes[0].lifes() % 100 == 0)
		std::cout << world.snakes[0].lifes() << " games left\n";

	//only update graphics in play mode
	//if(!m_trainings_mode)
		graphics()->update_graphics(world);
}



//I_ENVIRONMENT
std::vector<Action> Snake_Game::possible_actions(std::shared_ptr<Actor<Snake_World>> actor, Snake_World state) const
{
	//TODO richtige richtungen seden
	std::vector<Action> all_actions = {
		Actions::U,
		Actions::D,
		Actions::L,
		Actions::R,
		//	Actions::NO_ACTION 
	};



	return all_actions;
}


bool Snake_Game::is_final(std::shared_ptr<Actor<Snake_World>> actor, Snake_World state, bool is_simulation) const
{
	Snake_Entity* controlled_snake = &(state.snakes[actor->id()]);
	//TODO should be handled differently
	if (!is_simulation && controlled_snake->has_lost())
	{
		actor->deactivate();
		//notify the environment that all actors could be finished
		m_environment_condition.notify_all();

	}
	return controlled_snake->has_lost();
};


Reward Snake_Game::reward(std::shared_ptr<Actor<Snake_World>> actor, Snake_World state) const
{
	Snake_Entity* controlled_snake = &(state.snakes[actor->id()]);

	Reward reward = 0.0f;

	Snake_World::Events snake_event = state.m_actor_events[controlled_snake->agent_id];

	if (snake_event == Snake_World::Events::CRASHED)
		reward = -1.0f;
	else if (snake_event == Snake_World::Events::ATE)
		reward = 1.0f;
	/*if (controlled_snake->score() < controlled_snake->last_score())
		reward = -1.0f;
	else if (controlled_snake->score() > controlled_snake->last_score())
		reward = 1.0f;
	else
		reward = 0.0f;*/

	return reward;
	//return controlled_snake->score();
}


Snake_World Snake_Game::current_state(std::shared_ptr<Actor<Snake_World>> actor) const
{
	Snake_World current_world = world;
	return current_world;
}




Perception Snake_Game::get_perception(std::shared_ptr<Actor<Snake_World>> actor, Sensor sensor) const
{
	Snake_World current_world = world;

	return get_perception(actor, sensor, current_world);
}

Perception Snake_Game::get_perception(std::shared_ptr<Actor<Snake_World>> actor, Sensor sensor, Snake_World state) const
{
	Perception perception;
	if (sensor == Sensor::SEE_THE_WHOLE_STATE)
	{
		perception = convert_to_SEE_THE_WHOLE_STATE(actors()[actor->id()], state);
	}
	return perception;
}



void Snake_Game::apply_action(std::shared_ptr<Actor<Snake_World>> actor, Action action)
{
	//std::cout << "\napply_action\n";
	//assign action to actor
	exchange_action(actor, action);

	std::unique_lock<std::mutex> lock(m_execution_lock);
	m_unexecuted_actions++;

	//notify the environment that all actors could be finished
	m_environment_condition.notify_all();

	lock.unlock();

	actor->sleep();
}

//TODO
//converts the state to the following state but acts as if the other snakes can't move
std::vector<Snake_World> Snake_Game::assume_action(std::shared_ptr<Actor<Snake_World>> actor, Snake_World state, Action action) const
{
	Snake_Entity* controlled_snake = &(state.snakes[actor->id()]);

	controlled_snake->perform_action(action.action);
	std::vector<Snake_World::Events> dummy_snake_world_events(state.snakes.size());

	////handle events like ate apple or crashed
	state.check_events(dummy_snake_world_events);
	
	return { state };
};

//I_Environment helper
const Perception Snake_Game::convert_to_SEE_THE_WHOLE_STATE(Actor_Representation perceiving_actor, const Snake_World& world) const
{
	//TODO find good state representation

	/*
	converts the world to a state, with two numbers for each field:
	Empty fields will be represented as [0, 0].
	Walls as [1, 0].
	The apple as [2, 0]
	The percieving actor will be represented as 3s, the own head as 4,
	and the direction of the segment as a u=0, d=1, l=2, r=3, if there is no segment the field is 0
	Other actors as 5s + i, heads of them as 6s + i. with i = 0 incremeting by 1 for each actor
	the last two field represent is_crashed and has_eaten the are 1 if true
	they won't be normalized

	possible output before normalization
	for 3 actors and a playingfield size of 10*10 and the p_actor has_eaten:
	[1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [3, 0] [4, 3] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [3, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [8, 3] [0, 0] [2, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [5, 3] [5, 3] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [5, 1] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [5, 1] [6, 3] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
	[1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0]

	TODO at the moment all first values and after all secound values saved

	W W W W W W W W W W W
	W . . . | o . . . . W
	W . . . | . . . . A W
	W . _ _ . . o . . . W
	W . . | . . . . . . W
	W . . | o . . . . . W
	W . . . . . . . . . W
	W . . . . . . . . . W
	W . . . . . . . . . W
	W W W W W W W W W W W
	0 1

	const int C_WALL = 1;
	const int C_APPLE = 2;

	const int C_P_ACTOR_BODY = 3;
	const int C_P_ACTOR_HEAD = 4;
	const int C_O_ACTOR_BODY = 5;
	const int C_O_ACTOR_HEAD = 6;

	const int C_IS_CRASHED = 1;
	const int C_HAS_EATEN = 1;

	const int C_UP = 0;
	const int C_DOWN = 1;
	const int C_LEFT = 2;
	const int C_RIGHT = 3;
	*/
	std::vector<double> td_perception;

	const Snake_Entity* controlled_snake = &(world.snakes[perceiving_actor.actor->id()]);

	if (controlled_snake->has_lost())
		return { 0,0,0,0, 0,0,0,0 };


	int snakePosX = controlled_snake->body()[0].position().x;
	int snakePosY = controlled_snake->body()[0].position().y;
	Snake::Actions snakeDirection = controlled_snake->body()[0].direction();
	
	int distanceLeft = 1.0, distanceFront = 1.0, distanceRight = 1.0;
	int startX, startY;

	//hab auch mal m_actor_last_events rein gemacht falls das hilft 
	Snake_World worl_cpy = world;
	Snake_World::Events snake_event = worl_cpy.m_actor_events[controlled_snake->agent_id];

	if (snake_event == Snake_World::Events::CRASHED)
	{
		td_perception.push_back(1.0);
		td_perception.push_back(0.0);
	}
	else if (snake_event == Snake_World::Events::ATE)
	{
		td_perception.push_back(0.0);
		td_perception.push_back(1.0);
	}
	else
	{
		td_perception.push_back(0.0);
		td_perception.push_back(0.0);
	}
		if (controlled_snake->body().size() > 0)
		{
			switch (snakeDirection)
			{
			case Snake::Actions::U:
				td_perception.push_back((double)controlled_snake->distanceToBodyLeft());
				td_perception.push_back((double)controlled_snake->distanceToBodyUp());
				td_perception.push_back((double)controlled_snake->distanceToBodyRight());
				break;
			case Snake::Actions::D:
				td_perception.push_back((double)controlled_snake->distanceToBodyRight());
				td_perception.push_back((double)controlled_snake->distanceToBodyDown());
				td_perception.push_back((double)controlled_snake->distanceToBodyLeft());
				break;
			case Snake::Actions::L:
				td_perception.push_back((double)controlled_snake->distanceToBodyDown());
				td_perception.push_back((double)controlled_snake->distanceToBodyLeft());
				td_perception.push_back((double)controlled_snake->distanceToBodyUp());
				break;
			case Snake::Actions::R:
				td_perception.push_back((double)controlled_snake->distanceToBodyUp());
				td_perception.push_back((double)controlled_snake->distanceToBodyRight());
				td_perception.push_back((double)controlled_snake->distanceToBodyDown());
				break;
			case Snake::Actions::NO_ACTION:
				break;
			default:
				std::cerr << "convert_to_state()\ninvalid action: " << snakeDirection << "\n";
				if (td_perception[2] < 0
					|| td_perception[3] < 0
					|| td_perception[4] < 0
					|| td_perception[2] > world.playing_field.size()
					|| td_perception[3] > world.playing_field.size()
					|| td_perception[4] > world.playing_field.size())
				{
					td_perception[2] = 0;
					td_perception[3] = 0;
					td_perception[4] = 0;
				}
				else
				{
					td_perception[2] /= 100.0;
					td_perception[3] /= 100.0;
					td_perception[4] /= 100.0;

				}
				//exit(-1);
			}
			//std::cout << "\nl: " << td_perception[2] << ", f: " << td_perception[3] << ", r: " << td_perception[4];
		}
		else
		{
			td_perception.push_back(0.0);
			td_perception.push_back(0.0);
			td_perception.push_back(0.0);
		}
		int x, y;

		if (controlled_snake->body().size() > 0)
		{
			//x = std::abs(world.apple.position.x - controlled_snake->body()[0].position().x);
			//y = std::abs(world.apple.position.x - controlled_snake->body()[0].position().y);

			if (controlled_snake->body()[0].position().x < world.apple.position.x)
				x = world.apple.position.x - controlled_snake->body()[0].position().x;
			else if (controlled_snake->body()[0].position().x > world.apple.position.x)
				x = controlled_snake->body()[0].position().x - world.apple.position.x;
			else
				x = 0;

			if (controlled_snake->body()[0].position().y < world.apple.position.y)
				y = world.apple.position.x - controlled_snake->body()[0].position().y;
			else if (controlled_snake->body()[0].position().y > world.apple.position.x)
				y = controlled_snake->body()[0].position().y - world.apple.position.x;
			else
				y = 0;
			td_perception.push_back(x);
			td_perception.push_back(y);

			td_perception.push_back((world.playing_field.size() * 2 - (x + y)) / world.playing_field.size() * 2);
			td_perception.push_back(x - controlled_snake->body()[0].position().x + y - controlled_snake->body()[0].position().y);
		}
		else
		{
			td_perception.push_back(0.0);
			td_perception.push_back(0.0);
			td_perception.push_back(0.0);
			td_perception.push_back(0.0);
		}
	
	return td_perception;
}


void Snake_Game::debug(std::shared_ptr<Actor<Snake_World>> actor)
{
	return;


}

std::vector<double> Snake_Game::get_stats()
{
	std::vector<double> null;
	return null;
}
