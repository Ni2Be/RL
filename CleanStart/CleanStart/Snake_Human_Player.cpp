#include "Snake_Human_Player.h"
#include "Game.h"
#include "Snake_Graphics.h"
#include <SFML/Graphics.hpp>
#include <queue>
# include <SFML/Window/Joystick.hpp>
using namespace Ai_Arena;


template <class State_T>
Snake_Human_Player<State_T>::Snake_Human_Player(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Human_Player<State_T>(enviroment)
{}

template <class State_T>
void Snake_Human_Player<State_T>::set_action()
{
	if (!Human_Player<State_T>::m_environment->is_final(Human_Player<State_T>::m_self_pointer, Human_Player<State_T>::m_environment->current_state(Human_Player<State_T>::m_self_pointer)))
	{
		auto events = Human_Player<State_T>::m_environment->get_events();

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


			if (sf::Joystick::isConnected)
			{
				if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) == 100)
					m_last_action = Actions::U;
				else if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) == -100)
					m_last_action = Actions::D;
				else if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == -100)
					m_last_action = Actions::L;
				else if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == 100)
					m_last_action = Actions::R;
			}

			events.pop();
		}

		m_next_action = m_last_action;
	}
	else
	{
		std::cout << "human player sleeping!\n";
		Human_Player<State_T>::sleep();
	}
}
