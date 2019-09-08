/*
I_Environment.h
	Das Interface I_Environment stellt alle Funktionen
	zur Verfügung damit ein Actor (vorallem ein Agent)
	mit dem Environment kommunizieren kann.

//ACTOR COMMUNICATION

std::vector<Action> possible_actions(std::shared_ptr<Actor>, State)
	Wird von planenden Agenten aufgerufen um die möglichen Aktionen
	für einen bestimmten State zu erfahren sollte alle Actions zurückgeben
	und bei den nicht ausführbaren die bool is_possible auf false stellen.
std::vector<Action> possible_actions(std::shared_ptr<Actor>)
	Wird von Actors aufgerufen um die aktuell möglichen Actionen zu erfahren
	sollte alle Actions zurückgeben und bei den nicht ausführbaren
	die bool is_possible auf false stellen.

std::vector<State> assume_action(std::shared_ptr<Actor>, State, Action)
	Wird von planenden Agenten aufgerufen um für einen State und eine Action
	den	daraus folgenden State zu erfahren.
	Sind nach einer Action mehrere Stats möglich werden alle übertragen.

void apply_action(std::shared_ptr<Actor>, Action)
	Wird von Actors aufgerufen um dem Environment mitzuteilen welche Action
	er ausführen will.

State current_state(std::shared_ptr<Actor>)
	Wird von Agenten aufgerufen um den aktuellen State des Environment zu erfahren.


Perception get_perception(std::shared_ptr<Actor>, Sensor)
	Wird von Agenten auferufen um die aktuelle Wahrnehmung zu erhalten.
	Der übertragene Sensor bestimmt welche wahrnemung zurückgeliefet wird.
	Ein neuer Sensor muss in der Types.h enum Sensor definiert werden.
Perception get_perception(std::shared_ptr<Actor>, Sensor, State)
	Wird von Agenten aufgerufen un die Wahrnehmung für einen vorgegebenen State
	zu erhalten.

////WORKINPROGRESS////
Perceptions get_perceptions(std::shared_ptr<Actor>, Sensor)
	Wird vom Agenten aufgerufen und versucht für possible_actions alle Wahrnehmungen
	zu erhalten.
Perceptions get_perceptions(std::shared_ptr<Actor>, Sensor, State)
	Wird von Agenten aufgerufen un die Wahrnehmung für einen vorgegebenen State
	zu erhalten.
////WORKINPROGRESS////

Reward reward(std::shared_ptr<Actor>, State)
	Wird von planenden Agenten aufgerufen um für einen State einen Reward
	zu bekommen.
Reward reward(std::shared_ptr<Actor>)
	Wird von Agenten aufgerufen um für den aktuellen State einen Reward
	zu bekommen.

bool is_final(std::shared_ptr<Actor>, State)
	Wird von planenenden Agenten aufgerufen um zu erfahren ob von einem State
	aus kein weiterer State mehr erreicht werden kann.

//ACTOR GENERAL

struct Actor_Representation
{
	std::shared_ptr<Actor>
	Action
	bool state_is_final
}
	Representiert den Actor im Environment. Es enthält daten die dem Actor nicht
	bekannt sind, die aber auf ihn zutreffen. Hier wird zu jedem Actor die Action
	gespeichert die im nächsten schritt ausgeführt wird. Außerdem wird hier vermerkt
	ob sich der Actor in einem final state befindet.

void add_actor(std::shared_ptr<Actor>)
	Sollte überschrieben werden damit dem environment ein actor
	hinzugefügt werden kann. Für jeden geaddeten Actor sollte
	eine steuerbare Entity erstellt werden etc.
	Danach sollte die Funktion
	add_actor(Actor_Representation, Action> actor_action)
	aufgerufen werden.

void add_actor(Actor_Representation, Action> actor_action)
	Fügt dem environment einen Actor hinzu. Jeder Actor erhält eine id die
	mit Actor.id() abgefragt werden kann.

std::vector<Actor_Representation, Action>> actors()
	Returnd threadsafe eine Kopie aller Actors mit den Aktionen die sie
	ausführen möchten.

void set_state_is_final(std::shared_ptr<Actor> actor, bool state_is_final)
	Elaubt es threadsafe den State des actors zu verändern.

Action exchange_action(std::shared_ptr<Actor> actor, Action new_action)
	Tauscht die aktuelle Aktion des Actors mit einer neuen aus und gibt die
	alte zurück.

int m_human_actors
int human_actors()
	Liefert die Anzahl an menschlichen Actors zurück.

int m_active_actors
int active_actors()
	Liefert die Anzahl an Actors von denen eine Action zu erwarten ist.
	Kann z.B. benutzt werden um den Actor schlafen zu schicken, falls die
	kontorllierte Entity des Actors nicht mehr existiert.

State m_environment_state
	Der State des Environment. Diese variable kann benutzt werden um den
	Zustand aller Actoren und des Environments in den gesamt State umzuwandeln.

mutable std::mutex m_state_lock
	Der zugehörige lock

void set_environment_state(const State& new_state)
	Erlaubt den State threadsafe zu setzen.

State environment_state()
	Returnd threadsafe eine Kopie des States.


//ACTOR THREAD HANDLING

mutable std::mutex m_execution_lock;
mutable std::condition_variable m_environment_condition;
mutable std::condition_variable m_actors_condition;
mutable int m_unexecuted_actions = 0;
	Diese Variablen sollen den Zeitablauf des Environments
	steuern. Die Implementation hängt davon ab ob es sich um
	zum Beispiel ein Rundenbasiertes-, oder ein
	diskretes Echtzeit-Environment handelt.
	Der m_execution_lock sollte sperren wenn gerade die
	Aktionen ausgeführt bzw. gesammelt werden.
	m_environment_condition soll dem Environment helfen zu warten
	bis alle Actors eine Aktion gewählt haben. Damit keine Aktion
	übersprungen werden kann.
	m_actors_condition sollte den Actors helfen zu warten bis
	ihre Aktion ausgeführt wurde.
	Damit kein Reward gesammelt wird bevor die Aktion ausgeführt
	wurde, bzw. damit die Berechnungen der nächsten aktion nicht
	auf einem veralteten State beruhen.
	Der Ablauf für ein diskretes Echtzeitenvironment das auf die
	Aktionen von Agenten wartet aber den human_actors erlaubt zu
	jeder Zeit ihre Aktion zu ändern, könnte so aussehen:

void update()
{
	std::unique_lock<std::mutex> lock(m_execution_lock);

	//wait for all Agents to place an action
	if (active_actors() - human_actors() > 0)
		m_environment_condition.wait(lock, [this]() {return m_unexecuted_actions == active_actors() - human_actors(); });

	execute_actions();

	//wakeup the actors
	m_actors_condition.notify_all();
	lock.unlock();
}

void apply_action(std::shared_ptr<Actor> actor, Action action)
{
	std::unique_lock<std::mutex> lock(m_execution_lock);

	//wait for last action to be executed
	if (!actor->is_human())
		m_actors_condition.wait(lock, [this]() {return m_unexecuted_actions < active_actors() - human_actors(); });

	//assign action to actor
	exchange_action(actor, action);

	if (!actor->is_human())
	{
		m_unexecuted_actions++;
		//notify the environment that all actors could be finished
		m_environment_condition.notify_all();
		lock.unlock();
	}
}

	-------------------------------------------------------------------------

//GENERAL

void run()
	Startet das environment, return erst wenn das environment die variable
	m_is_running auf false setzt.

void update()
	Die Funktion update wird einmal pro update_interval aufgerufen, sie
	sollte überschrieben werden und alles beinhalten was den gamestate
	verändert.

std::chrono::milliseconds m_update_interval
	Das Updateintervall ist	die Zeit die vergeht bevor die funktion update
	erneut aufgerufen wird.
	Der Environment thread ist in der Zwischenzeit nicht am schlafen, er
	sammelt weiterhin sf::Events und auch der thread der die graphic updated
	läuft weiter.

std::chrono::milliseconds update_interval()
	Gibt eine Referenz zum update interval zurück.

std::queue<sf::Event> m_events
	Queue die alle Eingaben beinhaltet die durch Tastatur, Maus etc.
	stattfinden. Die Events sollten nur durch diese Queue verarbeitet
	werden, da die standard sf::Event Verarbeitung nicht threadsafe ist.

std::mutex m_event_lock
	Der zugehörige lock.

std::queue<sf::Event> get_events();
	Returned die event queue. Alle events werden dabei aus der queue
	gelöscht. Wird diese Funktion aufgerufen sollten also alle Events
	verarbeitet werden.

void set_events(std::queue<sf::Event> events);
	Wird nur intern in der update loop aufgerufen um die events
	threadsafe zu updaten.
*/
#pragma once
#include <vector>
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include <numeric>
#include <algorithm>
#include <queue>

