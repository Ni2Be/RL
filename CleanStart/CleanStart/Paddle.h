#pragma once
#include "Types.h"

namespace Ai_Arena
{
	class Paddle;

	namespace Pong
	{
		enum Actions
		{
			U, D, NO_ACTION
		};
		
		enum Sides
		{
			L, R,NONE
		};
		
	}
	using namespace Pong;

	class Paddle_Segment
	{
		friend class Paddle;
	public:
		Paddle_Segment(Pos_int pos) : m_position(pos) {}
		Paddle_Segment(int x, int y) : m_position(x, y) {}
		inline Pos_int& position() { return m_position; }
		inline Pong::Actions& direction() { return m_direction; }
		inline const Pos_int& position() const { return m_position; }
		inline const Pong::Actions& direction() const { return m_direction; }


	private:
		Pos_int m_position;
		Pong::Actions m_direction;
	};

	class Paddle
	{
	public:
		Paddle();
		void perform_action(Action action);
		Pos_int position();
		void reset_position();
		void set_values(Pos_int pos, int paddle_length, int side);
		//inline const void set_values(Pos_int pos, int paddle_length, int side) const;
		
		Action get_action();
		


		inline int& get_length() { return m_length; }
		inline const int get_length() const { return m_length; }

		inline int& get_side() { return m_side; }
		inline const int get_side() const { return m_side; }
		
		//handle game over here
		void game_over();

		inline std::vector<Paddle_Segment>& body() { return m_body; }
		inline const std::vector<Paddle_Segment>& body() const { return m_body; }
		void increase_Score(int score);
		
		inline Pos_int& start_position() { return m_start_position; };
		inline const Pos_int start_position() const { return m_start_position; };

		inline bool& has_lost() { return m_has_lost; };
		inline const bool has_lost() const { return m_has_lost; };

		inline int& score() { return m_score; }
		inline const int score() const { return m_score; }
		inline int& hits() { return m_hits; }
		inline const int hits() const { return m_hits; }
		inline int& last_score() { return m_last_score; }
		inline const int last_score() const { return m_last_score; }


	private:
		std::vector<Paddle_Segment> m_body;
		Action m_current_action = Pong::Actions::NO_ACTION;
		Pos_int m_start_position;
		int m_score = 0;
		int m_hits = 0;
		int  m_last_score = 0;
		int m_side;
		int m_length;
		bool m_has_lost = false;

		

		
	};
}