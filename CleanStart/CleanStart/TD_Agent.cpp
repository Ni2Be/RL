#pragma once
#include "TD_Agent.h"


#include <iostream>

#include "Utility.h"

using namespace Ai_Arena;

template <class State_T>
TD_Agent<State_T>::TD_Agent(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Agent<State_T>::Agent(enviroment)
{}

template <class State_T>
void TD_Agent<State_T>::evaluate_action()
{
	if (!Agent<State_T>::m_environment->is_final(Agent<State_T>::m_self_pointer))
	{

		debug();
		
		std::vector<Action> possible_actions =
			Agent<State_T>::m_environment->possible_actions(
				Agent<State_T>::m_self_pointer);
		
		

		int moveIndex = findMoveTD(possible_actions);
		//std::cout << "super" << std::endl;
		//Perception test = m_environment->get_perception(m_self_pointer, Sensor::OWN_POS_AND_BALL, m_environment->actual_state(m_self_pointer));
		//std::cout << test.size() << std::endl;
		


		//std::cout << moveIndex << std::endl;
		Agent<State_T>::m_environment->apply_action(Agent<State_T>::m_self_pointer, possible_actions[moveIndex]);
		checkRewards();
		stats();
	}
	else
	{
		std::cout << "actor " << Agent<State_T>::id() << " deactivated\n";
		deactivate();
		sleep();
	}

}



template <class State_T>
int TD_Agent<State_T>::findMoveTD(std::vector<Action> possible_actions)
{
	Perceptions perceptions;
	const auto state = m_environment->actual_state(m_self_pointer);

	for (int i = 0; i < possible_actions.size(); i++)
	{
		auto possible_state = m_environment->assume_action(m_self_pointer, state, possible_actions[i]);

		perceptions.push_back(m_environment->get_perception(m_self_pointer, Sensor::OWN_POS_AND_BALL, possible_state[0]));
	}



	if (firstRound)
	{

		int nextPerception = greedyMove(perceptions);
		lastPerception = perceptions[nextPerception];
		firstRound = false;
		return nextPerception;
	}

	if ((rand() % 10) == 0)
	{
		int perceptionIndex = rand() % 3;
		lastPerception = perceptions[perceptionIndex];
		return perceptionIndex;
	}
	int nextPerception = greedyMove(perceptions);
	double errorSignal = 0.9 * ((nn.getValue(perceptions[nextPerception]) - nn.getValue(lastPerception)));
	nn.backProb(errorSignal, 0.1);
	lastPerception = perceptions[nextPerception];
	return nextPerception;


}
template <class State_T>
int TD_Agent<State_T>::greedyMove(std::vector<std::vector<double>>& perceptions)
{
	int index = 0;

	double maxValue = nn.getValue(perceptions[index]);
	for (int i = 1; i < 3; i++)
	{
		//std::cout << "i: " << i << std::endl;
		if (maxValue < nn.getValue(perceptions[i]))
		{
			maxValue = nn.getValue(perceptions[i]);
			index = i;
		}
	}
	return index;
}

template <class State_T>
void TD_Agent<State_T>::checkRewards()
{
	const auto state = m_environment->actual_state(m_self_pointer);
	Reward reward = m_environment->reward(m_self_pointer, state);
	double eSignal = reward - nn.getValue(lastPerception);
	nn.backProb(eSignal, 0.1);
	statReward += reward;
}

template <class State_T>
void TD_Agent<State_T>::stats()
{
	if (lastPerception.size() > perceptionSizeMax)
		perceptionSizeMax = lastPerception.size();
	
	
	

	if (maxStep == step)
	{
		std::vector<double> stats = m_environment->get_stats();
		ballhits = stats[0];
		ballmisses = stats[1];

		if (ballhits > ballhitsbest)
			ballhitsbest = ballhits;
		hitmissratio = ((float)ballhits / (float)ballmisses );
		if (hitmissratio > besthitmissratio)
			besthitmissratio = hitmissratio;

		totalStep += maxStep;
		std::cout << "Total Steps: " << totalStep << "-------------" << std::endl;
		std::cout << "Stats: " << std::endl;
		std::cout << "Reward:" << statReward << std::endl;

//		std::cout << "Y Front: " << lastPerception[0] << std::endl;
//		std::cout << "Y Back: " << lastPerception[1] << std::endl;
//		std::cout << "Y Ball: " << lastPerception[2] << std::endl;
		for (int i = 0; lastPerception.size() > i; i++)
		{
			std::cout << "Input " << i << ": "<<lastPerception[i] << std::endl;
		}
		std::cout << "Perception Size: " << lastPerception.size() << std::endl;
		std::cout << "Perception Size Max: " << perceptionSizeMax << std::endl;
		std::cout << "Ball hits: " << ballhits << std::endl;
		std::cout << "Ball misses: " << ballmisses << std::endl;
		std::cout << "Ball hits best round: " << ballhitsbest << std::endl;
		std::cout << "Hit Miss Ratio: "  << hitmissratio << std::endl;
		std::cout << "Best Hit Miss Ratio: " << besthitmissratio << std::endl;
		std::cout << "------" << "-------Total Steps: " << totalStep << std::endl;
		std::cout << std::endl;

		statReward = 0;
		step = 0;
		
	}
	step++;
	
	
	
}

template <class State_T>
void TD_Agent<State_T>::debug()
{
		//using namespace Pong;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{

				m_environment->debug(m_self_pointer);
				std::cout << "Toggle Mode" << std::endl;

		}

}