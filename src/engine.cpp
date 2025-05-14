#include "engine.h"
#include "board.h"

Engine::Engine()
{
    eval = Evaluation();
	// currentBoard = Board(START_FEN);
}

Engine::~Engine()
{

}

void Engine::init()
{
	InitKnightMasks();
}

void Engine::RunTest()
{
    init();

	std::cout << "Running test..." << std::endl;
	currentBoard = Board(START_FEN);
	Utils::PrintBoard(currentBoard);


	while (true)
	{
		ManageInput();
	}
}

void Engine::ManageInput()
{
    std::string input;
    std::cin >> input;

    if (input == "0-0")
    {
        currentBoard.Castle(true);
        Utils::PrintBoard(currentBoard);
    }
    else if (input == "0-0-0")
    {
        currentBoard.Castle(false);
        Utils::PrintBoard(currentBoard);
    }

    if (input.length() != 4 && input.length() != 5) {
        std::cerr << "" << std::endl;
        return;
    }

    if (input.length() == 3) {
        // castling
    }

    std::string fromStr = input.substr(0, 2);
    std::string toStr = input.substr(2, 2);

    int from =  Utils::ConvertToIndexPosition(fromStr);
    int to =    Utils::ConvertToIndexPosition(toStr);

    if (input.length() == 5) {
        int piecePromoting = -1;
        for (int i = 0; i < 12; i++) {
            if (PIECE_CHAR[i] == input[4]) {
                piecePromoting = i;
				currentBoard.Promotion(from, to, piecePromoting);
            }
        }

    }
	else if (input.length() == 4) {
		currentBoard.MovePiece(from, to);
	}

    Utils::PrintBoard(currentBoard);
    std::cout << "Evaluation: " << eval.Evaluate(currentBoard) << "\n";
}

void Engine::InitKnightMasks() {
    for (int square = 0; square < 64; ++square) {
        uint64_t mask = 0ULL;
        int rank = square / 8;
        int file = square % 8;

        int dr[8] = { -2, -1, 1, 2, 2, 1, -1, -2 };
        int df[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

        for (int i = 0; i < 8; ++i) {
            int r = rank + dr[i];
            int f = file + df[i];
            if (r >= 0 && r < 8 && f >= 0 && f < 8) {
                int destination = r * 8 + f;
                mask |= (1ULL << destination);
            }
        }

        knightMasks[square] = mask;
    }
}


uint64_t Engine::knightMasks[64];

