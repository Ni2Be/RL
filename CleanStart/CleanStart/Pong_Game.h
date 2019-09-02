#pragma once
#include "Game.h"
#include "Game.cpp"
#include "Pong_World.h"
#include "Pong_Graphics.h"

namespace Ai_Arena
{
	class Pong_Game : public Game_Base<Pong_Graphics, Pong_World>
	{
	public:
		Pong_Game(int paddle_length, int fields_x, int fields_y, int field_pixel);

		void update_world();

		//does everything needed to start the game
		//places the apple, and a snake for each actor
		void set_up();
	private:
		Pong_World world;
		mutable std::mutex m_world_lock;
		int         m_fields_width_count;
		void execute_actions();
	public:
		//I_Environment


		std::vector<Action> possible_actions(std::shared_ptr<Actor<Pong_World>>, Pong_World) const;
		std::vector<Pong_World>  assume_action(std::shared_ptr<Actor<Pong_World>>, Pong_World, Action) const;
		void   apply_action(std::shared_ptr<Actor<Pong_World>>, Action);
		Pong_World  current_state(std::shared_ptr<Actor<Pong_World>>) const;

		Perception get_perception(std::shared_ptr<Actor<Pong_World>>, Sensor) const;
		Perception get_perception(std::shared_ptr<Actor<Pong_World>>, Sensor, Pong_World) const;

		


		Reward reward(std::shared_ptr<Actor<Pong_World>>, Pong_World) const;
		bool is_final(std::shared_ptr<Actor<Pong_World>>, Pong_World, bool is_simulation = false) const;
		void add_actor(std::shared_ptr<Actor<Pong_World>>);
		void update();
		std::vector<double> get_stats();
		void debug(std::shared_ptr<Actor<Pong_World>>);
		double normalize_width(double input) const;
		double normalize_height(double input) const;

		////DEBUG
		//void console_print(State);// const;
		//void console_print(State) const;

	private:
		////I_Environment helper
		const Perception convert_to_SEE_THE_WHOLE_STATE(Actor_Representation perceiving_actor, const Pong_World& world) const;
		const Perception own_position_and_ball(Actor_Representation perceiving_actor, const Pong_World& world) const;
		std::vector<Pong_World::Events> m_actor_events;



		int m_old_lives = 10;
		int C_MAX_SCORE = 10000;

		double x1_w, x2_w, y1_w, y2_w, m_w, b_w;
		double x1_h, x2_h, y1_h, y2_h, m_h, b_h;

		const int C_WALL = 1;
		const int C_BALL = 2;
		const int C_P_ACTOR_BODY = 3;
		const int C_O_ACTOR_BODY = 4;

		const int C_BALL_X_POS = 0;
		const int C_BALL_Y_POS = 0;
		const int C_BALL_X_NEG = 1;
		const int C_BALL_Y_NEG = 1;
		

		const int C_IS_BOUNCED = 1;
		const int C_HAS_SCORED = 1;
		const int C_UP = 0;
		const int C_DOWN = 1;
	};
}