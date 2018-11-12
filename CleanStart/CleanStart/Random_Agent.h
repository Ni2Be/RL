#pragma once
#include "Agent.h"



class Random_Agent : public Agent
{
public:
	Random_Agent(std::shared_ptr<I_Observable_Environment> enviroment);
	void learn();
	void evaluate_action();
};