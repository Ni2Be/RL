#pragma once
#include "Types.h"

namespace Ai_Arena
{
	class Snake_Entity;

	namespace Snake
	{
		enum Actions
		{
			U, D, R, L, NO_ACTION
		};
	}
	using namespace Snake;

	class Snake_Segment
	{
		friend class Snake_Entity;
	public:
		Snake_Segment(Pos_int pos) : m_position(pos) {}
		Snake_Segment(int x, int y) : m_position(x, y) {}
		inline Pos_int& position() { return m_position; }
		inline Actions& direction() { return m_direction; }
		inline bool& is_head() { return m_is_head; }
		inline const Pos_int& position() const { return m_position; }
		inline const Actions& direction() const { return m_direction; }
		inline const bool& is_head() const { return m_is_head; }
	private:
		bool m_is_head = false;
		Pos_int m_position;
		Actions m_direction;
	};

	class Snake_Entity
	{
	public:

		Snake_Entity();

		void perform_action(Action action);

		Pos_int position();

		bool has_lost();
		//grow the snake
		void extend();
		// placed on a random position in the given range
		void respown(Area_int area);

		//cuts off a given number of segements
		//void cut(int segment_count);

		//handle game over here
		void game_over();

		//get/set
		int& speed() { return m_speed; }
		bool& is_crashed() { return m_is_crashed; }
		const int& speed() const { return m_speed; }
		const bool& is_crashed() const { return m_is_crashed; }

		inline std::vector<Snake_Segment>& body() { return m_body; }
		inline const std::vector<Snake_Segment>& body() const { return m_body; }

		inline const Pos_int& head_position() const { return body()[0].position(); }
	private:

		std::vector<Snake_Segment> m_body;

		
		Pos_int m_prev_tail_end = {0,0};
		Action m_current_action = Actions::NO_ACTION;
		int  m_speed = 1;
		int  m_lives = 3;
		int  m_score = 0;
		bool m_lost = false;

		bool m_is_crashed;
	};
}