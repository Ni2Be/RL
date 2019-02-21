#include "Pong_Game.h"
#include "Actor.h"
#include "Pong_World.h"
using namespace Ai_Arena;
using namespace Pong;


Pong_Game::Pong_Game(int paddle_length,int fields_x, int fields_y, int field_pixel)
	:
	//m_fields(fields),
	world(fields_x, fields_y, paddle_length)
{
	world.MOVE_RANGE = fields_y;
	graphics(
		std::shared_ptr<Pong_Graphics>(
			new Pong_Graphics(
				fields_x, fields_y,
				"Pong",
				field_pixel)));


	m_graphics->update_graphics(world);

}



void Pong_Game::update_world()
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

void Pong_Game::set_up()
{
	std::unique_lock<std::mutex> lock(m_execution_lock);

	//save for learning debug
	//m_old_lives = world.paddles[0].lives();

	for (auto& actor : actors())
	{
		actor.actor->activate();
	}
}


void Pong_Game::add_actor(std::shared_ptr<Actor<Pong_World>> actor)
{
	Environment::add_actor(Actor_Representation(actor, { Actions::NO_ACTION }));
	m_actor_events.push_back(Pong_World::Events::NO_EVENT);
	world.add_paddle();
}

void Pong_Game::execute_actions()
{
	//execute all actions
	for (auto actor_representation : actors())
	{

		if (world.paddles[actor_representation.actor->id()].has_lost())
			set_actor_state(actor_representation.actor, true);
		else if (actor_representation.actor->is_active())
		{
			//the actor Id should allways be the position of the paddle in this array
			Paddle* controlled_paddle = &(world.paddles[actor_representation.actor->id()]);

			if (!controlled_paddle->has_lost())
				controlled_paddle->perform_action(actor_representation.action.action);
			else
				actor_representation.actor->deactivate();

			m_unexecuted_actions--;
		}
	}

	//handle events like ate apple or crashed
	world.check_events();

}

void Pong_Game::update()
{
	update_world();

/*	
if (m_old_lives != world.Pongs[0].lives())
	{
		m_old_lives = world.Pongs[0].lives();
		std::cout << "p1 lives: " << m_old_lives << "\n\n";
	}
*/
	//only update graphics in play mode
	//if(needed)
	graphics()->update_graphics(world);
}



//I_ENVIRONMENT
std::vector<Action> Pong_Game::possible_actions(std::shared_ptr<Actor<Pong_World>> actor, Pong_World state) const
{

	std::vector<Action> all_actions = {
		Actions::U,
		Actions::D
		//Actions::L,
		//Actions::R
		//Actions::NO_ACTION 
	};

	return all_actions;
}


bool Pong_Game::is_final(std::shared_ptr<Actor<Pong_World>> actor, Pong_World state, bool is_simulation) const
{
	Paddle* controlled_paddle = &(state.paddles[actor->id()]);
	//TODO should be handled differently
	if (controlled_paddle->has_lost() && !is_simulation)
	{
		actor->deactivate();
		//notify the environment that all actors could be finished
		m_environment_condition.notify_all();
	}
	return controlled_paddle->has_lost();
};


Reward Pong_Game::reward(std::shared_ptr<Actor<Pong_World>> actor, Pong_World state) const
{
	Paddle* controlled_paddle = &(state.paddles[actor->id()]);

	Reward reward = 0.0f;

	if (controlled_paddle->score() < controlled_paddle->last_score())
		reward = 0.0f;
	else if (controlled_paddle->score() > controlled_paddle->last_score())
		reward = 1.0f;
	else
		reward = 0.5f;

	//return reward;
	return controlled_paddle->score();
}


Pong_World Pong_Game::actual_state(std::shared_ptr<Actor<Pong_World>> actor) const
{
	Pong_World current_world = world;
	return current_world;
}




Perception Pong_Game::get_perception(std::shared_ptr<Actor<Pong_World>> actor, Sensor sensor) const
{
	Pong_World actual_world = world;

	return get_perception(actor, sensor, actual_world);
}

Perception Pong_Game::get_perception(std::shared_ptr<Actor<Pong_World>> actor, Sensor sensor, Pong_World state) const
{
	Perception perception;
	if (sensor == Sensor::SEE_THE_WHOLE_STATE)
	{
		perception = convert_to_SEE_THE_WHOLE_STATE(actors()[actor->id()], state);
	}
	return perception;
}



void Pong_Game::apply_action(std::shared_ptr<Actor<Pong_World>> actor, Action action)
{
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
//converts the state to the following state but acts as if the other paddles can't move
std::vector<Pong_World> Pong_Game::assume_action(std::shared_ptr<Actor<Pong_World>> actor, Pong_World state, Action action) const
{
	Paddle* controlled_paddle = &(state.paddles[actor->id()]);

	controlled_paddle->perform_action(action.action);

	////handle events like ate apple or crashed
	state.check_events();

	return { state };
};

//I_Environment helper

const Perception Pong_Game::convert_to_SEE_THE_WHOLE_STATE(Actor_Representation perceiving_actor, const Pong_World& world) const
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


	//create temp 2d vector
	std::vector<std::vector<std::pair<int, int>>>
		world_state(world.playing_field[0].size(),
			std::vector<std::pair<int, int>>
			(world.playing_field.size(), { 0, 0 })
		);

	//fill in the walls
	for (int x = 0; x < world.playing_field.size(); x++) {
		for (int y = 0; y < world.playing_field[0].size(); y++) {
			if (world.playing_field[x][y] == Pong_World::Playing_Field::WALL)
			{
				world_state[x][y].first = C_WALL;
			}
		}
	}

	//fill in the ball
	world_state[world.ball.position.x][world.ball.position.y].first = C_BALL;

	//fill in the paddles
	int paddle_counter = 0;
	for (auto paddle : world.paddles)
	{
		for (auto paddle_segment : paddle.body())
		{
			world_state[paddle_segment.position().x][paddle_segment.position().y].first = C_P_ACTOR_BODY + paddle_counter * 2;

			switch (paddle_segment.direction())
			{
			case Pong::Actions::U:
				world_state[paddle_segment.position().x][paddle_segment.position().y].second = C_UP;
				break;
			case Pong::Actions::D:
				world_state[paddle_segment.position().x][paddle_segment.position().y].second = C_DOWN;
				break;
			case Pong::Actions::NO_ACTION:
				break;
			default:
				std::cerr << "convert_to_state()\ninvalid action: " << paddle_segment.direction() << "\n";
				exit(-1);
			}

		}
		paddle_counter++;
	}

	//convert to perception
	std::vector<float> perception;

	for (int collumn = 0; collumn < world_state.size(); collumn++)
	{
		for (int row = 0; row < world_state[0].size(); row++)
		{
			perception.push_back(world_state[row][collumn].first);
			perception.push_back(world_state[row][collumn].second);
		}
	}

	//pushback events
	//TODO EVENTS
	if (m_actor_events[perceiving_actor.actor->id()] == Pong_World::Events::PADDLE_BOUNCE)
		perception.push_back(C_IS_BOUNCED);
	else
		perception.push_back(0.0f);
	if (m_actor_events[perceiving_actor.actor->id()] == Pong_World::Events::GOAL_HIT)
		perception.push_back(C_HAS_SCORED);
	else
		perception.push_back(0.0f);

	////////////////////////////////

	//TODO normalize
	//for (auto& i : state)
	//	i /= static_cast<float>(C_O_ACTOR_HEAD + snake_counter + 2);

	return perception;
}

