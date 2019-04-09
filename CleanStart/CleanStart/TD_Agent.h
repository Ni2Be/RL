#pragma once
#include "Agent.h"
#include "Agent.cpp"
#include "NeuralNetwork.h"

namespace Ai_Arena
{
	template <class State_T>
	class TD_Agent : public Agent<State_T>
	{
	public:
		TD_Agent(std::shared_ptr<Environment<State_T>> enviroment);
		void evaluate_action();
		int findMoveTD(std::vector<Action> possible_actions);
		int greedyMove(std::vector<std::vector<double>>& perceptions);
		void checkRewards();
		void stats();
		void debug();

		using Actor<State_T>::m_self_pointer;
		using Actor<State_T>::m_environment;
		using Actor<State_T>::start_actor_thread;
		using Actor<State_T>::is_sleeping;
		using Actor<State_T>::sleep;
		using Actor<State_T>::wake_up;
		using Actor<State_T>::id;
		using Actor<State_T>::is_human;
		using Actor<State_T>::m_id;
		using Actor<State_T>::m_is_sleeping;
		using Actor<State_T>::m_is_running;
		using Actor<State_T>::is_active;
		using Actor<State_T>::activate;
		using Actor<State_T>::deactivate;
		NeuralNetwork nn = NeuralNetwork(5, 12, 3);
		NeuralNetwork best_nn;
		bool firstRound = true;
		Perception lastPerception;
		float statReward;
		float statRewardMax;
		int maxStep = 1000;
		int step = 0;
		int totalStep = 0;
		int perceptionSizeMax = 0;
		int ballhits = 0;
		int ballmisses = 0;
		int ballhitsbest = 0;
		float hitmissratio = 0;
		float besthitmissratio = 0;

	};
}