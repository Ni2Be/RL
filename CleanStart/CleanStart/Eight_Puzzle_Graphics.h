//#pragma once
//#include "Game_Graphics.h"
//#include <SFML/Graphics/Drawable.hpp>
//namespace Ai_Arena
//{
//	class Eight_Puzzle_Graphics : public Game_Graphics
//	{
//	public:
//		Eight_Puzzle_Graphics(int width, int height, const std::string title, int field_pixel)
//			:
//			m_field_pixel(field_pixel),
//			Game_Graphics(width, height, title)
//		{}
//		typedef std::vector<std::vector<int>> Playing_Field;
//		void update_graphics(const Playing_Field& playing_field);
//		int m_field_pixel;
//	private:
//		class Labeled_Rect : public sf::Drawable
//		{
//		public:
//			Labeled_Rect(
//				sf::Vector2f size,
//				sf::Vector2f pos,
//				sf::Color    color,
//				const sf::Font& font,
//				std::string     text = "",
//				int             text_size = 90);
//
//			sf::RectangleShape rect;
//			sf::Text label;
//			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
//		};
//	};
//}