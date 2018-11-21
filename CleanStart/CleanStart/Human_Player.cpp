#include "Human_Player.h"
#include <SFML/Graphics.hpp>

using namespace Ai_Arena;

Human_Player::Human_Player(std::shared_ptr<I_Environment> enviroment)
	:
	Actor(enviroment)
{}
