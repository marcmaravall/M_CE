#include "engine.h"
#include "search.h"

const int Engine::maxSearchDepth;

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
    polyglotSettings = generatePolyglotSettings();

    std::ofstream debugFile("C:\\Users\\Marc\\source\\repos\\M_CE\\tests", std::ios::app);
    debugFile.clear();
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
    std::getline(std::cin, input);

    Move current = {
        .from = 255, .to = 255, .castling = false, .mode = 0, .check = false, .checkmate = false,
    };
	current.promotion = 255;

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

    else if (input == "p4") {

        Divide(currentBoard, 5);
    }

    else if (input == "p3") {

        Divide(currentBoard, 3);
    }

    else if (input == "p2") {

        Divide(currentBoard, 2);
    }


    else if (input == "p1") {

        Divide(currentBoard, 1);
    }

    else if (input == "c")
    {
		std::cout << "IsCheck: " << currentBoard.IsCheck(currentBoard.turn == WHITE_TURN? WHITE : BLACK, true) << "\n";
    }

    if (input.rfind("fen ", 0) == 0)
    {
        std::string fen = input.substr(4);
        std::cout << "Loaded fen: " << fen << "\n";
        currentBoard = Board(fen.c_str());
        Utils::PrintBoard(currentBoard);
        return;
    }

    if (input.length() != 4 && input.length() != 5) {
        std::cerr << "" << std::endl;
        return;
    }

    std::string fromStr = input.substr(0, 2);
    std::string toStr = input.substr(2, 2);

    current.from =  Utils::ConvertToIndexPosition(fromStr);
    current.to =    Utils::ConvertToIndexPosition(toStr);

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
    // std::cout << "Evaluation: " << Evaluation::Evaluate(currentBoard) << "\n";
    // std::cout << "TEST: " << currentBoard.IsCheck(currentBoard.turn == !WHITE_TURN? currentBoard.GetWhiteKingPosition() : currentBoard.GetBlackKingPosition()) << "\n";
	// MoveEval bestMove = Search(5);
	// std::cout << "Evaluation: " << bestMove.eval << "\n";
}

