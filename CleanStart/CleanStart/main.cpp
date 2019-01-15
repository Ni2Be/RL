#include "Arena.h"


int main()
{
	using namespace Ai_Arena;
	Arena arena;
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