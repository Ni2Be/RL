#pragma once
#include "Game_Graphics.h"



class Eight_Puzzle_Graphics : public Game_Graphics
{
public:
	Eight_Puzzle_Graphics(int width, int height, const std::string title, int field_pixel)
		:
		m_field_pixel(field_pixel),
		Game_Graphics(width, height, title)
	{}
	typedef std::vector<std::vector<int>> Playing_Field;
	void update_graphics(const Playing_Field& playing_field);
	int m_field_pixel;
};