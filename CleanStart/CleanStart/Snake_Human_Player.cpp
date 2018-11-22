#include "Snake_Human_Player.h"
#include "Game.h"
#include "Multi_Snake_Graphics.h"
#include <SFML/Graphics.hpp>
#include <queue>
using namespace Ai_Arena;


Snake_Human_Player::Snake_Human_Player(std::shared_ptr<Environment> enviroment)
	:
	Human_Player(enviroment)
{}

void Snake_Human_Player::evaluate_action()
{
	auto events = m_environment->get_events();

	while (!events.empty())
	{
		using namespace Snake;
		if (events.front().type == sf::Event::KeyPressed)
		{
			if (events.front().key.code == sf::Keyboard::Up)
				m_last_action = Actions::U;
			else if (events.front().key.code == sf::Keyboard::Down)
				m_last_action = Actions::D;
			else if (events.front().key.code == sf::Keyboard::Left)
				m_last_action = Actions::L;
			else if (events.front().key.code == sf::Keyboard::Right)
				m_last_action = Actions::R;
		}
		events.pop();
	}

	set_action(m_last_action);
	//m_environment->apply_action(
	//	m_self_pointer,
	//	m_last_action
	//);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if (m_environment->is_final(m_self_pointer, m_environment->actual_state(m_self_pointer)))
	{
		sleep();
	}
}
