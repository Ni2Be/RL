//#pragma once
//#include "MCTS_Agent.h"
//
//#include <iostream>
//#include <fstream>
//
//#include "Utility.h"
//
//using namespace Ai_Arena;
//
//
//template <class State_T>
//MCTS_Agent<State_T>::MCTS_Agent(std::shared_ptr<Environment<State_T>> enviroment)
//	:
//	Agent<State_T>::Agent(enviroment)
//{
//	load_settings();
//}
//
//
//template <class State_T>
//void MCTS_Agent<State_T>::set_up()
//{
//}
//
//template <class State_T>
//void MCTS_Agent<State_T>::load_settings()
//{
//	std::ifstream ifs;
//	ifs.open("MCTS_Settings.txt");
//	if (!ifs)
//	{
//		std::cerr << "could not load MCTS Agent\n";
//		exit(-1);
//	}
//	std::string consumer;
//	ifs >> consumer;
//	ifs >> max_simulation_depth;
//	ifs >> consumer;
//	ifs >> discount_factor;
//	ifs >> consumer;
//	ifs >> m_c;
//	ifs.close();
//	std::cout << "Load MCTS:"
//		<< "\nmax_simulation_depth: " << max_simulation_depth
//		<< "\ndiscount_factor: " << discount_factor
//		<< "\nc: " << m_c
//		<< "\n";
//}
//
//
//template <class State_T>
//void MCTS_Agent<State_T>::evaluate_action()
//{
//	if (!m_environment->is_final(m_self_pointer))
//	{
//		//Create tree
//		std::vector<Action> possible_actions =
//			m_environment->possible_actions(
//				m_self_pointer);
//		const auto state = m_environment->actual_state(m_self_pointer);
//
//		set_up_tree(state, possible_actions);
//
//
//		//use the time untill the next action will be executed to
//		//find a good action
//		Action next_action = possible_actions[0];
//		auto next_action_time = m_environment->next_execution_time();
//
//		int simulation_steps = 0;
//		while (std::chrono::system_clock::now() < next_action_time)
//		{
//			simulation_steps++;
//			next_action = select_action();
//		}
//		//DEBUG
//		std::cout << "simulation steps: " << simulation_steps << "\n";
//		simulated_steps += simulation_steps;
//		for (int i = 0; i < root->children.size(); i++)
//		{
//			char ch = 'n';
//			switch (root->children[i]->action.action) {
//			case 0: ch = 'u'; break;
//			case 1: ch = 'd'; break;
//			case 2: ch = 'r'; break;
//			case 3: ch = 'l'; break;
//			}
//			std::cout << "action " << root->children[i]->action.action << ": " << ch << ": " << ucb(root->children[i]) << "  |  ";
//			std::cout << "child: va: " << root->children[i]->value << ", vi: " << root->children[i]->visits << "\n";
//		}
//		std::cout << "selected: " << next_action.action << "\n----\n\n";
//		//ENDDEBUG
//
//		delete_tree();
//
//		//std::cout << "action: " << next_action.action << "\n\n\n";
//		m_environment->apply_action(m_self_pointer, next_action);
//		applied_actions++;
//	}
//	else
//	{
//		std::cout << "final\n";
//		deactivate();
//		sleep();
//	}
//}
//
//template <class State_T>
//Action MCTS_Agent<State_T>::select_action()
//{
//	std::vector<Action> possible_actions =
//		m_environment->possible_actions(
//			m_self_pointer);
//
//	//select best leaf
//	auto& best_leaf = select();
//
//	//expand best leaf
//	//TODO could support variable possible_actions.size()
//	expand(best_leaf, possible_actions);
//
//	//simulate all children
//	for (auto child : best_leaf.children)
//		simulate(*child);
//
//	//beackpropagate values
//	for (auto child : best_leaf.children)
//		backpropagate(*child);
//
//	//return the assumed best action
//	MC_Node<State_T>* best = best_child(this->root);
//	Action next_action = best->action;
//
//	return next_action;
//}
//
//template <class State_T>
//void MCTS_Agent<State_T>::shut_down()
//{
//	std::cout << "shut down\n";
//
//	std::ofstream ofs;
//	ofs.open("MCTSlog.txt", std::ios::app);
//	if (!ofs)
//	{
//		std::cerr << "could not open MCTSlog.txt";
//		exit(-1);
//	}
//
//	ofs << "\napplied actions: " << applied_actions;
//	ofs << "\nsimulated states: " << simulated_steps;
//	ofs.close();
//	m_is_running = false;
//}
//
//
////NODE
//template <class State_T>
//MC_Node<State_T>::MC_Node(State_T state, Action action)
//	:
//	state(state),
//	action(action)
//{
//}
//
//template <class State_T>
//MC_Node<State_T>::~MC_Node()
//{
//	for (auto child : children)
//		delete child;
//}
//
//
////TREE
//template <class State_T>
//void MCTS_Agent<State_T>::set_up_tree(const State_T& actual_state, const std::vector<Action>& possible_actions)
//{
//	//set the root Node
//	root = new MC_Node(actual_state, Action(0));
//	expand(*root, possible_actions);
//	//do one playthrought for each child
//	for (auto& child : root->children)
//		simulate(*child);
//}
//
//template <class State_T>
//void MCTS_Agent<State_T>::delete_tree()
//{
//	delete(root);
//}
//
//template <class State_T>
//MC_Node<State_T>& MCTS_Agent<State_T>::select() const
//{
//	//find the most promising leaf
//	MC_Node<State_T>* best_leaf = root;
//	while (best_leaf->is_fully_expanded
//		&& best_leaf->children.size() > 0)
//	{
//		best_leaf = best_child(best_leaf);
//	}
//	return *best_leaf;
//}
//
//template <class State_T>
//MC_Node<State_T>* MCTS_Agent<State_T>::best_child(MC_Node<State_T>* node) const
//{
//	if (node->children.size() == 0)
//		return node;
//	MC_Node<State_T>* best_child = node->children[0];
//	for (int i = 1; i < node->children.size(); i++)
//	{
//		if (ucb(node->children[i])
//			>= ucb(best_child))
//		{
//			//DEBUG
//			//std::cout << "best_child ucb: " << ucb(best_child) << "\n";
//			best_child = node->children[i];
//		}
//
//	}
//	//std::cout << "\n";
//	return best_child;
//}
//
//template <class State_T>
//double MCTS_Agent<State_T>::ucb(MC_Node<State_T>* child) const
//{
//	double partent_vi = static_cast<double>(child->parent->visits);
//	double child_vi = static_cast<double>(child->visits);
//
//	return (child->value / child_vi) + (m_c * std::sqrt(std::log(partent_vi) / child_vi));
//}
//
//
//template <class State_T>
//void MCTS_Agent<State_T>::expand(MC_Node<State_T>& leaf, const std::vector<Action>& possible_actions)
//{
//	//make a new child for every possible Action that does not lead to a final state
//	for (int i = 0; i < possible_actions.size(); i++)
//	{
//		auto possible_child_states = m_environment->assume_action(m_self_pointer, leaf.state, possible_actions[i]);
//		if (!m_environment->is_final(m_self_pointer, possible_child_states[0], true)
//			&& (m_environment->reward(m_self_pointer, possible_child_states[0]) >= 0)) //TEST SNAKE
//		{
//			leaf.children.push_back(new MC_Node<State_T>(possible_child_states[0], possible_actions[i]));
//			leaf.children.back()->parent = &leaf;
//			leaf.children.back()->height = leaf.children.back()->parent->height + 1;
//			leaf.children.back()->value = m_environment->reward(m_self_pointer, possible_child_states[0])
//				* std::pow(discount_factor, leaf.children.back()->height);
//			//leaf.children.back()->value += leaf.children.back()->parent->value;//TODO nur für snake
//
//		}
//	}
//	leaf.is_fully_expanded = true;
//}
//
//
//template <class State_T>
//void MCTS_Agent<State_T>::simulate(MC_Node<State_T>& child)
//{
//	State_T child_state = child.state;
//	int simulation_step = 0;
//
//	//simulates a playthrought until a final state
//	//or the max_simulation_depth is reached
//	while (!m_environment->is_final(m_self_pointer, child_state, true)
//		&& max_simulation_depth > simulation_step)
//	{
//		std::vector<Action> possible_actions = m_environment->possible_actions(m_self_pointer);
//		int  random_action_index = Utility::random_int_ts(0, possible_actions.size() - 1);
//		auto possible_child_states = m_environment->assume_action(m_self_pointer, child_state, possible_actions[random_action_index]);
//		child_state = possible_child_states[0];
//		//check reward and update value
//		child.value += m_environment->reward(m_self_pointer, child_state)
//			* std::pow(discount_factor, simulation_step + child.height);
//		simulation_step++;
//
//		//TEST SNAKE STOP WHEN HITTING WALL
//		//if (m_environment->reward(m_self_pointer, child_state) < 0)
//		//{
//		//	child.value -= 3;
//		//	break;
//		//}
//		//END TEST SNAKE STOP WHEN HITTING WALL
//
//	}
//}
//
//template <class State_T>
//void MCTS_Agent<State_T>::backpropagate(MC_Node<State_T>& child)
//{
//	double leaf_value = child.value;
//	auto node = &child;
//	node->visits++;
//	while (node != root)
//	{
//		node->parent->visits++;
//		//node->parent->value += leaf_value;//TODO klappt besser ohne
//		node = node->parent;
//	}
//}
