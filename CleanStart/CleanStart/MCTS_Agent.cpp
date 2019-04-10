#pragma once
#include "MCTS_Agent.h"

#include <iostream>
#include <fstream>

#include "Utility.h"

using namespace Ai_Arena;


template <class State_T>
MCTS_Agent<State_T>::MCTS_Agent(std::shared_ptr<Environment<State_T>> enviroment)
	:
	Agent<State_T>::Agent(enviroment)
{
	load_settings();
}


template <class State_T>
void MCTS_Agent<State_T>::set_up()
{
}

template <class State_T>
void MCTS_Agent<State_T>::load_settings()
{
	std::ifstream ifs;
	ifs.open("MCTS_Settings.txt");
	if (!ifs)
	{
		std::cerr << "could not load MCTS Agent\n";
		exit(-1);
	}
	std::string consumer;
	ifs >> consumer;
	ifs >> max_simulation_depth;
	ifs >> consumer;
	ifs >> discount_factor;
	ifs >> consumer;
	ifs >> m_c;
	ifs.close();
	std::cout << "Load MCTS:"
		<< "\nmax_simulation_depth: " << max_simulation_depth
		<< "\ndiscount_factor: " << discount_factor
		<< "\nc: " << m_c << "\n";
}


template <class State_T>
void MCTS_Agent<State_T>::evaluate_action()
{
	if (!m_environment->is_final(m_self_pointer))
	{
		//Create tree
		std::vector<Action> possible_actions =
			m_environment->possible_actions(
				m_self_pointer);
		const auto state = m_environment->actual_state(m_self_pointer);

		set_up_tree(state, possible_actions);

		
		//use the time untill the next action will be executed to
		//find a good action
		Action next_action = 0;
		auto next_action_time = m_environment->next_execution_time();

		int simulation_steps = 0;
		while (std::chrono::system_clock::now() < next_action_time)
		{
			simulation_steps++;
			next_action = select_action();
		}
		//DEBUG
		std::cout << "simulation steps: " << simulation_steps << "\n";
		simulated_steps += simulation_steps;
		for (int i = 0; i < root->children.size(); i++)
		{
			char ch = 'n';
			switch (i) {
			case 0: ch = 'u'; break;
			case 1: ch = 'd'; break;
			case 2: ch = 'l'; break;
			case 3: ch = 'r'; break;}
			std::cout << "action " << i << ": " << ch << ": " << root->children[i]->value << "\n";
		}
		std::cout << "selected: " << next_action.action << "\n----\n\n";
		//ENDDEBUG

		delete_tree();

		//std::cout << "action: " << next_action.action << "\n\n\n";
		m_environment->apply_action(m_self_pointer, next_action);
		applied_actions++;
	}
	else
	{
		std::cout << "final\n";
		deactivate();
		sleep();
	}
}

template <class State_T>
Action MCTS_Agent<State_T>::select_action()
{
	std::vector<Action> possible_actions =
		m_environment->possible_actions(
			m_self_pointer);

	//select best leaf
	auto& best_leaf = select();
	
	//expand best leaf
	//TODO could support variable possible_actions.size()
	expand(best_leaf, possible_actions);

	//simulate all children
	for (auto child : best_leaf.children)
		simulate(*child);
	
	//beackpropagate values
	for (auto child : best_leaf.children)
		backpropagate(*child);
	
	//DEBUG

	//for (auto child : root->children)
	//	std::cout << "value: " << child->value << ", visits: " << child->visits << ", uct: " << MC_Node<State_T>::uct(root->visits, child->visits, child->value, child->m_c) << "\n";
	//std::cout << "\n";
	//ENDDEBUG

	//TODO implement function
	//return the assumed best action
	int next_action_index = 0;
	auto best_child = root->children[0];
	for (int i = 0; i < root->children.size(); i++)
	{
		if (root->children[i]->value > best_child->value)
		{
			best_child = root->children[i];
			next_action_index = i;
		}
	}
	//std::cout << "seldected: " << next_action_index << "\n";
	

	return possible_actions[next_action_index];
}

template <class State_T>
void MCTS_Agent<State_T>::shut_down()
{
	std::cout << "shut down\n";

	std::ofstream ofs;
	ofs.open("MCTSlog.txt", std::ios::app);
	if (!ofs)
	{
		std::cerr << "could not open MCTSlog.txt";
		exit(-1);
	}
	
	ofs << "\napplied actions: " << applied_actions;
	ofs << "\nsimulated states: " << simulated_steps;
	ofs.close();
	m_is_running = false;
}

	
//NODE
template <class State_T>
MC_Node<State_T>::MC_Node(State_T state, float c)
	:
	state(state),
	m_c(c)
{
}