void Engine::PlayAgainistItself()
{
    init();

    std::cout << "Playing vs it..." << std::endl;
    currentBoard = Board(START_FEN);
    Utils::PrintBoard(currentBoard);
    int i = 0;
    while (i < 1)
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        
		NODES = 0;
        ALPHA_BETA_PRUNINGS = 0;
        Divide(currentBoard, 4);

        MoveEval bestMove = AlphaBeta(currentBoard, maxSearchDepth, -1000000, 1000000, currentBoard.turn == WHITE_TURN, maxSearchDepth);
        std::cout << "Best move: " << Utils::ConvertToBoardPosition(bestMove.move.from) << Utils::ConvertToBoardPosition(bestMove.move.to) << "\n";
        currentBoard.MovePiece(bestMove.move);
        Utils::PrintBoard(currentBoard);

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
		std::cout   << "Elapsed time: " << elapsed.count() << " seconds\n"
		            << "Nodes searched: " << NODES << "\n" 
                    << "NPS: " << NODES / elapsed.count() << "\n" 
                    << "Prunings: " << ALPHA_BETA_PRUNINGS << "\n";

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
            // std::cout << "Move: ";
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
            MoveEval bestMove = AlphaBeta(currentBoard, maxSearchDepth, -1000000, 1000000, false, maxSearchDepth);

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

void Engine::StartPos()
{
	currentBoard = Board(START_FEN);
}

void Engine::SetPosition(const char* fen)
{
	currentBoard = Board(fen);
}

#include <chrono>

MoveEval Engine::SearchTime(int time_ms)
{
    MoveEval currentMove;

    int depth = 1;
    const int maxDepth = 10000;

    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

    while (elapsed < time_ms && depth <= maxDepth)
    {
        auto iteration_start = high_resolution_clock::now();

        currentMove = AlphaBeta(currentBoard, depth, -1000000, 1000000, currentBoard.turn == WHITE_TURN, maxSearchDepth);

        depth++;
        auto iteration_end = high_resolution_clock::now();
        auto iteration_duration = duration_cast<milliseconds>(iteration_end - iteration_start).count();
        elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

        if (elapsed + iteration_duration > time_ms)
        {
            break;
        }
    }

    return currentMove;
}


MoveEval Engine::Search(int depth)
{
	MoveEval currentMove;
	currentMove = AlphaBeta(currentBoard, depth, -1000000, 1000000, currentBoard.turn == WHITE_TURN, depth);

	return currentMove;
}

void Engine::MovePiece(const char* moveStr)
{
	std::string input(moveStr);
    if (input.length() >= 4)
    {
        Move current = {
            .from = 255, .to = 255,.castling = false, .mode = 0, .check = false, .checkmate = false,
        };
        current.promotion = 255;

        uint8_t from = 0, to = 0;

        std::string fromStr = input.substr(0, 2);
        std::string toStr = input.substr(2, 2);

        current.from = Utils::ConvertToIndexPosition(fromStr);
        current.to = Utils::ConvertToIndexPosition(toStr);

        if (currentBoard.GetTurn() == WHITE_TURN) {
            if (currentBoard.GetWhiteKingPosition() == 4 && current.to == 6 && currentBoard.wCastlingKing) {
                current.castling = true;
                current.mode = 1;
            } 
            else if (currentBoard.GetWhiteKingPosition() == 4 && current.to == 2 && currentBoard.wCastlingQueen) {
                 current.castling = true;
                 current.mode = 0;
            }
        }
        else if (currentBoard.GetTurn() == BLACK_TURN) {
            if (currentBoard.GetBlackKingPosition() == 60 && current.to == 62 && currentBoard.bCastlingKing) {
                current.castling = true;
                current.mode = 1;
            }
            else if (currentBoard.GetBlackKingPosition() == 60 && current.to == 58 && currentBoard.bCastlingQueen) {
                current.castling = true;
                current.mode = 0;
            }
        }

		if (input.length() == 5) {
            uint8_t promotionPiece = 255;

			promotionPiece = input.substr(4, 1)[0];
            current.promotion = Utils::GetPromotionPiece(promotionPiece, currentBoard.turn == WHITE_TURN);

			std::cerr << "promotion: " << current.promotion << "\n";
            // std::cerr << "to: " << toStr << "\n";
			// std::cerr << input << "\n";
		}

		currentBoard.MovePiece(current);
    }
}

void Engine::PrintBoard()
{
	const Board& board = currentBoard;

    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << "+---+---+---+---+---+---+---+---+\n";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            bool found = false;

            for (size_t pieceIndex = 0; pieceIndex < 12; pieceIndex++)
            {
                if (board.bitboards[pieceIndex] & (1ULL << square))
                {
                    std::cout << "| " << PIECE_CHAR[pieceIndex] << " ";
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "|   ";
            }
        }
		std::cout << "| " << rank+1 << "\n";
    }
	std::cout << "+---+---+---+---+---+---+---+---+\n"
		      << "  a   b   c   d   e   f   g   h\n";

	std::cout << "\nFen: " << Utils::ConvertToFEN(currentBoard) << "\n";
}

void Engine::DivideTest(uint8_t depth)
{
	Divide(currentBoard, depth);
}

uint32_t Engine::polyglotSeed = 0x9D81F9B8;;

uint32_t Engine::getRandomU32() {
    uint32_t x = polyglotSeed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    polyglotSeed = x;
    return x;
}

uint64_t Engine::getRandomU64() {
    uint64_t high = getRandomU32();
    uint64_t low =  getRandomU32();
    return (high << 32) | low;
}

ZobristHashSettings Engine::polyglotSettings;

ZobristHashSettings Engine::generatePolyglotSettings() {
    ZobristHashSettings settings;

    for (int piece = 0; piece < 12; ++piece) {
        for (int square = 0; square < 64; ++square) {
            settings.zobristPieces[piece][square] = getRandomU64();
        }
    }

    for (int i = 0; i < 4; ++i) {
        settings.zobristCastling[i] = getRandomU64();
    }

    for (int i = 0; i < 8; ++i) {
        settings.zobristEnPassant[i] = getRandomU64();
    }

    settings.zobristTurn = getRandomU64();

    return settings;
}
