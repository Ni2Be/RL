#include "Snake_Game.h"
#include "Actor.h"
#include "Snake_World.h"
using namespace Ai_Arena;
using namespace Snake;


Snake_Game::Snake_Game(int fields, int field_pixel)
	:
	m_fields_width_count(fields),
	world(fields) //world constructor places an apple and sets the walls
{
	graphics(
		std::shared_ptr<Snake_Graphics>(
			new Snake_Graphics(
				fields,
				"Snake",
				field_pixel)));


	m_graphics->update_graphics(world);

}

void Snake_Game::update_world()
{
	std::unique_lock<std::mutex> lock(m_execution_lock);


	//TODO actors should say that they sleep
	int really_active_actors = 0;
	for (auto actor : actors())
	{
		if (!actor.actor->is_sleeping())
			really_active_actors++;
	}
	if (really_active_actors == 0)
	{
		m_is_running = false;
		return;
	}
	//std::cout << "active: " << really_active_actors << ", human: " << human_actors() << "\n";
	//std::cout << "unexecuted: " << m_unexecuted_actions << "\n";
	//TODO

	//wait for all actors to place an action
	if (active_actors() - human_actors() > 0)
		m_environment_condition.wait(lock, [this]() {return m_unexecuted_actions == active_actors() - human_actors(); });

	

	//Do everything that changes the state
	execute_actions();

	////set environment state, represented by the state actor 1 whould see it
	//set_environment_state(convert_to_state(actors()[0], world));

	////DEBUG
	//console_print(environment_state());

	//wakeup the actors
	m_actors_condition.notify_all();
	lock.unlock();
}

void Snake_Game::set_up()
{
	//TODO set state the first time
	//set_environment_state(convert_to_state(***));

	for (auto& actor : actors())
		actor.actor->wake_up();
}


void Snake_Game::add_actor(std::shared_ptr<Actor<State_Type>> actor)
{
	Environment::add_actor(Actor_Representation(actor, { Actions::NO_ACTION }));
	m_actor_events.push_back(Snake_World::Events::NO_EVENT);
	world.add_snake();
}

void Snake_Game::execute_actions()
{
	//execute all actions
	for (auto actor_representation : actors())
	{

		if(world.snakes[actor_representation.actor->id()].has_lost())
			set_actor_state(actor_representation.actor, true);


		if (!actor_representation.actor->is_sleeping())
		{
			//the actor Id should allways be the position of the snake in this array
			Snake_Entity* controlled_snake = &(world.snakes[actor_representation.actor->id()]);

			if(!controlled_snake->has_lost())
				controlled_snake->perform_action(actor_representation.action.action);

			if (!actor_representation.actor->is_human())
				m_unexecuted_actions--;
		}
	}

	//save old scores
	std::vector<int> snake_old_scores;
	for (auto actor_representation : actors())
	{
		Snake_Entity* controlled_snake = &(world.snakes[actor_representation.actor->id()]);
		snake_old_scores.push_back(controlled_snake->score());
	}
	//handle events like ate apple or crashed
	world.check_events();

	//TODO should not be programmed like this
	//update actor scores and set event if score has changed
	for (auto actor_representation : actors())
	{
		Snake_Entity* controlled_snake = &(world.snakes[actor_representation.actor->id()]);
		if (snake_old_scores[actor_representation.actor->id()] < controlled_snake->score())
			m_actor_events[actor_representation.actor->id()] = Snake_World::Events::ATE;
		else if (snake_old_scores[actor_representation.actor->id()] > controlled_snake->score())
			m_actor_events[actor_representation.actor->id()] = Snake_World::Events::CRASHED;
		else
			m_actor_events[actor_representation.actor->id()] = Snake_World::Events::NO_EVENT;
	}
}

void Snake_Game::update()
{
	update_world();

	//only update graphics in play mode
	//if(needed)
	graphics()->update_graphics(world);
}



//I_ENVIRONMENT
std::vector<Action> Snake_Game::possible_actions(std::shared_ptr<Actor<State_Type>>, State_Type) const
{
	return { 
		Actions::U, 
		Actions::D, 
		Actions::L, 
		Actions::R, 
		Actions::NO_ACTION };
}


bool Snake_Game::is_final(std::shared_ptr<Actor<State_Type>> actor, State_Type state) const
{
	//TODO return is_final for future states
	return actors()[actor->id()].state_is_final;
};


