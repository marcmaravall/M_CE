#include "engine.h"
#include "board.h"

Engine::Engine()
{
	// currentBoard = Board(START_FEN);
}

Engine::~Engine()
{

}

void Engine::init()
{
	GenerateZobristHash(rand());
	InitKnightMasks();
    InitKingMasks();

    std::ofstream debugFile("C:\\Users\\Marc\\source\\repos\\M_CE\\tests", std::ios::app);
    debugFile.clear();
}

void Engine::RunTest()
{
    init();

	std::cout << "Running test..." << std::endl;
	currentBoard = Board(TEST_FEN);
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

    Move current = {
        .from = 255, .to = 255, .castling = false, .mode = 0, .check = false, .checkmate = false,
    };

    if (input == "0-0")
    {
        current.castling = true;
        current.mode = true;

        currentBoard.Castle(current);
        Utils::PrintBoard(currentBoard);
    }
    else if (input == "0-0-0")
    {
        current.castling = true;
        current.mode = false;

        currentBoard.Castle(current);
        Utils::PrintBoard(currentBoard);
    }

    if (input.length() != 4 && input.length() != 5) {
        std::cerr << "" << std::endl;
        return;
    }

    std::string fromStr = input.substr(0, 2);
    std::string toStr = input.substr(2, 2);

    current.from =  Utils::ConvertToIndexPosition(fromStr);
    current.to =    Utils::ConvertToIndexPosition(toStr);

    current.promotion = -1;

    if (input.length() == 5) {
        for (int i = 0; i < 12; i++) {
            if (PIECE_CHAR[i] == input[4]) {
                current.promotion = i;
                currentBoard.Promotion(current);
                break;
            }
        }

        if (current.promotion == -1) {
            std::cerr << "Invalid promotion: " << input[4] << std::endl;
            return;
        }
    }

	else if (input.length() == 4) {
		currentBoard.MovePiece(current);
	}

    Utils::PrintBoard(currentBoard);
    std::cout << "Evaluation: " << Evaluation::Evaluate(currentBoard) << "\n";
    // std::cout << "TEST: " << currentBoard.IsCheck(currentBoard.turn == !WHITE_TURN? currentBoard.GetWhiteKingPosition() : currentBoard.GetBlackKingPosition()) << "\n";
	// MoveEval bestMove = Minimax(currentBoard, 0, currentBoard.turn == WHITE_TURN);
	// std::cout << "Best move: " << Utils::ConvertToBoardPosition(bestMove.move.from) << Utils::ConvertToBoardPosition(bestMove.move.to) << "\n";
}

void Engine::PlayAgainistItself()
{
    init();

    std::cout << "Playing vs it..." << std::endl;
    currentBoard = Board(TEST_FEN);
    Utils::PrintBoard(currentBoard);
    int i = 0;
    while (i < 10)
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        
		NODES = 0;
        ALPHA_BETA_PRUNINGS = 0;

        std::cout << "Perft: " << Perft(currentBoard, maxSearchDepth) << "\n\n";
        std::cout << "Divide: " << Perft(currentBoard, 1);

        /*MoveEval bestMove = AlphaBeta(currentBoard, maxSearchDepth, -1000000, 1000000, currentBoard.turn == WHITE_TURN);
        std::cout << "Best move: " << Utils::ConvertToBoardPosition(bestMove.move.from) << Utils::ConvertToBoardPosition(bestMove.move.to) << "\n";
        currentBoard.MovePiece(bestMove.move);
        Utils::PrintBoard(currentBoard);

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
		std::cout   << "Elapsed time: " << elapsed.count() << " seconds\n"
		            << "Nodes searched: " << NODES << "\n" 
                    << "NPS: " << NODES / elapsed.count() << "\n" 
                    << "Prunings: " << ALPHA_BETA_PRUNINGS << "\n";*/

        i++;
    }
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

