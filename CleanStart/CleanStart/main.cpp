#include "Eight_Puzzle.h"
#include "Random_Agent.h"

int main()
{
	Eight_Puzzle game(4, 4, 90);
	//TODO make Arena that holds agents and games
	Random_Agent rand_agent(game);

	game.run();

	return 0;
}