Reward Snake_Game::reward(std::shared_ptr<Actor<State_Type>> actor, State_Type) const
{ 
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	if (m_actor_events[actor->id()] == Snake_World::Events::ATE)
		return 1.0f;
	else if (m_actor_events[actor->id()] == Snake_World::Events::CRASHED)
		return -1.0f;
	else
		return 0.0f;
}


State_Type Snake_Game::actual_state(std::shared_ptr<Actor<State_Type>> actor) const
{
	return {};
}


void Snake_Game::apply_action(std::shared_ptr<Actor<State_Type>> actor, Action action)
{
	std::unique_lock<std::mutex> lock(m_execution_lock);
	if (!actor->is_human())
	{
		//wait for last action to be executed
		m_actors_condition.wait(lock, [this]() {return m_unexecuted_actions < active_actors() - human_actors(); });
	}

	//assign action to actor
	exchange_action(actor, action);

	if (!actor->is_human())
	{
		m_unexecuted_actions++;

		//notify the environment that all actors could be finished
		m_environment_condition.notify_all();
		lock.unlock();
	}
}

//TODO
//converts the state to the following state but acts as if the other snakes can't move
//and as if it whould know where the apple will sporn
std::vector<State_Type> Snake_Game::assume_action(std::shared_ptr<Actor<State_Type>> actor, State_Type state, Action action) const
{ 
	////TODO convert state to world
	////Snake_World world_cpy = covert_to_world(state);
	//Snake_World world_cpy = world;

	//Snake_Entity* controlled_snake = &(world_cpy.snakes[actor->id()]);
	//controlled_snake->perform_action(action);

	////save old score
	//int old_score = controlled_snake->score();

	////handle events like ate apple or crashed
	//world_cpy.check_events();

	//State new_state = convert_to_state(actors()[actor->id()], world_cpy);
	////TODO should not be programmed like this
	////update actor scores and set event if score has changed
	////is crashed
	//if (old_score > controlled_snake->score())
	//	new_state[new_state.size() - 2] = 1;
	////has eaten
	//else if (old_score < controlled_snake->score())
	//	new_state[new_state.size() - 1] = 1;
	//

	return { };
};