void Engine::PlayAgainistHuman()
{
    init();
    currentBoard = Board(START_FEN);
    Utils::PrintBoard(currentBoard);
    while (true)
    {
        if (currentBoard.turn == WHITE_TURN)
        {
            std::string input;
            std::cout << "Move: ";
            std::cin >> input;

            Move current = {
                .from = 255, .to = 255, .castling = false, .mode = 0, .check = false, .checkmate = false,
            };

            if (input == "0-0")
            {
                current.castling = true;
                current.mode = true;
                currentBoard.Castle(current);
                Utils::PrintBoard(currentBoard);
                continue;
            }
            else if (input == "0-0-0")
            {
                current.castling = true;
                current.mode = false;
                currentBoard.Castle(current);
                Utils::PrintBoard(currentBoard);
                continue;
            }

            if (input.length() != 4 && input.length() != 5)
            {
                std::cerr << "Invalid format." << std::endl;
                continue;
            }

            std::string fromStr = input.substr(0, 2);
            std::string toStr = input.substr(2, 2);

            current.from = Utils::ConvertToIndexPosition(fromStr);
            current.to = Utils::ConvertToIndexPosition(toStr);
            current.promotion = -1;

            if (input.length() == 5)
            {
                for (int i = 0; i < 12; i++)
                {
                    if (PIECE_CHAR[i] == input[4])
                    {
                        current.promotion = i;
                        break;
                    }
                }

                if (current.promotion == -1)
                {
                    std::cerr << "Invalid promotion: " << input[4] << std::endl;
                    continue;
                }

                currentBoard.Promotion(current);
            }
            else
            {
                currentBoard.MovePiece(current);
            }

            Utils::PrintBoard(currentBoard);
            std::cout << "Evaluation: " << Evaluation::Evaluate(currentBoard) << "\n";
        }
        else
        {
            std::cout << "Engine thnking..." << std::endl;

            NODES = 0;
            MoveEval bestMove = AlphaBeta(currentBoard, maxSearchDepth, -1000000, 1000000, false);

            if (bestMove.move.from == 255 || bestMove.move.to == 255)
            {
                std::cout << "GAME OVER" << std::endl;
                break;
            }

            currentBoard.MovePiece(bestMove.move);
            Utils::PrintBoard(currentBoard);

            std::cout << "M_CE plays: "
                << Utils::ConvertToBoardPosition(bestMove.move.from)
                << Utils::ConvertToBoardPosition(bestMove.move.to)
                << (bestMove.move.promotion != -1 ? PIECE_CHAR[bestMove.move.promotion] : ' ')
                << "\nEvaluation: " << bestMove.eval << "\n"
                << "Nodes: " << NODES << "\n";
        }
    }
}

uint64_t Engine::knightMasks[64];
uint64_t Engine::kingMasks[64];
ZobristHashSettings Engine::hashSettings;
std::vector<UndoInfo> Engine::undoStack;

void Engine::GenerateZobristHash(const int seed)
{
	// generate 64 bit randoms
	std::mt19937_64 rng(seed);
	for (int piece = 0; piece < 12; ++piece)
	{
		for (int square = 0; square < 64; ++square)
			hashSettings.zobristPieces[piece][square] = rng();
		
	}

	for (int castling = 0; castling < 4; ++castling)
		hashSettings.zobristCastling[castling] = rng();

	for (int square = 0; square < 8; ++square)
		hashSettings.zobristEnPassant[square] = rng();

	hashSettings.zobristTurn = rng();
}

void Engine::InitKingMasks() {
    for (int sq = 0; sq < 64; sq++) {
        uint64_t mask = 0ULL;
        int rank = sq / 8;
        int file = sq % 8;

        for (int dr = -1; dr <= 1; dr++) {
            for (int df = -1; df <= 1; df++) {
                if (dr == 0 && df == 0) continue;

                int r = rank + dr;
                int f = file + df;

                if (r >= 0 && r < 8 && f >= 0 && f < 8) {
                    int targetSq = r * 8 + f;
                    mask |= (1ULL << targetSq);
                }
            }
        }

        kingMasks[sq] = mask;
    }
}


