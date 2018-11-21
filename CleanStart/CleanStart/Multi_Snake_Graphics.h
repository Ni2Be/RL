#pragma once
#include "Game_Graphics.h"

namespace Ai_Arena
{
	class Snake_World;
	class Snake_Segment;
	class Snake_Entity;
	class Apple;
	class Playing_Field;

	class Multi_Snake_Graphics : public Game_Graphics
	{
	public:
		Multi_Snake_Graphics(int fields, const std::string title, int field_pixel)
			:
			m_field_pixel(field_pixel),
			Game_Graphics(fields * field_pixel, fields * field_pixel, title)
		{}


		void update_graphics(Snake_World world);



		class Snake_Segment_G : public sf::Drawable
		{
		public:
			Snake_Segment_G(const Snake_Segment& segment, int field_pixel, sf::Color color);

			sf::RectangleShape rect;
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};

		class Snake_Entity_G : public sf::Drawable
		{
		public:
			Snake_Entity_G(const Snake_Entity&, int field_pixel, sf::Color head_color, sf::Color color);

			sf::Color m_color;
			sf::Color m_head_color;//not used

			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			std::vector<Snake_Segment_G> segments;
		};

		class Apple_G : public sf::Drawable
		{
		public:
			Apple_G(const Apple&);

			sf::CircleShape circle;
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};

	private:
		int m_field_pixel;
	};
}