#include <SFML/Graphics.hpp>

#include "Types.h"
#include "Actor.h"

namespace Ai_Arena
{
	template <class State_T>
	class Actor;

	template <class State_T>
	class Environment
	{
	public:
		//ACTOR COMMUNICATION

			/*returns the possible actions in a given State*/
		virtual std::vector<Action> possible_actions(std::shared_ptr<Actor<State_T>>, State_T) const = 0;
		/*returns the possible actions in the current State*/
		virtual std::vector<Action> possible_actions(std::shared_ptr<Actor<State_T>> actor) const;

		/*returns all possible States that could result from a given action*/
		virtual std::vector<State_T> assume_action(std::shared_ptr<Actor<State_T>>, State_T, Action) const = 0;

		/*sets the action an actor wants to perform*/
		virtual void   apply_action(std::shared_ptr<Actor<State_T>>, Action) = 0;

		/*returns the current state of the enviroment*/
		//umbenennen current
		virtual State_T  current_state(std::shared_ptr<Actor<State_T>>) const = 0;


		/*returns the current perception of the enviroment*/
		virtual Perception get_perception(std::shared_ptr<Actor<State_T>>, Sensor) const = 0;
		/*returns the perception of the enviroment for a given state*/
		virtual Perception get_perception(std::shared_ptr<Actor<State_T>>, Sensor, State_T) const = 0;
		/*returns the current perception of the enviroment*/
	//	virtual Perceptions get_perceptions(std::shared_ptr<Actor<State_T>>, Sensor) const = 0;
		/*returns the perception of the enviroment for a given state*/
		//virtual Perceptions get_perceptions(std::shared_ptr<Actor<State_T>>, Sensor, State_T) const = 0;

