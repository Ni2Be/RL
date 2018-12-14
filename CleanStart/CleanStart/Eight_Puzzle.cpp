//#include "Eight_Puzzle.h"
//
//#include <random>
//#include <numeric>
//#include <algorithm>
//
//#include "Eight_Puzzle_Graphics.h"
//#include "Actor.h"
//
//using namespace Ai_Arena;
//
//Eight_Puzzle::Eight_Puzzle(int width, int height, int field_pixel)
//	:
//	m_field_width(width),
//	m_field_height(height)
//{
//	graphics(
//		std::shared_ptr<Eight_Puzzle_Graphics>(
//			new Eight_Puzzle_Graphics(
//				field_pixel * width, 
//				field_pixel * height, 
//				"Eight Puzzle", 
//				field_pixel)));
//	do
//	{
//		build_field();
//	} while (!is_solvable());
//
//	m_graphics->update_graphics(m_playing_field);
//
//	set_environment_state(convert_to_state(m_playing_field));
//}
//
//
//Eight_Puzzle::~Eight_Puzzle()
//{
//}
//
////GAME
//void Eight_Puzzle::build_field()
//{
//	/*
//	Builds a playing field using numbers from 1 to (n - 1) for fields
//	and 0 for the empty field.
//	It is possible that the field can't be solved.
//	*/
//	m_playing_field = Playing_Field(m_field_height, std::vector<int>(m_field_width));
//	std::vector<int> field_values(m_field_width * m_field_height);
//	std::iota(field_values.begin(), field_values.end(), 0);
//	std::shuffle(field_values.begin(), field_values.end(), std::mt19937{ std::random_device{}() });
//
//	int i = 0;
//	for (int row = 0; row < m_playing_field.size(); row++)
//	{
//		for (int collumn = 0; collumn < m_playing_field[row].size(); collumn++)
//		{
//			if (field_values[i] == 0)
//			{
//				m_playing_field.emty_position.y = row;
//				m_playing_field.emty_position.x = collumn;
//			}
//			m_playing_field[row][collumn] = field_values[i++];
//		}
//	}
//}
//
//bool Eight_Puzzle::is_solvable()
//{
//	int inversion_counter = 0;
//	console_print(m_playing_field);
//	for (int l_row = 0; l_row < m_playing_field.size(); l_row++)
//		for (int l_collumn = 0; l_collumn < m_playing_field[l_row].size(); l_collumn++)
//			for (int r_row = l_row, r_collumn = l_collumn; r_row < m_playing_field.size(); r_row++)
//			{
//				for (; r_collumn < m_playing_field[r_row].size(); r_collumn++)
//					if ((m_playing_field[l_row][l_collumn] > m_playing_field[r_row][r_collumn]) && (m_playing_field[r_row][r_collumn] != 0))
//						inversion_counter++;
//				r_collumn = 0;
//			}
//
//	/*
//	In general, for a given grid of width N, we can find out check if a N*N – 1 puzzle is solvable or not by following below simple rules :
//    If N is odd, then puzzle instance is solvable if number of inversions is even in the input state.
//    If N is even, puzzle instance is solvable if
//        the blank is on an even row counting from the bottom (second-last, fourth-last, etc.) and number of inversions is odd.
//        the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.) and number of inversions is even.
//    For all other cases, the puzzle instance is not solvable.
//	*/
//	if (m_field_width % 2 == 0)
//		if ((m_field_height - m_playing_field.emty_position.y) % 2 == 0)
//			return !(inversion_counter % 2 == 0);
//		else
//			return (inversion_counter % 2 == 0);
//	else
//		return (inversion_counter % 2 == 0);
//}
//
//void Eight_Puzzle::update_puzzle()
//{
//	m_counter++;
//	execute_actions();
//	set_environment_state(convert_to_state(m_playing_field));
//
//	//m_graphics->update_graphics(m_playing_field);
//	if (m_counter % 10000 == 0)
//	{
//		std::cout << "updates: " << m_counter << std::endl << "actions: " << m_action_counter << std::endl << std::endl;
//		m_graphics->update_graphics(m_playing_field);
//	}
//	if (prove_victory_condition(m_playing_field))
//	{
//		std::cout << "WIN!!! updates: " << m_counter << std::endl;
//		m_graphics->update_graphics(m_playing_field);
//		sf::sleep(sf::milliseconds(1000));
//		m_counter = 0;
//		m_action_counter = 0;
//	}
//}
//
//bool Eight_Puzzle::prove_victory_condition(Playing_Field playing_field) const
//{
//	int field_value = playing_field[0][0];
//	for (const auto& row : playing_field)
//	{
//		for (const auto& field : row)
//		{
//			if ((field_value > field) && (field != 0))
//				return false;
//			field_value = field;
//		}
//	}
//	if (playing_field[playing_field.size() - 1][playing_field[0].size() - 1] == 0)
//		return true;
//	else
//		return false;
//}
//
//void Eight_Puzzle::swap_field_values(Pos_int p1, Pos_int p2, Playing_Field& f) const
//{
//	int temp = f[p1.y][p1.x];
//	f[p1.y][p1.x] = f[p2.y][p2.x];
//	f[p2.y][p2.x] = temp;
//}
//
//void Eight_Puzzle::execute_actions()
//{
//	std::unique_lock<std::mutex> lock(m_execution_lock);
//	//wait for all actors to place an action
//	m_environment_condition.wait(lock, [this]() {return m_unexecuted_actions == active_actors(); });
//
//	//execute all actions
//	for (auto& actor_action_pair : actors())
//	{
//		m_playing_field = execute_action(m_playing_field, actor_action_pair.action);// exchange_action(actor_action_pair.first, NON));
//		set_environment_state(convert_to_state(m_playing_field));
//		m_unexecuted_actions--;
//	}
//
//	//wakeup the actors
//	m_actors_condition.notify_all();
//	lock.unlock();
//}
//
//Eight_Puzzle::Playing_Field Eight_Puzzle::execute_action(Playing_Field playing_field, Action action) const
//{
//	switch (action.action)
//	{
//	case Actions::U:
//		if (!(playing_field.emty_position.y == playing_field.size() - 1))
//		{
//			swap_field_values(
//				playing_field.emty_position, 
//				Pos_int(playing_field.emty_position.y + 1, 
//					playing_field.emty_position.x), 
//				playing_field);
//			playing_field.emty_position.y++;
//		}
//		break;
//	case Actions::D:
//		if (!(playing_field.emty_position.y == 0))
//		{
//			swap_field_values(
//				playing_field.emty_position, 
//				Pos_int(playing_field.emty_position.y - 1, 
//					playing_field.emty_position.x), 
//				playing_field);
//			playing_field.emty_position.y--;
//		}
//		break;
//	case Actions::R:
//		if (!(playing_field.emty_position.x == playing_field[playing_field.emty_position.y].size() - 1))
//		{
//			swap_field_values(
//				playing_field.emty_position, 
//				Pos_int(playing_field.emty_position.y, 
//					playing_field.emty_position.x + 1), 
//				playing_field);
//			playing_field.emty_position.x++;
//		}
//		break;
//	case Actions::L:
//		if (!(playing_field.emty_position.x == 0))
//		{
//			swap_field_values(
//				playing_field.emty_position, 
//				Pos_int(playing_field.emty_position.y, 
//					playing_field.emty_position.x - 1), 
//				playing_field);
//			playing_field.emty_position.x--;
//		}
//		break;
//	case Actions::NO_ACTION:
//		break;
//	default:
//		std::cerr << std::endl << "invalid action" << std::endl;
//		exit(-1);
//	}
//	return playing_field;
//}
//
//
////CONVERTER
//State Eight_Puzzle::convert_to_state(const Playing_Field playing_field) const
//{
//	State state(playing_field[0].size() * playing_field.size());
//	int i = 0;
//	for (const auto& row : playing_field)
//		for (const auto& field : row)
//			state[i++] = static_cast<float>(field);
//	return state;
//}
//
//Eight_Puzzle::Playing_Field Eight_Puzzle::convert_to_playing_field(const State state) const
//{
//	Playing_Field field(m_field_height, std::vector<int>(m_field_width));
//	int i = 0;
//	for (auto& row : field)
//		for (auto& field : row)
//			field = static_cast<int>(state[i++]);
//	return field;
//}
//
//
////OBSERVABLE ENVIROMENT
//std::vector<Action> Eight_Puzzle::possible_actions(std::shared_ptr<Actor>, State) const
//{
//	return { {Actions::U}, {Actions::D}, {Actions::R}, {Actions::L}, {Actions::NO_ACTION, false}};
//}
//
////THREADSAFE
//std::vector<State> Eight_Puzzle::assume_action(std::shared_ptr<Actor> actor, State state, Action action) const
//{
//	std::cerr << "Eight_Puzzle::assume_action not implemented!" << std::endl;
//	exit(-1);
//	//TODO
//}
//
//
////THREADSAFE
//void Eight_Puzzle::apply_action(std::shared_ptr<Actor> actor, Action action)
//{
//	std::unique_lock<std::mutex> lock(m_execution_lock);
//	//wait for last action to be executed
//	m_actors_condition.wait(lock, [this]() {return m_unexecuted_actions < active_actors(); });
//
//	//assign action to actor
//	exchange_action(actor, action);
//	m_unexecuted_actions++;
//	m_action_counter++;
//
//	//notify the environment that all actors could be finished
//	m_environment_condition.notify_all();
//	lock.unlock();
//}
//
////THREADSAFE
//State Eight_Puzzle::actual_state(std::shared_ptr<Actor>) const
//{
//	return environment_state();
//}
//
////THREADSAFE
//Reward Eight_Puzzle::reward(std::shared_ptr<Actor>, State state) const
//{
//	//TODO implement manhattan distance
//	if (prove_victory_condition(convert_to_playing_field(state)))
//		return 100.0;
//	return 0.0;
//}
//
////THREADSAFE
//bool Eight_Puzzle::is_final(std::shared_ptr<Actor>, State state) const
//{
//	return prove_victory_condition(convert_to_playing_field(state));
//}
//
////THREADSAFE
//void Eight_Puzzle::add_actor(std::shared_ptr<Actor> actor)
//{
//	Environment::add_actor(Actor_Representation(actor, { Actions::NO_ACTION }));
//}
//
//void Eight_Puzzle::update()
//{
//	actors()[0].actor->wake_up();
//	update_puzzle();
//}
//
//
//
////DEBUG
//void Eight_Puzzle::console_print(Playing_Field playing_field)
//{
//	for (auto& row : playing_field)
//	{
//		for (auto& field : row)
//			std::cout << field << " ";
//		std::cout << std::endl;
//	}
//}