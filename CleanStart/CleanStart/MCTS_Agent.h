#pragma once
#include "Agent.h"
#include "Agent.cpp"

#include "Environment.h"

#include <cmath>
namespace Ai_Arena
{
	template <class State_T>
	class MC_Node
	{
	public:
		MC_Node(State_T, Action);
		~MC_Node();
		State_T state;
		Action action;
		double value = 0.01;
		bool   is_fully_expanded = false;
		int    visits = 1;
		int    height = 1;
		bool   is_final;
		std::vector<MC_Node*> children;
		MC_Node* parent;
	};

	template <class State_T>
	class MCTS_Agent : public Agent<State_T>
	{
	public:
		MCTS_Agent(std::shared_ptr<Environment<State_T>> enviroment);
		void evaluate_action();
		void set_up();
		void shut_down();

		
	private:
		Action select_action();

		//Monitor
		int simulated_steps;
		int simulated_states = 0;
		int simulated_playouts = 0;
		int applied_actions;

		//Tree
	public:
		void set_up_tree(const State_T& current_state, const std::vector<Action>& possible_actions);
		void delete_tree();

		MC_Node<State_T>* best_child(MC_Node<State_T>*) const;
		double ucb(MC_Node<State_T>* child) const;

		MC_Node<State_T>& select() const;
		void expand       (MC_Node<State_T>&, const std::vector<Action>&);
		void simulate     (MC_Node<State_T>&);
		void backpropagate(MC_Node<State_T>&);
	private:
		void load_settings();

		MC_Node<State_T>* root;
		int   max_simulation_depth;
		int   max_tree_height = 100;//not used atm
		double m_c = 2;
		double discount_factor;
		//parent template
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
	};
}
