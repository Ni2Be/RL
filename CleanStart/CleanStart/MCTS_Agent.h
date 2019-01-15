#pragma once
#include "Agent.h"
#include "Agent.cpp"

#include "Environment.h"

namespace Ai_Arena
{
	template <class State_T>
	class MC_Node
	{
	public:
		MC_Node(State_T);
		~MC_Node();
		State_T state;
		float value;
		bool is_fully_expanded = false;
		int visits = 0;
		std::vector<MC_Node*> children;
		MC_Node* parent;
		//upper confidence bound
		MC_Node* uct() const;
		float m_c = 1.0f;
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


		//Tree
	public:
		void set_up_tree(const State_T& actual_state, const std::vector<Action>& possible_actions);
		void delete_tree();
		MC_Node<State_T>& select() const;
		void expand(MC_Node<State_T>&, const std::vector<Action>&);
		void simulate(MC_Node<State_T>&);
		void backpropagate(MC_Node<State_T>&);
	private:

		int node_height(MC_Node<State_T>& node) const;
		MC_Node<State_T>* root;
		int max_simulation_depth = 10;
		float discount_factor = 0.4;

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