		/*returns stats relatet to the game*/
		virtual std::vector<double> get_stats() = 0;

		/*returns the reward of a given state*/
		virtual Reward reward(std::shared_ptr<Actor<State_T>>, State_T) const = 0;
		/*returns the reward of a given state*/
		virtual Reward reward(std::shared_ptr<Actor<State_T>> actor);

		/*prooves if a state is a final state*/
		virtual bool   is_final(std::shared_ptr<Actor<State_T>>, State_T, bool is_simulation = false) const = 0;
		/*prooves if the current state is a final state*/
		virtual bool   is_final(std::shared_ptr<Actor<State_T>> actor, bool is_simulation = false) const;


		//ACTOR GENERAL

			/*adds an actor to the environment*/
		virtual void add_actor(std::shared_ptr<Actor<State_T>>) = 0;
		/*execute special options for debug purposes*/
		virtual void debug(std::shared_ptr<Actor<State_T>>) = 0;
		/*returns the number of humen actors*/
		const int human_actors() const;
		/*retruns the number of active actors*/
		const int& active_actors() const;

		//GENERAL

			/*starts the environment*/
		virtual void run() = 0;

		virtual void update() = 0;

		//GET/SET
			/*returns the update interval of the environment*/
		std::chrono::milliseconds& update_interval();
		std::chrono::milliseconds  update_interval() const;

		std::chrono::system_clock::time_point next_execution_time() const;
		void set_next_execution_time(std::chrono::system_clock::time_point time);
		std::chrono::system_clock::time_point m_next_execution_time;
		mutable std::mutex m_next_execution_time_lock;

		/*returns all Events like key/mouse input and emptys the queue*/
		virtual std::queue<sf::Event> get_events();
	protected:
		bool m_is_running = true;

		virtual void set_events(std::queue<sf::Event> events);

		/*the time interval the environments physics are updated*/
		std::chrono::milliseconds m_update_interval;
		mutable std::mutex m_actor_lock;
		mutable std::mutex m_human_actor_lock;
		std::thread m_environment_thread;

		mutable std::mutex m_execution_lock;
		mutable std::condition_variable m_environment_condition;
		mutable std::condition_variable m_actors_condition;
		mutable std::atomic<int> m_unexecuted_actions = 0;

		//represents the actor in the environment
		struct Actor_Representation
		{
			Actor_Representation(std::shared_ptr<Actor<State_T>> actor, Action action, bool state_is_final = false)
				: actor(actor), action(action), state_is_final(state_is_final) {}
			std::shared_ptr<Actor<State_T>> actor;
			Action action;
			bool state_is_final;
		};

		//GET/SET
		//THREADSAFE
		void set_environment_state(const State_T& new_state);
		//THREADSAFE
		const State_T environment_state() const;

		//THREADSAFE
		const std::vector<Actor_Representation> actors() const;
		void set_actor_state(std::shared_ptr<Actor<State_T>> actor, bool state_is_final);
		//THREADSAFE returns the old action
		Action exchange_action(std::shared_ptr<Actor<State_T>> actor, Action new_action);
		//THREADSAFE will count +1 to active_actors
		void add_actor(Actor_Representation actor_action);
		void debug(Actor_Representation actor_action);
		Environment() {};

	public:
		bool m_trainings_mode = false;

	private:
		std::queue<sf::Event> m_events;
		std::mutex            m_event_lock;

		mutable std::mutex m_state_lock;
		State_T m_environment_state;


		std::vector<Actor_Representation> m_actors;
		int m_human_actors;
	};
}