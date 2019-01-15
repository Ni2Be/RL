#pragma once
#include "Types.h"

namespace Ai_Arena
{
	class Snake_Entity;

	namespace Snake
	{
		enum Actions
		{
			U, D, L, R, NO_ACTION
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
		Actions m_direction = NO_ACTION;
	};

	class Snake_Entity
	{
	public:

		Snake_Entity();

		void perform_action(Action action);

		Pos_int position();

		//grow the snake
		void extend();
		void extend(Pos_int pos);

		// placed on a random position in the given range
		void respown(Area_int area);

		//cuts off a given number of segements
		//void cut(int segment_count);

		//handle game over here
		void game_over();

		//get/set
		bool& is_crashed() { return m_is_crashed; }
		const bool& is_crashed() const { return m_is_crashed; }

		inline std::vector<Snake_Segment>& body() { return m_body; }
		inline const std::vector<Snake_Segment>& body() const { return m_body; }

		inline const Pos_int& head_position() const { return body()[0].position(); }

		inline int& lives() { return m_lives; }
		inline const int lives() const { return m_lives; }
		inline int& score() { return m_score; }
		inline const int score() const { return m_score; }
		inline int& last_score() { return m_last_score; }
		inline const int last_score() const { return m_last_score; }


		inline bool& has_lost() { return m_has_lost; };
		inline const bool has_lost() const { return m_has_lost; };
	private:

		std::vector<Snake_Segment> m_body;

		
		Snake_Segment m_prev_tail_end = {0,0};
		Action  m_current_action = Actions::NO_ACTION;

		int  m_lives = 30;
		int  m_last_score = 0;
		int  m_score = 0;
		bool m_has_lost = false;

		bool m_is_crashed = false;
	};
}