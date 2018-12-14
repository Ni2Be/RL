#include "Human_Player.h"
#include <SFML/Graphics.hpp>

using namespace Ai_Arena;

template <class State_T>
Human_Player<State_T>::Human_Player(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Actor<State_T>(enviroment)
{}
