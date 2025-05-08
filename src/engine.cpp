#include "engine.h"

Engine::Engine()
{
	// currentBoard = Board(START_FEN);
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
	currentBoard = Board(TEST_FEN);

	while (true)
	{
		ManageInput();
	}
}

void Engine::ManageInput()
{
    std::string input;
    std::cin >> input;

    if (input.length() != 4 && input.length() != 5) {
        std::cerr << "" << std::endl;
        return;
    }

    if (input.length() == 3) {
        // castling
    }

    std::string fromStr = input.substr(0, 2);
    std::string toStr = input.substr(2, 2);

    int from = currentBoard.ConvertToIndexPosition(fromStr);
    int to = currentBoard.ConvertToIndexPosition(toStr);

    if (input.length() == 5) {
        int piecePromoting = -1;
        for (int i = 0; i < 12; i++) {
            if (PIECE_CHAR[i] == input[4]) {
                piecePromoting = i;
            }
        }

    }

    currentBoard.MovePiece(from, to);

    currentBoard.PrintBoard();
}

