#include "engine.h"
#include "search.h"

const int Engine::maxSearchDepth;
Book Engine::book;

Engine::Engine()
{
    init();
	// currentBoard = Board(START_FEN);
}

Engine::~Engine()
{
}

void Engine::init()
{
    programDir = getProjectDirectory().string();

	GenerateZobristHash(rand());
    InitBetweenTable();
	InitKnightMasks();
    InitKingMasks();
    InitDistanceTable();

    GenerateMagicNumbers();

    InitBishopTable();
    InitRookTable();

    // ValidateMagics();
    
    // DebugMagics();
    // DebugTables();

    polyglotSettings = generatePolyglotSettings();

    const std::string bookName = "komodo.bin";

#ifdef WIN32
    book = Book(programDir+"\\books\\" + bookName);
    std::ofstream debugFile(programDir + "\\tests", std::ios::app);
#elif __linux__
    book = Book(programDir+"/books/" + bookName);
    std::ofstream debugFile(programDir + "/tests", std::ios::app);
#endif

	currentBoard = Board(START_FEN);    

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
            std::cin >> input;

            Move current = {
                .from = 255, .to = 255,.castling = false, .mode = 0, .check = false, .checkmate = false,
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

Bitboard Engine::bishopAttackTable[64][512];
Bitboard Engine::rookAttackTable[64][4096];

Magic Engine::rookMagics[64] = {
    { 0, 0xa8002c000108020ULL,  0},
    { 0, 0x6c00049b0002001ULL,  0},
    { 0, 0x100200010090040ULL,  0},
    { 0, 0x2480041000800801ULL, 0},
    { 0, 0x280028004000800ULL,  0},
    { 0, 0x900410008040022ULL,  0},
    { 0, 0x280020001001080ULL,  0},
    { 0, 0x2880002041000080ULL, 0},
    { 0, 0xa000800080400034ULL, 0},
    { 0, 0x4808020004000ULL,    0},
    { 0, 0x2290802004801000ULL, 0},
    { 0, 0x411000d00100020ULL,  0},
    { 0, 0x402800800040080ULL,  0},
    { 0, 0xb000401004208ULL,    0},
    { 0, 0x2409000100040200ULL, 0},
    { 0, 0x1002100004082ULL,    0},
    { 0, 0x22878001e24000ULL,   0},
    { 0, 0x1090810021004010ULL, 0},
    { 0, 0x801030040200012ULL,  0},
    { 0, 0x500808008001000ULL,  0},
    { 0, 0xa08018014000880ULL,  0},
    { 0, 0x8000808004000200ULL, 0},
    { 0, 0x201008080010200ULL,  0},
    { 0, 0x801020000441091ULL,  0},
    { 0, 0x800080204005ULL,     0},
    { 0, 0x1040200040100048ULL, 0},
    { 0, 0x120200402082ULL,     0},
    { 0, 0xd14880480100080ULL,  0},
    { 0, 0x12040280080080ULL,   0},
    { 0, 0x100040080020080ULL,  0},
    { 0, 0x9020010080800200ULL, 0},
    { 0, 0x813241200148449ULL,  0},
    { 0, 0x491604001800080ULL,  0},
    { 0, 0x100401000402001ULL,  0},
    { 0, 0x4820010021001040ULL, 0},
    { 0, 0x400402202000812ULL,  0},
    { 0, 0x209009005000802ULL,  0},
    { 0, 0x810800601800400ULL,  0},
    { 0, 0x4301083214000150ULL, 0},
    { 0, 0x204026458e001401ULL, 0},
    { 0, 0x40204000808000ULL,   0},
    { 0, 0x8001008040010020ULL, 0},
    { 0, 0x8410820820420010ULL, 0},
    { 0, 0x1003001000090020ULL, 0},
    { 0, 0x804040008008080ULL,  0},
    { 0, 0x12000810020004ULL,   0},
    { 0, 0x1000100200040208ULL, 0},
    { 0, 0x430000a044020001ULL, 0},
    { 0, 0x280009023410300ULL,  0},
    { 0, 0xe0100040002240ULL,   0},
    { 0, 0x200100401700ULL,     0},
    { 0, 0x2244100408008080ULL, 0},
    { 0, 0x8000400801980ULL,    0},
    { 0, 0x2000810040200ULL,    0},
    { 0, 0x8010100228810400ULL, 0},
    { 0, 0x2000009044210200ULL, 0},
    { 0, 0x4080008040102101ULL, 0},
    { 0, 0x40002080411d01ULL,   0},
    { 0, 0x2005524060000901ULL, 0},
    { 0, 0x502001008400422ULL,  0},
    { 0, 0x489a000810200402ULL, 0},
    { 0, 0x1004400080a13ULL,    0},
    { 0, 0x4000011008020084ULL, 0},
    { 0, 0x26002114058042ULL,   0},
};

Magic Engine::bishopMagics[64] = {
    { 0, 0x89a1121896040240ULL, 0 },
    { 0, 0x2004844802002010ULL, 0 },
    { 0, 0x2068080051921000ULL, 0 },
    { 0, 0x62880a0220200808ULL, 0 },
    { 0, 0x4042004000000ULL,    0 },
    { 0, 0x100822020200011ULL,  0 },
    { 0, 0xc00444222012000aULL, 0 },
    { 0, 0x28808801216001ULL,   0 },
    { 0, 0x400492088408100ULL,  0 },
    { 0, 0x201c401040c0084ULL,  0 },
    { 0, 0x840800910a0010ULL,   0 },
    { 0, 0x82080240060ULL,      0 },
    { 0, 0x2000840504006000ULL, 0 },
    { 0, 0x30010c4108405004ULL, 0 },
    { 0, 0x1008005410080802ULL, 0 },
    { 0, 0x8144042209100900ULL, 0 },
    { 0, 0x208081020014400ULL,  0 },
    { 0, 0x4800201208ca00ULL,   0 },
    { 0, 0xf18140408012008ULL,  0 },
    { 0, 0x1004002802102001ULL, 0 },
    { 0, 0x841000820080811ULL,  0 },
    { 0, 0x40200200a42008ULL,   0 },
    { 0, 0x800054042000ULL,     0 },
    { 0, 0x88010400410c9000ULL, 0 },
    { 0, 0x520040470104290ULL,  0 },
    { 0, 0x1004040051500081ULL, 0 },
    { 0, 0x2002081833080021ULL, 0 },
    { 0, 0x400c00c010142ULL,    0 },
    { 0, 0x941408200c002000ULL, 0 },
    { 0, 0x658810000806011ULL,  0 },
    { 0, 0x188071040440a00ULL,  0 },
    { 0, 0x4800404002011c00ULL, 0 },
    { 0, 0x104442040404200ULL,  0 },
    { 0, 0x511080202091021ULL,  0 },
    { 0, 0x4022401120400ULL,    0 },
    { 0, 0x80c0040400080120ULL, 0 },
    { 0, 0x8040010040820802ULL, 0 },
    { 0, 0x480810700020090ULL,  0 },
    { 0, 0x102008e00040242ULL,  0 },
    { 0, 0x809005202050100ULL,  0 },
    { 0, 0x8002024220104080ULL, 0 },
    { 0, 0x431008804142000ULL,  0 },
    { 0, 0x19001802081400ULL,   0 },
    { 0, 0x200014208040080ULL,  0 },
    { 0, 0x3308082008200100ULL, 0 },
    { 0, 0x41010500040c020ULL,  0 },
    { 0, 0x4012020c04210308ULL, 0 },
    { 0, 0x208220a202004080ULL, 0 },
    { 0, 0x111040120082000ULL,  0 },
    { 0, 0x6803040141280a00ULL, 0 },
    { 0, 0x2101004202410000ULL, 0 },
    { 0, 0x8200000041108022ULL, 0 },
    { 0, 0x21082088000ULL,      0 },
    { 0, 0x2410204010040ULL,    0 },
    { 0, 0x40100400809000ULL,   0 },
    { 0, 0x822088220820214ULL,  0 },
    { 0, 0x40808090012004ULL,   0 },
    { 0, 0x910224040218c9ULL,   0 },
    { 0, 0x402814422015008ULL,  0 },
    { 0, 0x90014004842410ULL,   0 },
    { 0, 0x1000042304105ULL,    0 },
    { 0, 0x10008830412a00ULL,   0 },
    { 0, 0x2520081090008908ULL, 0 },
    { 0, 0x40102000a0a60140ULL, 0 }, 
};

Bitboard Engine::knightMasks[64];
Bitboard Engine::kingMasks[64];
Bitboard Engine::between[64][64];
ZobristHashSettings Engine::hashSettings;
std::vector<UndoInfo> Engine::undoStack;
int Engine::distanceTable[64][64];

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

#define USE_BOOKS

MoveEval Engine::SearchTime(const int time_ms)
{
    MoveEval currentMove;

    int depth = 1;

    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

#ifdef USE_BOOKS
    currentMove = MoveBook();
    if (currentMove.eval == 0) {
        return currentMove;
    }
#endif

    while (elapsed < time_ms && depth <= maxSearchDepth)
    {
        const auto iteration_start = high_resolution_clock::now();

        for (size_t i = 0; i < TT_SIZE; i++) {
			TranspositionTable[i] = TTEntry(); 
        }

        currentMove = AlphaBeta(currentBoard, depth, -MATE_SCORE, MATE_SCORE, currentBoard.turn == WHITE_TURN, depth);

        depth++;
        const auto iteration_end = high_resolution_clock::now();
        const auto iteration_duration = duration_cast<milliseconds>(iteration_end - iteration_start).count();
        elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

        if (elapsed + iteration_duration* manager.waitingOffset > time_ms)
        {
            break;
        }
    }

    std::cout << "info depth " << depth << "\n";
    return currentMove;
}

MoveEval Engine::SearchNodes(const uint64_t nodes) {
    MoveEval currentMove;

#ifdef USE_BOOKS
    currentMove = MoveBook();
    if (currentMove.eval == 0) {
        std::cout << "info string Using Books in Search\n";
        return currentMove;
    }
#endif

    int depth = 1;

    while (NODES < nodes && depth <= maxSearchDepth)
    {
        currentMove = AlphaBeta(currentBoard, depth, -MATE_SCORE, MATE_SCORE, currentBoard.turn == WHITE_TURN, depth);
        depth++;
    }

    std::cout << "info depth " << depth << "\n";
    return currentMove;
}


MoveEval Engine::MoveBook() {

    if (!useBooks) {
        // std::cout << "don't use book!\n";
        return MoveEval({
            .move = Move(),
            .eval = 255,
        });
    }
    uint64_t hash = Utils::GetZobristHash(currentBoard, Engine::polyglotSettings);

    const auto it = std::find_if(book.entries.begin(), book.entries.end(),
        [hash](const PolyglotEntry& entry) {
            return entry.key == hash;
        });

    if (it != book.entries.end())
    {
        const std::vector<MoveEval>& moves = book.GetMoves(hash);

        int totalWeight = 0;
        for (const MoveEval& move : moves) {
            totalWeight += move.weight;
        }

        const int random = rand() % totalWeight;
        int cumulative = 0;

        for (const MoveEval& move : moves) {
            cumulative += move.weight;
            if (random < cumulative) {
                return move;
            }
        }

        return MoveEval(moves[0]);
    }

	return MoveEval({
		.move = Move(),
        .eval = 255, 
	});
}

MoveEval Engine::Search(int depth)
{
	MoveEval currentMove;
    currentMove.move.promotion = 255;
    uint64_t hash = Utils::GetZobristHash(currentBoard, Engine::polyglotSettings);

#ifdef USE_BOOKS
	currentMove = MoveBook();
    if (currentMove.eval == 0) {
        return currentMove;
    }
#endif

    currentMove = AlphaBeta(currentBoard, depth, -MATE_SCORE, MATE_SCORE, currentBoard.turn == WHITE_TURN, depth);

	return currentMove;
}

MoveEval Engine::SearchInfinite()
{
    MoveEval currentMove;
    uint16_t currentDepth = 1;
    
    while (currentDepth < maxSearchDepth) {
        
        NODES = 0;
        const auto start = std::chrono::high_resolution_clock::now();

        currentMove = AlphaBeta(currentBoard, currentDepth, -MATE_SCORE, MATE_SCORE, currentBoard.turn == WHITE_TURN, currentDepth);
        
        const auto end = std::chrono::high_resolution_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // print info of the engine: 
        std::cout << "info nodes " << NODES << " nps " << (double(NODES) / elapsed)*1000.0 << " depth " << currentDepth << " currmove " << Utils::MoveToStr(currentMove.move) << " score ";
        uint16_t mateIn = Utils::GetMate(currentMove.eval);
        if (mateIn != 0) {
            std::cout << "mate " << mateIn << " ";
        } else {
            std::cout << "cp " << currentMove.eval << "\n";
        }
        currentDepth++;
    }

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
            if (currentBoard.GetWhiteKingPosition() == 4 && current.to == 6 && currentBoard.wCastlingKing && current.from == 4) {
                current.castling = true;
                current.mode = 1;
            } 
            else if (currentBoard.GetWhiteKingPosition() == 4 && current.to == 2 && currentBoard.wCastlingQueen && current.from == 4) {
                 current.castling = true;
                 current.mode = 0;
            }
        }
        else if (currentBoard.GetTurn() == BLACK_TURN) {
            if (currentBoard.GetBlackKingPosition() == 60 && current.to == 62 && currentBoard.bCastlingKing && current.from == 60) {
                current.castling = true;
                current.mode = 1;
            }
            else if (currentBoard.GetBlackKingPosition() == 60 && current.to == 58 && currentBoard.bCastlingQueen && current.from == 60) {
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

		currentBoard.MovePieceFast(current);
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

    std::cout 
        << "\nFen: " << Utils::ConvertToFEN(currentBoard) 
        << "\nKey: " << Utils::GetZobristHash(currentBoard, hashSettings) << "\n\n";
}

void Engine::DivideTest(uint8_t depth)
{
	Divide(currentBoard, depth);
}

/*
uint32_t Engine::polyglotSeed = 0x9D81F9B8;                     ADDED TABLE PRECOMPUTED IN BOOK.H

uint32_t Engine::getRandomU32() {
    uint32_t x = polyglotSeed;
    x ^= (x << 13);
    x ^= (x >> 17);
    x ^= (x << 5);
    polyglotSeed = x;
    return x;
}

uint64_t Engine::getRandomU64() {
    uint64_t low = static_cast<uint64_t>(getRandomU32());
    uint64_t high = static_cast<uint64_t>(getRandomU32());

    return low | (high << 32);
}*/

ZobristHashSettings Engine::polyglotSettings;

ZobristHashSettings Engine::generatePolyglotSettings() {
    ZobristHashSettings settings;
    int index = 0;

    const int orderToPolyglot[12] = {
        6,
        0,
        7,
        1,
        8,
        2,
        9,
        3,
        10,
        4,
        11,
        5
    };

    for (int piece = 0; piece < 12; ++piece) {
        for (int square = 0; square < 64; ++square) {
            settings.zobristPieces[orderToPolyglot[piece]][square] = Random64[index++];
        }
    }

    for (int i = 0; i < 4; ++i) {
        settings.zobristCastling[i] = Random64[index++];
    }

    for (int i = 0; i < 8; ++i) {
        settings.zobristEnPassant[i] = Random64[index++];
    }

    settings.zobristTurn = Random64[index++];

    return settings;
}

void Engine::RunBookTest()
{
    init();

    SetPosition(START_FEN);

    this->PrintBoard();
    std::vector<MoveEval> moves = book.GetMoves(Utils::GetZobristHash(currentBoard, Engine::polyglotSettings));

    if (moves.empty()) {
        std::cout << Utils::GetZobristHash(currentBoard, Engine::polyglotSettings) << "\n";
        std::cout << "ERROR: 'moves' is empty\n";
    }
    else
    {
        std::cout << Utils::GetZobristHash(currentBoard, Engine::polyglotSettings) << "\n";

        std::cout << "MOVES NOT EMPTY\n";
        std::cout << moves.size();
    }

    // std::cout << Utils::MoveToStr(moves);
}

void Engine::InitBetweenTable()
{
    for (int from = 0; from < 64; ++from) {
        for (int to = 0; to < 64; ++to) {
            if (from == to) continue;

            const uint8_t from_rank = from / 8;
            const uint8_t from_file = from % 8;
            const uint8_t to_rank = to / 8;
            const uint8_t to_file = to % 8;

            int dir = 0;

            if (from_rank == to_rank) {
                dir = (to > from) ? 1 : -1;
            }

            else if (from_file == to_file) {
                dir = (to > from) ? 8 : -8;
            }

            else if (to_rank - from_rank == to_file - from_file) {
                dir = (to > from) ? 9 : -9;
            }

            else if (to_rank - from_rank == from_file - to_file) {
                dir = (to > from) ? 7 : -7;
            }
            else {
                continue;
            }

            uint8_t square = from + dir;
            while (square != to) {
                between[from][to] |= (1ULL << square);
                square += dir;
            }
        }
    }
}

// code copied from my nes emulator :) --------

fs::path Engine::getProjectDirectory() {
    fs::path exe = fs::current_path();
    while (!exe.empty() && exe.filename() != "M_CE") {
        exe = exe.parent_path();
    }

    if (exe.empty()) {
        std::cerr << "ERROR: directory not found." << std::endl;
        return "";
    }

    return exe;
}
// -------------------------------------------

void Engine::InitBishopTable() {
    for (int sq = 0; sq < 64; sq++) {
        Bitboard mask = bishopMagics[sq].mask;
        int relevantBits = Utils::CountBits(mask);
        int occCount = 1 << relevantBits;

        for (int i = 0; i < occCount; i++) {
            Bitboard occupancy = Utils::GenerateOccupancy(mask, i);
            int index = (occupancy * bishopMagics[sq].magic) >> bishopMagics[sq].shift;
            bishopAttackTable[sq][index] = Utils::GenerateBishopAttacks(sq, occupancy);
        }
    }
}

void Engine::InitRookTable() {
    for (int sq = 0; sq < 64; sq++) {
        Bitboard mask = rookMagics[sq].mask;
        int relevantBits = Utils::CountBits(mask);
        int occCount = 1 << relevantBits;

        for (int i = 0; i < occCount; i++) {
            Bitboard occupancy = Utils::GenerateOccupancy(mask, i);
            int index = (occupancy * rookMagics[sq].magic) >> rookMagics[sq].shift;
            rookAttackTable[sq][index] = Utils::GenerateRookAttacks(sq, occupancy);
        }
    }
}

void Engine::GenerateMagicNumbers() // TODO: implement the generator (without precalculed attacks)
{
    // for rooks
    for (int square = 0; square < 64; square++) {
        const Bitboard mask = Utils::GenerateRookAttacksForMagics(square, 0);
        
        const int relevantBits = Utils::CountBits(mask);
        const int occupancyCount = 1 << relevantBits;
        
        rookMagics[square].mask = mask;
        rookMagics[square].shift = 64 - relevantBits;

        // int relevantBits = Utils::CountBits(mask);
        // int occupancyCount = 1 << relevantBits;
        // 
        // uint64_t attemps = 0;
        // 
        // while (true) {
        //     attemps++;
        //     Bitboard magic = (Utils::Rand64() & Utils::Rand64() & Utils::Rand64());
        // 
        //     auto copy = rookAttackTable[square];
        // 
        //     for (size_t i = 0; i < 4096; i++) {
        //         copy[i] *= magic;
        //     }
        // 
        //     bool fail = Utils::HasRepeated(copy);
        // 
        //     if (!fail) {
		// 		std::cout << "Found Magic " << square << " after " << attemps << " attempts.\n";
        //         attemps = 0;
        //         rookMagics[square].magic = magic;
		// 		rookMagics[square].mask = mask;
		// 		rookMagics[square].shift = 64 - relevantBits;
        //         break;
        //     }
        // }
    }

    // for bishops
    for (int square = 0; square < 64; square++) {
        const Bitboard mask = Utils::GenerateBishopAttacksForMagics(square, 0ULL);

        const int relevantBits = Utils::CountBits(mask);
        const int occupancyCount = 1 << relevantBits;

        bishopMagics[square].mask = mask;
        bishopMagics[square].shift = 64 - relevantBits;

        // while (true) {
        //     Bitboard magic = (Utils::Rand64() & Utils::Rand64() & Utils::Rand64());
        // 
		// 	auto copy = bishopAttackTable[square];
        // 
        //     for (size_t i = 0; i < 512; i++) {
        //         copy[i] *= magic;
        //     }
        // 
        //     bool fail = Utils::HasRepeated(copy);
        // 
        //     if (!fail) {
        //         bishopMagics[square].magic = magic;
        //         bishopMagics[square].mask = mask;
        //         bishopMagics[square].shift = 64 - relevantBits;
        //         break;
        //     }
        // }
    }

#ifdef M_CE_DEBUG
    std::cout << "Generated Magic Numbers of bishops and rooks.\n";
#endif
}

void Engine::DebugTables() {
    // std::cout << "DEBUGGING TABLES\n";

    std::string debugDir = programDir + "\\debug";
    std::filesystem::create_directories(debugDir);

    std::string filePath = debugDir + "\\text.txt";
    std::ofstream debugFile(filePath);

    if (!debugFile.is_open()) {
        std::cerr << "ERROR: cannot found path " << filePath << std::endl;
        return;
    }

    std::cout << "Writting in: " << filePath << std::endl;

    debugFile << "---------- \nBISHOP TABLE \n---------- \n \n";
    for (size_t i = 0; i < 64; i++) {
        for (size_t j = 0; j < 512; j++) {
            debugFile << "BISHOP: " << i << " " << j << ": " << Utils::ToBin(bishopAttackTable[i][j]) << "\n";
        }
    }

    debugFile << "---------- \nROOK TABLE \n---------- \n \n";
    for (size_t i = 0; i < 64; i++) {
        for (size_t j = 0; j < 4096; j++) {
            debugFile << "ROOK: "  << i << " " << j << ": " << Utils::ToBin(rookAttackTable[i][j]) << "\n";
        }
    }

    debugFile.close();
}


void Engine::TestBishopGen()
{
    Bitboard bb = 0ULL;

    for (uint8_t sq = 0; sq < 64; sq++) {
		bb = Utils::GenerateBishopAttacksForMagics(sq, 0ULL);

		std::cout << "BISHOP: " << int(sq) << ": \n" << Utils::ToBin(bb) << "\n";

    }
}

void Engine::DebugMagics()
{
    std::string debugDir = programDir + "\\debug";
    std::filesystem::create_directories(debugDir);

    std::string filePath = debugDir + "\\magics.txt";
    std::ofstream debugFile(filePath);

    if (!debugFile.is_open()) {
        std::cerr << "ERROR: cannot found path " << filePath << std::endl;
        return;
    }

    std::cout << "Writting magics in: " << filePath << std::endl;

    debugFile << "---------- \nROOK MAGICS \n---------- \n \n";
    for (size_t i = 0; i < 64; i++) {
        debugFile << "Magic " << i << ": " << rookMagics[i].magic << ". \n";
    }

    debugFile << "---------- \nBISHOP MAGICS \n---------- \n \n";
    for (size_t i = 0; i < 64; i++) {
        debugFile << "Magic " << i << ": " << bishopMagics[i].magic << ". \n";
    }

    debugFile.close();
}


bool Engine::ValidateMagics()
{
    for (int sq = 0; sq < 64; ++sq) {
        Bitboard mask = rookMagics[sq].mask;
        int rb = 64 - rookMagics[sq].shift;
        int vars = 1 << rb;
        for (int i = 0; i < vars; ++i) {
            Bitboard occ = Utils::GenerateOccupancy(mask, i);
            int idx = (int)((occ * rookMagics[sq].magic) >> rookMagics[sq].shift);
            Bitboard ref = Utils::GenerateRookAttacks(sq, occ);
            Bitboard tab = rookAttackTable[sq][idx];
            if (ref != tab) {
                std::cerr << "Rook magic fail sq=" << sq << " i=" << i << "\n";
                return false;
            }
        }
    }

    for (int sq = 0; sq < 64; ++sq) {
        Bitboard mask = bishopMagics[sq].mask;
        int rb = 64 - bishopMagics[sq].shift;
        int vars = 1 << rb;
        for (int i = 0; i < vars; ++i) {
            Bitboard occ = Utils::GenerateOccupancy(mask, i);
            int idx = (int)((occ * bishopMagics[sq].magic) >> bishopMagics[sq].shift);
            Bitboard ref = Utils::GenerateRookAttacks(sq, occ);
            Bitboard tab = bishopAttackTable[sq][idx];
            if (ref != tab) {
                std::cerr << "Bishop magic fail sq=" << sq << " i=" << i << "\n";
                return false;
            }
        }
    }

    return true;
}

void Engine::TestOccupancy()
{
    Bitboard mask = Utils::GenerateRookAttacks(0, 0ULL);

    int bits = Utils::CountBits(mask);
    int occCount = 1 << bits;

    for (int i = 0; i < occCount; ++i) {
        Bitboard occ = Utils::GenerateOccupancy(mask, i);

        if (occ & ~mask) {
            std::cerr << "ERROR: occ has bits outside mask. i=" << i << "\n";
        }
    }
}

void Engine::InitDistanceTable()
{
    // thanks to https://www.chessprogramming.org/Evaluation_Function_Draft for the code 
    int i, j;

    for (i = 0; i < 64; ++i) {
        for (j = 0; j < 64; ++j) {
            distanceTable[i][j] = 14 - (abs(i%8 - j%8)
                + abs(i/8 - j/8));
        }
    }
}
