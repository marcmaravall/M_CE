#include "engine.h"

Engine::Engine()
{
	currentBoard = Board(START_FEN);
}

Engine::~Engine()
{

}

void Engine::init()
{
}

void Engine::RunTest()
{
	std::cout << "Running test..." << std::endl;
	currentBoard = Board(START_FEN);
	currentBoard.PrintBoard();
	std::cout << "FEN: " << currentBoard.ConvertToFEN() << std::endl;
}
