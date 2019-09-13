#include "Arena.h"


int main(int argc, char *argv[])
{
	using namespace Ai_Arena;

	int humanPlayers = 0, MCTSAgents = 0, ReflexAgents = 0, RandomAgents = 0, TDAgents = 2, max_Score_Limit = 100000;

	int trainings_mode = 0;
	if (argc >= 3)
	{

		humanPlayers = std::stoi(argv[1]);
		MCTSAgents = std::stoi(argv[2]);
		ReflexAgents = std::stoi(argv[3]);
		RandomAgents = std::stoi(argv[4]);
		TDAgents = std::stoi(argv[5]);
		max_Score_Limit = std::stoi(argv[6]);
		//game = std::stoi(argv[3]);
	}

	Arena arena(humanPlayers, MCTSAgents, ReflexAgents, RandomAgents, TDAgents, max_Score_Limit);

	if (argc >= 6)
		trainings_mode = std::stoi(argv[6]);
	arena.trainings_mode() = trainings_mode;

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
