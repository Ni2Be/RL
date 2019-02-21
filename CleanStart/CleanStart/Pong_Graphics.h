#pragma once
#include "Game_Graphics.h"

namespace Ai_Arena
{
	class Pong_World;
	class Paddle;
	class Paddle_Segment;
	class Ball;
	class Playing_Field;

	class Pong_Graphics : public Game_Graphics
	{
	public:

		Pong_Graphics(int fields_x, int fields_y, const std::string title, int field_pixel)
			:
			m_field_pixel(field_pixel),
			Game_Graphics(fields_x * field_pixel, fields_y * field_pixel, title)
		{}


		void update_graphics(Pong_World world);


		class Paddle_Segment_G : public sf::Drawable
		{
		public:
			Paddle_Segment_G(const Paddle_Segment& segment, int field_pixel, sf::Color color);

			sf::RectangleShape rect;

			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};

		class Paddle_G : public sf::Drawable
		{
		public:
			Paddle_G(const Paddle&, int field_pixel, sf::Color color);

			//sf::RectangleShape rect;
			sf::Color m_color;
			//sf::Color m_head_color;//not used


			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			std::vector<Paddle_Segment_G> segments;
		};

		

		class Ball_G : public sf::Drawable
		{
		public:
			Ball_G(const Ball&);

			sf::CircleShape circle;
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			std::vector<Paddle_G> segments;
		};

	private:
		int m_field_pixel;


	};
}