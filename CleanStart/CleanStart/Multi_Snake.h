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
#include "Snake_World.h"
#include "Multi_Snake_Graphics.h"

namespace Ai_Arena
{
	class Multi_Snake : public Game_Base<Multi_Snake_Graphics>
	{
	public:
		Multi_Snake(int fields, int field_pixel);

		void update_world();

		//does everything needed to start the game
		//places the apple, and a snake for each actor
		void set_up();
	private:
		Snake_World world;
		int         m_fields;

		void execute_actions();
	public:
		//I_Environment
		std::vector<Action> possible_actions(std::shared_ptr<Actor>, State) const;
		std::vector<State>  assume_action(std::shared_ptr<Actor>, State, Action) const { return {}; };
		void   apply_action(std::shared_ptr<Actor>, Action);
		State  actual_state(std::shared_ptr<Actor>) const { return {}; };
		Reward reward(std::shared_ptr<Actor>, State) const { return 0.0f; };
		bool   is_final(std::shared_ptr<Actor>, State) const { return false; };
		void add_actor(std::shared_ptr<Actor>);
		void update();
	};
}