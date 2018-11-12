#pragma once
#include "Actor.h"

class Agent : public Actor
{
public:
	Agent(std::shared_ptr<I_Observable_Environment> environment);
	virtual void evaluate_action() = 0;
protected:
};