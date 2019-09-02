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
		actor.actor->m_sensor = Sensor::OWN_POS_AND_BALL;
	}
	//Lineare Funktion aufstellen (normalisierung zwischen 0 und 1) 
	
	x1_w = 0;
	x2_w = world.m_fields_x;
	y1_w = 0;
	y2_w = 1;
	m_w = (y2_w - y1_w) / (x2_w - x1_w);
	b_w = y2_w - (m_w*x2_w);
	
	x1_h = 0;
	x2_h = world.m_fields_y;
	y1_h = 0;
	y2_h = 1;
	m_h = (y2_h - y1_h) / (x2_h - x1_h);
	b_h = y2_h - (m_h*x2_h);



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

	for (auto& actor : actors())
	{
		Paddle* paddle = &(world.paddles[actor.actor->id()]);
		if (paddle->score() > C_MAX_SCORE)
			this->m_is_running = false;
	}

	update_world();

/*	
if (m_old_lives != world.Pongs[0].lives())
	{
		m_old_lives = world.Pongs[0].lives();
		std::cout << "p1 lives: " << m_old_lives << "\n\n";
	}
*/
	//only update graphics in play mode
	if(!m_trainings_mode)
		graphics()->update_graphics(world);
}



//I_ENVIRONMENT
std::vector<Action> Pong_Game::possible_actions(std::shared_ptr<Actor<Pong_World>> actor, Pong_World state) const
{

	std::vector<Action> all_actions = {
		Actions::U,
		Actions::D//,
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
	Reward reward = 0.0;
	const Pong_World world = state;

	int chunkSize = 5;
	int actor_front_y = controlled_paddle->body().front().position().y;
	int paddle_size = (int) controlled_paddle->body().size();
	int actor_center_y = controlled_paddle->body()[paddle_size / 2 - 1].position().y;
	int actor_back_y = controlled_paddle->body().back().position().y;
	int ball_y = state.ball.position.y;
	float chunk = state.m_fields_y / chunkSize;

	
	
	for (int i = 0; chunkSize > i; i++)
	{
		//std::cout << "chunk " << i << ": " << chunk * i<< std::endl;

		if (chunk * i < actor_center_y && actor_center_y < chunk * (i + 1))
		{
		
			if (chunk * i < ball_y && ball_y < chunk * (i + 1))
				reward += 0.2;
		}
	}
	

	
	//Ball auf Paddle Höhe
	if (controlled_paddle->body().front().position().y <= state.ball.position.y && state.ball.position.y <= controlled_paddle->body().back().position().y)
		reward += 0.7;
		

	
	//Paddle schlägt Ball
	for (int i = 0; controlled_paddle->body().size() > i; i++)
	{
		if (controlled_paddle->body()[i].position().x + 1 == state.ball.position.x && controlled_paddle->body()[i].position().y == state.ball.position.y && controlled_paddle->get_side() == Sides::L ||
			controlled_paddle->body()[i].position().x - 1 == state.ball.position.x && controlled_paddle->body()[i].position().y == state.ball.position.y && controlled_paddle->get_side() == Sides::R
			)
		{
			reward += 0.8;
			//std::cout << "BOUNCE! "  << std::endl;
		}
	}

	if (reward == 0.0)
		reward = 0.0001;


	return reward;
	//return controlled_paddle->score();
}


Pong_World Pong_Game::current_state(std::shared_ptr<Actor<Pong_World>> actor) const
{
	Pong_World current_world = world;
	return current_world;
}




Perception Pong_Game::get_perception(std::shared_ptr<Actor<Pong_World>> actor, Sensor sensor) const
{
	Pong_World current_world = world;
	return get_perception(actor, sensor, current_world);

	//Perception perception;
	//return perception;
}



Perception Pong_Game::get_perception(std::shared_ptr<Actor<Pong_World>> actor, Sensor sensor, Pong_World state) const
{
	Perception perception;
	//if (sensor == Sensor::SEE_THE_WHOLE_STATE)
	//{
	//	perception = convert_to_SEE_THE_WHOLE_STATE(actors()[actor->id()], state);
	//}
	//if (sensor == Sensor::OWN_POS_AND_BALL)
	//{
		perception = own_position_and_ball(actors()[actor->id()], state);
	//}

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
	std::vector<double> perception;

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


const Perception Pong_Game::own_position_and_ball(Actor_Representation perceiving_actor, const Pong_World& world) const
{
	Perception perception;
	Pong_World state = world;
	int chunkSize = 5;
	int actor_front_y = state.paddles[perceiving_actor.actor->id()].body().front().position().y;
	int actor_center_y = state.paddles[perceiving_actor.actor->id()].body()[(int) state.paddles[perceiving_actor.actor->id()].body().size() / 2 - 1].position().y;
	int actor_back_y = state.paddles[perceiving_actor.actor->id()].body().back().position().y;
	int actor_x = state.paddles[perceiving_actor.actor->id()].body().front().position().x;
	int ball_x = state.ball.position.x;
	int ball_y = state.ball.position.y;
	float chunk = state.m_fields_y / chunkSize;


	//Perception falls Ball vor einem!
	/*if (actor_front_y <= ball_y && ball_y <= actor_back_y)
		perception.push_back(1.0);
	else
		perception.push_back(0.0);
		*/

	//Perception für Chunks

	/*
	for (int i = 0; chunkSize > i; i++)
	{
		if (chunk * i <= actor_center_y && actor_center_y < chunk * (i + 1))
		{
			perception.push_back(normalize_height(chunk * (i)));
			//perception.push_back(i);
			if (chunk * i <= ball_y && ball_y < chunk * (i + 1))
			{
				perception.push_back(1.0);
			}
			else
			{
				perception.push_back(0.0);

			}
		}
	}
	*/

	//Nicht normalisiert
	//perception.push_back(actor_front_y);
	//perception.push_back(actor_back_y);
	//perception.push_back(ball_y);

	perception.push_back(normalize_height(actor_front_y));
	//perception.push_back(normalize_width(actor_x));
	//perception.push_back(normalize_height(actor_back_y));
	//perception.push_back(normalize_width(ball_x));
	perception.push_back(normalize_height(ball_y));







	return perception;
}

void Pong_Game::debug(std::shared_ptr<Actor<Pong_World>> actor)
{
	
	if (world.paddles[0].get_length() == world.paddles[1].get_length())
	{
		world.paddles[1].set_values({ world.m_fields_x - 3,1 , 0 }, world.m_fields_y -2, Sides::R);
	}
	else
	{
		world.paddles[1].set_values({ world.m_fields_x - 3,(int)world.m_fields_y / 2 , 0 }, world.m_paddle_length, Sides::R);
	}
	


}

std::vector<double> Pong_Game::get_stats()
{
	std::vector<double> stats;
	stats.push_back(world.paddles[0].hits());
	stats.push_back(world.paddles[1].score());
	world.paddles[0].hits() = 0;
	world.paddles[1].score() = 0;
	return stats;

}
double Pong_Game::normalize_width(double input) const
{
	return m_w * input + b_w;
}
double Pong_Game::normalize_height(double input) const
{
	return m_h * input + b_h;
}