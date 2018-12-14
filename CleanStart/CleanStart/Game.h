#pragma once
#include "Game_Graphics.h"

#include <memory>
#include <thread>
#include "SFML/Graphics.hpp"
#include "Environment.h"
#include "Environment_def.cpp"

namespace Ai_Arena
{
	template<class Graphics, class State_T>
	class Game_Base : public Environment<State_T>
	{
		static_assert(std::is_base_of<Game_Graphics, Graphics>::value, "Game_Base template class \"Graphics\" must be derived from Game_Graphics");
	public:
		Game_Base();
		~Game_Base();

		//is called in run immediately before the game loop stats
		virtual void set_up() {};
		virtual void run();
		virtual void execute_actions() {};

		//GET/SET
		std::shared_ptr<Graphics>& graphics() { return m_graphics; };
		void      graphics(std::shared_ptr<Graphics> graphics) { m_graphics = graphics; }

	protected:
		std::shared_ptr<Graphics> m_graphics;

		virtual void update() = 0;
	};
}

