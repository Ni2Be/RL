#pragma once
#include "Game_Graphics.h"

class Game
{
public:
	explicit Game(int width, int height, const std::string window_name);
	~Game();

	void run();
	Game_Graphics& graphics() { return m_graphics; };
protected:
	virtual void update_game() = 0;
	bool m_is_running = true;
	Game_Graphics m_graphics;
};

