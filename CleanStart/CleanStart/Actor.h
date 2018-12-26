/*
Actor.h
	Ein Actor ist alles was in einem Environment eigene
	Aktionen in Auftrag geben kann. Dabei kann es sich
	um eine Person oder einen Agenten handeln.
	Jeder Actor besitzt einen eigenen Thread der nachdem
	er gestartet wurde in einer Endlosschleife die
	Funktion evaluate_action() aufruft.


________________________________________________________________
ANLEITUNG:

!!ACHTUNG!!!!ACHTUNG!!!!ACHTUNG!!!!ACHTUNG!!!!ACHTUNG!!
	Es sollte nicht direkt von Actor geerbt werden!
	Stattdessen sollte die passende Klasse 
	Agent oder Human_Player
	verwendet werden.
!!ACHTUNG!!!!ACHTUNG!!!!ACHTUNG!!!!ACHTUNG!!!!ACHTUNG!!
	
	Sollte ein eigener Actor implementiert werden bietet
	es sich an dort im header die folgenden variablen und
	funktionen bekannt zu machen (einfach rein kopieren):
		
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

	Es bietet sich außerdem an Environment.h zu includen und
	einen State_T type (z.B. int) vorzugeben damit IntelliSense
	richtig arbeitet und man die environment funktionen nicht 
	von hand eingeben muss.
________________________________________________________________

void evaluate_action()
	Sollte überschrieben werden und alles beinhalten was
	zum berechnen der nächsten Aktion benötigt wird.
	Sie sollte die Funktion set_action aufrufen um die
	berechnete Action festzulegen.

void set_action()
	Legt die Action fest die dem Environment als nächstes
	mitgeteilt werden soll.

void apply_action()
	Wird automatisch nach jedem evaluate_action() ausgeführt
	und teilt dem Environment die Action mit die in m_next_action
	abgelegt wurde.


		std::shared_ptr<Actor> m_self_pointer;
		std::shared_ptr<Environment> m_environment;


std::mutex  m_actor_lock
std::thread m_actor_thread
std::condition_variable m_actor_condition
std::atomic<bool> m_is_sleeping 
std::atomic<bool> m_is_running
void start_actor_thread();
bool is_sleeping();
void sleep();
void wake_up();
	Diese Funktionen und Variabeln bieten eine Hilfestellung 
	um den Actor thread zu steuern.

void shut_down()
	beendet den Actor thread

std::shared_ptr<Actor> self()
	Gibt einen shared_ptr auf den Agenten zurück.

int id()
	Liefert die id des Agenten zurück. Die id sollte erst 
	abgerufen werden nachdem der actor einem Environment
	mit add_actor() hizugefügt wurde.

bool is_human()
	Gibt zurück ob der Actor ein human_actor ist.


std::shared_ptr<Actor> m_self_pointer;
	Ein shared_ptr auf den Actor. shared_ptr werden von
	vielen Ai_Arena Funktionen als Eingabeparameter gefordert
	durch diesen pointer muss nicht immer ein neuer erstellt 
	werden.

std::shared_ptr<Environment> m_environment;
	Ein shared_ptr auf das Environment in dem sich 
	der Actor befindet.
*/
#pragma once
#include "Types.h"
#include <memory>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <iostream>

namespace Ai_Arena
{
	template <class State_T>
	class Environment;

	template <class State_T>
	class Actor
	{
	public:
		Actor(std::shared_ptr<Environment<State_T>> environment);

		void start_actor_thread();
		bool is_sleeping();
		void sleep();
		void wake_up();

		bool is_active();
		void activate();
		void deactivate();

		virtual void shut_down() { m_is_running = false; };
		

		int& id() { return m_id; }
		const int& id() const { return m_id; }
		virtual bool is_human() = 0;
	protected:
		int    m_id;

		virtual void set_up() {};
		virtual void evaluate_action() = 0;


		std::shared_ptr<Actor> m_self_pointer;
		std::shared_ptr<Environment<State_T>> m_environment;

		std::mutex  m_actor_lock;
		std::thread m_actor_thread;
		std::condition_variable m_actor_condition;
		std::atomic<bool> m_is_sleeping = true;
		std::atomic<bool> m_is_active = true;
		std::atomic<bool> m_is_running = false;
	};
}


using namespace Ai_Arena;

template <class State_T>
Actor<State_T>::Actor(std::shared_ptr<Environment<State_T>> environment)
	:
	m_environment(environment),
	m_self_pointer(this)
{}


template <class State_T>
void Actor<State_T>::start_actor_thread()
{
	m_actor_thread = std::thread([this]() {
		m_is_running = true;

		set_up();
		//sleep();
		while (m_is_running)
		{
			//std::cout << "eval, actor: " << id() << std::endl;
			evaluate_action();
			//std::cout << "end eval, actor: " << id() << std::endl;
		}
	});
}

template <class State_T>
bool Actor<State_T>::is_sleeping()
{
	return m_is_sleeping; 
};

template <class State_T>
void Actor<State_T>::wake_up()
{
	std::unique_lock<std::mutex> lock(m_actor_lock);
	m_is_sleeping = false;
	//std::cout << std::endl << "wakeup, actor: " << id() << std::endl;
	m_actor_condition.notify_one();
	lock.unlock();
};


template <class State_T>
bool Actor<State_T>::is_active()
{
	return m_is_active;
};

template <class State_T>
void Actor<State_T>::activate()
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	m_is_active = true;
	//std::cout << std::endl << "active, actor: " << id() << std::endl;
};

template <class State_T>
void Actor<State_T>::deactivate()
{
	std::scoped_lock<std::mutex> lock(m_actor_lock);
	m_is_active = false;
	//std::cout << std::endl << "deactive, actor: " << id() << std::endl;
};

template <class State_T>
void Actor<State_T>::sleep()
{
	std::unique_lock<std::mutex> lock(m_actor_lock);
	m_is_sleeping = true;
	//std::cout << std::endl << "sleep" << std::endl;
	m_actor_condition.wait(lock);
	//std::cout << std::endl << "awake" << std::endl;
	lock.unlock();
};