////I_Environment helper
//const State Snake_Game::convert_to_state(Actor_Representation percieving_actor, const Snake_World& world) const
//{
//	//TODO find good state representation
//
//	/*
//	converts the world to a state, with two numbers for each field:
//	Empty fields will be represented as [0, 0].
//	Walls as [1, 0].
//	The apple as [2, 0]
//	The percieving actor will be represented as 3s, the own head as 4,
//	and the direction of the segment as a u=0, d=1, l=2, r=3, if there is no segment the field is 0
//	Other actors as 5s + i, heads of them as 6s + i. with i = 0 incremeting by 1 for each actor
//	the last two field represent is_crashed and has_eaten the are 1 if true
//	they won't be normalized
//
//	possible output before normalization
//	for 3 actors and a playingfield size of 10*10 and the p_actor has_eaten:
//	[1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [3, 0] [4, 3] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [3, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [8, 3] [0, 0] [2, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [5, 3] [5, 3] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [5, 1] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [5, 1] [6, 3] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [0, 0] [1, 0]
//	[1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0] [1, 0]
//
//	TODO at the moment all first values and after all secound values saved 
//
//	W W W W W W W W W W W
//	W . . . | o . . . . W
//	W . . . | . . . . A W
//	W . _ _ . . o . . . W
//	W . . | . . . . . . W
//	W . . | o . . . . . W
//	W . . . . . . . . . W
//	W . . . . . . . . . W
//	W . . . . . . . . . W
//	W W W W W W W W W W W
//	0 1
//
//	const int C_WALL = 1;
//	const int C_APPLE = 2;
//
//	const int C_P_ACTOR_BODY = 3;
//	const int C_P_ACTOR_HEAD = 4;
//	const int C_O_ACTOR_BODY = 5;
//	const int C_O_ACTOR_HEAD = 6;
//
//	const int C_IS_CRASHED = 1;
//	const int C_HAS_EATEN = 1;
//
//	const int C_UP = 0;
//	const int C_DOWN = 1;
//	const int C_LEFT = 2;
//	const int C_RIGHT = 3;
//	*/
//	
//	
//	//create temp 2d vector
//	std::vector<std::vector<std::pair<int, int>>> 
//		world_state(world.playing_field[0].size(),
//			        std::vector<std::pair<int, int>>
//			            (world.playing_field.size(), { 0, 0 })
//		);
//	 
//	//fill in the walls
//	for (int x = 0; x < world.playing_field.size(); x++) {
//		for (int y = 0; y < world.playing_field[0].size(); y++) {
//			if (world.playing_field[x][y] == Snake_World::Playing_Field::WALL) 
//			{
//				world_state[x][y].first = C_WALL;
//			}
//		}
//	}
//
//	//fill in the apple
//	world_state[world.apple.position.x][world.apple.position.y].first = C_APPLE;
//
//	//fill in the snakes
//	int snake_counter = 0;
//	for (auto snake : world.snakes)
//	{
//		for (auto snake_segment : snake.body())
//		{
//			if (snake_segment.is_head())
//				world_state[snake_segment.position().x][snake_segment.position().y].first = C_P_ACTOR_HEAD + snake_counter * 2;
//			else
//				world_state[snake_segment.position().x][snake_segment.position().y].first = C_P_ACTOR_BODY + snake_counter * 2;
//
//			switch (snake_segment.direction())
//			{
//			case Snake::Actions::U:
//				world_state[snake_segment.position().x][snake_segment.position().y].second = C_UP;
//				break;
//			case Snake::Actions::D:
//				world_state[snake_segment.position().x][snake_segment.position().y].second = C_DOWN;
//				break;
//			case Snake::Actions::L:
//				world_state[snake_segment.position().x][snake_segment.position().y].second = C_LEFT;
//				break;
//			case Snake::Actions::R:
//				world_state[snake_segment.position().x][snake_segment.position().y].second = C_RIGHT;
//				break;
//			case Snake::Actions::NO_ACTION:
//				break;
//			default:
//				std::cerr << "convert_to_state()\ninvalid action: " << snake_segment.direction() << "\n";
//				exit(-1);
//			}
//
//		}
//		snake_counter++;
//	}
//
//	//convert to state
//	std::vector<float> state;
//
//	for (int collumn = 0; collumn < world_state.size(); collumn++)
//	{
//		for (int row = 0; row < world_state[0].size(); row++)
//		{
//			state.push_back(world_state[row][collumn].first);
//			state.push_back(world_state[row][collumn].second);
//		}
//	}
//
//	//pushback events
//	if (m_actor_events[percieving_actor.actor->id()] == Snake_World::Events::CRASHED)
//		state.push_back(C_IS_CRASHED);
//	else
//		state.push_back(0.0f);
//	if (m_actor_events[percieving_actor.actor->id()] == Snake_World::Events::ATE)
//		state.push_back(C_HAS_EATEN);
//	else
//		state.push_back(0.0f);
//
//	////////////////////////////////
//
//	//TODO normalize
//	//for (auto& i : state)
//	//	i /= static_cast<float>(C_O_ACTOR_HEAD + snake_counter + 2);
//	
//	return state;
//}
//
//const Snake_World Snake_Game::convert_to_world(const State& state) const
//{
//	//create temp 2d vector
//	std::vector<std::vector<std::pair<int, int>>>
//		world_state(world.playing_field[0].size(),
//			std::vector<std::pair<int, int>>
//			(world.playing_field.size(), { 0, 0 })
//		);
//
//	//fill with state data
//	//last 2 do not belong to the world_state
//	for (int i = 0, x = 0, y = 0; i < state.size() - 2; i++)
//	{
//		x = (i / 2) - y * world.playing_field.size();
//		if (x == world.playing_field.size())
//		{
//			x = 0;
//			y++;
//		}
//		if (i % 2 == 0)
//			world_state[x][y].first = static_cast<int>(state[i]);
//		else
//			world_state[x][y].second = static_cast<int>(state[i]);
//	}
//
//	//create world
//	Snake_World new_world(world.playing_field.size());
//
//	//TODO fill in walls here instead of using the constructor
//
//	//set the apples position
//	for(int x = 0; x < world_state.size(); x++)
//		for(int y = 0; y < world_state[0].size(); y++)
//			if (world_state[x][y].first == C_APPLE)
//			{
//				new_world.apple.position.x = x;
//				new_world.apple.position.y = y;
//			}
//
//
//	//fill in the snakes//
//	//search all snake heads
//	std::vector<std::pair<Pos_int, int>> head_pos_number;
//	for (int x = 0; x < world_state.size(); x++)
//	{
//		for (int y = 0; y < world_state[0].size(); y++)
//		{
//			//found a head
//			if ((world_state[x][y].first >= C_P_ACTOR_HEAD)
//				&& (world_state[x][y].first % 2 == 0))
//			{
//				//save head position with its number
//				head_pos_number.push_back({ {x, y}, world_state[x][y].first });
//			}
//		}
//	}
//	//sort the heads by their number in ascending order
//	//after this 0 will be the percieving actor
//	std::sort(
//		std::begin(head_pos_number), 
//		std::end(head_pos_number), 
//		[](const auto& lhs, const auto& rhs) {return lhs.second < rhs.second; }
//	);
//
//	//TODO !!!FIX BUG!!! irgendwie geht das noch nicht lol
//	//Lässt sich wahrscheinlich nur machen wenn man die pairs durch echte namen ersetzt
//
//	//add snake and go to the position where the head came from, 
//	//if there is a body part add it, else continue with next snake
//	for (const auto& head_pos_num : head_pos_number)
//	{
//		//add head
//		new_world.add_snake(head_pos_num.first);
//		//set direction
//		new_world.snakes.back().body().back().direction() = static_cast<Actions>(world_state[head_pos_num.first.x][head_pos_num.first.y].second);
//
//		//add body
//		Pos_int possible_next_part = head_pos_num.first;
//		switch (new_world.snakes.back().body().back().direction())
//		{
//		case Snake::Actions::U:
//			possible_next_part.y++;
//			break;
//		case Snake::Actions::D:
//			possible_next_part.y--;
//			break;
//		case Snake::Actions::L:
//			possible_next_part.x--;
//			break;
//		case Snake::Actions::R:
//			possible_next_part.x++;
//			break;
//		case Snake::Actions::NO_ACTION:
//			break;
//		default:
//			std::cerr << "convert_to_world()\ninvalid action\n";
//			exit(-1);
//		}
//
//		//head number -1 is the representation of the body
//		while (world_state[possible_next_part.x][possible_next_part.y].first == head_pos_num.second - 1) 
//		{
//			//add segment
//			new_world.snakes.back().extend({possible_next_part.x, possible_next_part.y});
//			//set direction
//			new_world.snakes.back().body().back().direction() = static_cast<Actions>(world_state[possible_next_part.x][possible_next_part.y].second);
//
//			switch (new_world.snakes.back().body().back().direction())
//			{
//			case Snake::Actions::U:
//				possible_next_part.y++;
//				break;
//			case Snake::Actions::D:
//				possible_next_part.y--;
//				break;
//			case Snake::Actions::L:
//				possible_next_part.x--;
//				break;
//			case Snake::Actions::R:
//				possible_next_part.x++;
//				break;
//			case Snake::Actions::NO_ACTION:
//				break;
//			default:
//				std::cerr << "convert_to_world()\ninvalid action\n";
//				exit(-1);
//			}
//		}
//	}
//	//ignore events
//	
//	////////////////////////////////
//
//	return {};//new_world;
//}



