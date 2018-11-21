#include "Multi_Snake.h"
#include "Actor.h"
#include "Snake_World.h"
using namespace Ai_Arena;
using namespace Snake;


Multi_Snake::Multi_Snake(int fields, int field_pixel)
	:
	m_fields(fields),
	world(fields)
{
	graphics(
		std::shared_ptr<Multi_Snake_Graphics>(
			new Multi_Snake_Graphics(
				fields,
				"Snake",
				field_pixel)));


	m_graphics->update_graphics(world);

	//set_environment_state(convert_to_state(***));
}

void Multi_Snake::update_world()
{
	std::unique_lock<std::mutex> lock(m_execution_lock);
	//wait for all actors to place an action
	if (actors().size() - human_actors() > 0)
		m_environment_condition.wait(lock, [this]() {return m_unexecuted_actions == actors().size() - human_actors(); });


	//TODO
	//
	execute_actions();


	//wakeup the actors
	m_actors_condition.notify_all();
	lock.unlock();


}

void Multi_Snake::set_up()
{
	for (auto& actor : actors())
		actor.first->wake_up();

	//TODO
	//spielfeld aufbauen
	
	//	schlangen plazieren

	//	apfel ein erstes mal spornen
}


void Multi_Snake::add_actor(std::shared_ptr<Actor> actor)
{
	I_Environment::add_actor(std::pair<std::shared_ptr<Actor>, Action>(actor, { Actions::NO_ACTION }));
	world.add_snake();
}

void Multi_Snake::execute_actions()
{
	//execute all actions
	for (auto actor_action_pair : actors())
	{
		//the actor Id is allways the position of the snake in this arry
		Snake_Entity* controlled_snake = &(world.snakes[actor_action_pair.first->id()]);
		controlled_snake->perform_action(actor_action_pair.second.action);

		if(!actor_action_pair.first->is_human())
			m_unexecuted_actions--;
	}

	//handle events like ate apple
	world.check_events();


	//TODO
	//set_environment_state(convert_to_state(***));
}

void Multi_Snake::update()
{
	update_world();

	//only update graphics in play mode
	//if(needed)
	graphics()->update_graphics(world);
}



//I_ENVIRONMENT
std::vector<Action> Multi_Snake::possible_actions(std::shared_ptr<Actor>, State) const
{
	return { 
		Actions::U, 
		Actions::D, 
		Actions::L, 
		Actions::R, 
		Actions::NO_ACTION };
}

void Multi_Snake::apply_action(std::shared_ptr<Actor> actor, Action action)
{
	std::unique_lock<std::mutex> lock(m_execution_lock);
	if (!actor->is_human())
	{
		//wait for last action to be executed
		m_actors_condition.wait(lock, [this]() {return m_unexecuted_actions < actors().size() - human_actors(); });
	}

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