/*
class Multi_Snake:
	Handles the	communication with all Actors

	the world contains the snakes, the apple and 
	the walls. 
	The snakes know how to move and how to grow.
	The game rules like "what is to do if a snake 
	hits the wall?", are definded in Snake_World.h
*/

#pragma once
#include "Game.h"
#include "Game.cpp"
#include "Snake_World.h"
#include "Snake_Graphics.h"


namespace Ai_Arena
{
	class Snake_Game : public Game_Base<Snake_Graphics, State_Type>
	{
	public:
		Snake_Game(int fields_width_count, int field_pixel);

		void update_world();

		//sets the state the first time, wakes up the actors
		void set_up();
	private:
		Snake_World world;
		int         m_fields_width_count;

		void execute_actions();
	public:
		//I_Environment

		//TODO not int
		std::vector<Action> possible_actions(std::shared_ptr<Actor<State_Type>>, State_Type) const;
		std::vector<State_Type>  assume_action(std::shared_ptr<Actor<State_Type>>, State_Type, Action) const;
		void   apply_action(std::shared_ptr<Actor<State_Type>>, Action);
		State_Type  actual_state(std::shared_ptr<Actor<State_Type>>) const;
		Reward reward(std::shared_ptr<Actor<State_Type>>, State_Type) const;
		bool   is_final(std::shared_ptr<Actor<State_Type>>, State_Type) const;
		void add_actor(std::shared_ptr<Actor<State_Type>>);
		void update();

		////DEBUG
		//void console_print(State);// const;
		//void console_print(State) const;

	private:
		////I_Environment helper
		//const State convert_to_state(Actor_Representation percepting_actor, const Snake_World& world) const;
		//const Snake_World convert_to_world(const State& state) const;
		std::vector<Snake_World::Events> m_actor_events;

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
	};
}