//
////DEBUG
////TODO make const again
//void Snake_Game::console_print(State state) //const
//{
//	if (state.back())
//	{
//		for (int i = 0; i < state.size() - 2; i += 2)
//		{
//			if (i % (world.playing_field.size() * 2) == 0)
//				std::cout << std::endl << std::endl;
//			std::cout << "[" << state[i] << ", " << state[i + 1] << "] ";
//		}
//
//		std::cout << "\nis crashed: " << state[state.size() - 2] << ", has eaten: " << state[state.size() - 1];
//		std::cout << std::endl;
//
//
//		//DEBUG
//		std::cout << "\n2 Sekunden bis crash!!!\n";
//		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//
//		world = convert_to_world(state);
//
//
//		std::cout << "\nconverted!!!\n";
//		//END DEBUG
//	}
//}
//void Snake_Game::console_print(State state) const
//{
//	for (int i = 0; i < state.size() - 2; i += 2)
//	{
//		if (i % (world.playing_field.size() * 2) == 0)
//			std::cout << std::endl << std::endl;
//		std::cout << "[" << state[i] << ", " << state[i + 1] << "] ";
//	}
//
//	std::cout << "\nis crashed: " << state[state.size() - 2] << ", has eaten: " << state[state.size() - 1];
//	std::cout << std::endl;
//
//
//	//DEBUG
//	std::cout << "\n2 Sekunden bis crash!!!\n";
//	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//
//
//	std::cout << "\nconverted!!!\n";
//	//END DEBUG
//}