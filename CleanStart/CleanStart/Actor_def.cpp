//#pragma once
#include "Actor_def.h"
//
//#include <iostream>
//
//using namespace Ai_Arena;
//
//template <class State_T>
//Actor<State_T>::Actor(std::shared_ptr<Environment<State_T>> environment)
//	:
//	m_environment(environment),
//	m_self_pointer(this)
//{}
//
////TODO 
//template <class State_T>
//void Actor<State_T>::start_actor_thread()
//{
//	m_actor_thread = std::thread([this]() {
//		m_is_running = true;
//		m_is_sleeping = false;
//		while (m_is_running)
//		{
//			evaluate_action();
//			m_self_pointer->evaluate_action();
//			m_self_pointer->apply_action();
//		}
//	});
//}
//
//template <class State_T>
//void Actor<State_T>::set_action(Action action)
//{
//	m_next_action = action;
//}
//
//template <class State_T>
//void Actor<State_T>::apply_action()
//{
//	m_environment->apply_action(m_self_pointer, m_next_action);
//}
//
//template <class State_T>
//bool Actor<State_T>::is_sleeping()
//{
//	return m_is_sleeping; 
//};
//
//template <class State_T>
//void Actor<State_T>::wake_up()
//{
//	std::unique_lock<std::mutex> lock(m_actor_lock);
//	m_is_sleeping = false;
//	//std::cout << std::endl << "wakeup" << std::endl;
//	m_actor_condition.notify_one();
//	lock.unlock();
//};
//
//template <class State_T>
//void Actor<State_T>::sleep()
//{
//	std::unique_lock<std::mutex> lock(m_actor_lock);
//	m_is_sleeping = true;
//	//std::cout << std::endl << "sleep" << std::endl;
//	m_actor_condition.wait(lock);
//	//std::cout << std::endl << "awake" << std::endl;
//	lock.unlock();
//};