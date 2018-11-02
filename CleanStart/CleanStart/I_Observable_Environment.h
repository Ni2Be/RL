#pragma once
#include <vector>

template <class State, class Action, class Reward>
class I_Observable_Environment
{
public:
	Reward reward(State);
	std::vector<Action> possible_actions(State);
	State assume_action(State, Action);
	State applay_action(State, Action);
	State actual_state();
protected:
	State m_game_state;
	std::vector<Action> m_actions;
	I_Observable_Environment(){}
};