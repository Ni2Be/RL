/*
class Multi_Snake:
	Contains the game rules and handles the
	communication with an Actor

	the world contains the snakes and the apple
	the snakes know how to move and how to grow
	the game rules decide when a snake grows, it 
	has to respawn, or it has lost.
*/

#pragma once
#include "Game.h"
#include "Game.cpp"
#include "Snake_World.h"
#include "Snake_Graphics.h"
#include <vector>

namespace Ai_Arena
{
	class Snake_Game : public Game_Base<Snake_Graphics, Snake_World>
	{
	public:

		Snake_Game(int fields_width_count, int field_pixel);

		void update_world();

		//does everything needed to start the game
		//places the apple, and a snake for each actor
		void set_up();
	private:
		Snake_World  world;
		mutable std::mutex m_world_lock;
		int         m_fields_width_count;

		void execute_actions();

		std::vector<int> action_counter;
	public:
		//I_Environment


		std::vector<Action> possible_actions(std::shared_ptr<Actor<Snake_World>>, Snake_World) const;
		std::vector<Snake_World>  assume_action(std::shared_ptr<Actor<Snake_World>>, Snake_World, Action) const;
		void   apply_action(std::shared_ptr<Actor<Snake_World>>, Action);
		Snake_World  current_state(std::shared_ptr<Actor<Snake_World>>) const;

		Perception get_perception(std::shared_ptr<Actor<Snake_World>>, Sensor) const;
		Perception get_perception(std::shared_ptr<Actor<Snake_World>>, Sensor, Snake_World) const;

		Reward reward(std::shared_ptr<Actor<Snake_World>>, Snake_World) const;
		bool is_final(std::shared_ptr<Actor<Snake_World>>, Snake_World, bool is_simulation = false) const;
		void add_actor(std::shared_ptr<Actor<Snake_World>>);
		void update();
		std::vector<double> get_stats();
		void debug(std::shared_ptr<Actor<Snake_World>>);

		////DEBUG
		//void console_print(State);// const;
		//void console_print(State) const;
	private:
		////I_Environment helper
		const Perception convert_to_SEE_THE_WHOLE_STATE(Actor_Representation perceiving_actor, const Snake_World& world) const;
		std::vector<Snake_World::Events> m_actor_events;
		std::vector<Snake_World::Events> m_actor_last_events;


		int m_old_lifes = 0;

		


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