template <class State_T>
MC_Node<State_T>::~MC_Node()
{
	for (auto child : children)
		delete child;
}

template <class State_T>
MC_Node<State_T>* MC_Node<State_T>::uct_child() const
{
	if (children.size() <= 0)
	{
		std::cerr << "MC_Node best_child(), children size <= 0!\n";
		exit(-1);
	}
	//for all visited childs 
	MC_Node* best_path = children[0];
	float best_path_uct = uct(this->visits, best_path->visits, best_path->value, best_path->m_c);
	for (auto child : children)
	{
		float child_uct = uct(this->visits, child->visits, child->value, m_c);

		if (child_uct > best_path_uct)
		{
			best_path = child;
			best_path_uct = child_uct;
		}
	}
	return best_path;
}

template <class State_T>
float MC_Node<State_T>::uct(int parent_visits, int child_visits, float child_value, float c)
{
	float partent_vi = static_cast<float>(parent_visits);
	float child_vi = static_cast<float>(child_visits);

	return (child_value / child_vi) + (c * std::sqrt(std::logf(partent_vi) / child_vi));
}

//TREE
template <class State_T>
void MCTS_Agent<State_T>::set_up_tree(const State_T& actual_state, const std::vector<Action>& possible_actions)
{
	//set the root Node
	root = new MC_Node(actual_state, m_c);
	expand(*root, possible_actions);
	root->visits++;
	for (auto& child : root->children)
		simulate(*child);

}

template <class State_T>
void MCTS_Agent<State_T>::delete_tree()
{
	delete(root);
}

template <class State_T>
MC_Node<State_T>& MCTS_Agent<State_T>::select() const
{
	//find the leaf with the highest uct that is not fully expanded
	MC_Node<State_T>* best_leaf = root;
	int i = 0;
	while (best_leaf->is_fully_expanded)
	{
		best_leaf = best_leaf->uct_child();
		i++;
	}
	return *best_leaf;
}


template <class State_T>
void MCTS_Agent<State_T>::expand(MC_Node<State_T>& leaf, const std::vector<Action>& possible_actions)
{
	//make a new child for every possible Action and set the childs state
	for (int i = 0; i < possible_actions.size(); i++)
	{
		auto possible_child_states = m_environment->assume_action(m_self_pointer, leaf.state, possible_actions[i]);
		leaf.children.push_back(new MC_Node<State_T>(possible_child_states[0], m_c));
		leaf.children.back()->parent = &leaf;
	}
	leaf.is_fully_expanded = true;
}


template <class State_T>
void MCTS_Agent<State_T>::simulate(MC_Node<State_T>& child)
{
	//simulates a playthrought for one child until a final state
	//or the max_simulation_depth is reached
	
	int simulation_step = 0;
	float child_value = 0.0f;
	auto child_state = child.state;
	auto child_height = node_height(child);
	//randomly choose one action
	//TODO could support variable possible_actions.size()
	std::vector<Action> possible_actions =
		m_environment->possible_actions(m_self_pointer);
	while ((simulation_step++ < max_simulation_depth)
		&& !m_environment->is_final(m_self_pointer, child_state, true))
	{
		//play random moves from the childs state
		int random_action_index = Utility::random_int_ts(0, possible_actions.size() - 1);
		auto possible_child_states = m_environment->assume_action(m_self_pointer, child_state, possible_actions[random_action_index]);
		//check reward and update value
		child_value += m_environment->reward(m_self_pointer, possible_child_states[0]) * std::pow(discount_factor, simulation_step + child_height);
		child_state = possible_child_states[0];
	}
	//set child value
	child_value /= static_cast<float>(simulation_step);
	child.value = child_value + child.parent->value;
	child.visits += 1;
}

template <class State_T>
void MCTS_Agent<State_T>::backpropagate(MC_Node<State_T>& child)
{
	//backpropagate the childs value weighted
	MC_Node<State_T>* node = &child;
	if (&child == nullptr)
	{
		std::cout << "node is null!\n";
		return;
	}
	while (node->parent != root)
	{
		//add childs value to parents value and count up parents visits
		node->parent->value += node->value;
		node->parent->visits++;
		//repeat
		node = node->parent;
	}
	root->visits++;
}

template <class State_T>
int MCTS_Agent<State_T>::node_height(MC_Node<State_T>& node) const
{
	MC_Node<State_T>* node_ptr = &node;
	int height = 1;
	if (node_ptr = root)
		return 0;
	while (node_ptr->parent != root)
	{
		height++;
		node_ptr = node_ptr->parent;
	}
	return height;
}