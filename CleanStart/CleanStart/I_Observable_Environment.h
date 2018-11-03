#pragma once
#include <vector>


template<class Action>
class I_Observable_Environment
{
public:
	typedef std::vector<float> State;
	typedef float Reward;

	virtual Reward reward(State) = 0;
	virtual std::vector<Action> possible_actions(State) = 0;
	virtual State assume_action(State, Action) = 0;
	virtual State applay_action(State, Action) = 0;
	virtual State actual_state() = 0;
protected:
	State m_game_state;
	I_Observable_Environment() {};
};

