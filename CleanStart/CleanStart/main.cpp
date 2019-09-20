#include "Arena.h"


int main(int argc, char *argv[])
{
	using namespace Ai_Arena;

	int humanPlayers = 0, MCTSAgents = 0, ReflexAgents = 0, RandomAgents = 0, TDAgents = 1, max_Score_Limit = 100000, console_output = 1, game_speed = 0;


	if (argc >= 3)
	{

		humanPlayers = std::stoi(argv[1]);
		MCTSAgents = std::stoi(argv[2]);
		ReflexAgents = std::stoi(argv[3]);
		RandomAgents = std::stoi(argv[4]);
		TDAgents = std::stoi(argv[5]);
		max_Score_Limit = std::stoi(argv[6]);
		console_output = std::stoi(argv[7]);
		game_speed = std::stoi(argv[8]);
		//game = std::stoi(argv[3]);
	}

	Arena arena(humanPlayers, MCTSAgents, ReflexAgents, RandomAgents, TDAgents, max_Score_Limit, game_speed);


	if (console_output == 0)
	{
		std::cout.setstate(std::ios_base::failbit);
	}


	try
	{
		arena.run();
	}
	catch (std::exception& e)
	{
		std::cout << "exeption!\n";
		std::cerr << e.what();
	}
	return 0;
}
