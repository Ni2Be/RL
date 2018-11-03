#pragma once
#include <vector>

typedef std::vector<float> State;
typedef float Reward;
typedef enum Action;

class I_Observable_Environment
{
public:
	Reward reward(State);
	std::vector<Action> possible_actions(State);
	State assume_action(State, Action);
	State applay_action(State, Action);
	State actual_state();
protected:
	State game_state();

	State m_game_state;
	I_Observable_Environment() {};
};

//importieren:
//ml-agents-master/unitySDK

/*
dann examples/3dball
braintype erst external dann internal

intellij idea 

local refernezieren punkt weg machen


*/