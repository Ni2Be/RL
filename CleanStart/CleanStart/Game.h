#pragma once
#include "Game_Graphics.h"

class Game
{
public:
	explicit Game(int width, int height, const std::string window_name);
	~Game();

	void run();
protected:
	virtual void update_game() = 0;
	bool is_running = true;
	Game_Graphics graphics;
};

