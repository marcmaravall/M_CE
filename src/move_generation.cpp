#include "move_generation.h"

void GeneratePawnMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {

	Move currentMove;
	currentMove.promotion = 255;
	currentMove.capture = false;
	currentMove.from = from;
	currentMove.to = 0;

	currentMove.movedPiece = board.turn == WHITE_TURN ? W_PAWN_I : B_PAWN_I;

	const bool isWhite = board.turn==WHITE_TURN;
	const uint8_t rank = from / 8;
	const uint8_t file = from % 8;
	const bool canPromote = (isWhite && rank == 6) || (!isWhite && rank == 1);
	const bool canEnPassant = board.enPassantSquare != 255 && (
		isWhite ? (from + NORTH_EAST == board.enPassantSquare && (file != 7) || from + NORTH_WEST == board.enPassantSquare && (file != 0)) : 
				  (from + SOUTH_EAST == board.enPassantSquare && (file != 7) || from + SOUTH_WEST == board.enPassantSquare && (file != 0)));
	
	const Bitboard occupancies = Utils::GetAllBitboards(board.bitboards, BOTH);

	bool debug = false;

	if (from == 26) {
		debug = true;
	}
	
	if (isWhite)
	{
		// WHITE

		const Bitboard enemyOcc = Utils::GetAllBitboards(board.bitboards, BLACK);

		Bitboard mask = 1ULL << from;

		if (!Utils::GetBitboardValueOnIndex(occupancies, from+NORTH))
			mask |= 1ULL << (from + NORTH);
		
		if (rank == 1 && !Utils::GetBitboardValueOnIndex(occupancies, from+NORTH*2) && !Utils::GetBitboardValueOnIndex(occupancies, from + NORTH))
			mask |= 1ULL << (from + NORTH*2);

		if (canEnPassant) {
			if (file != 7 && from + NORTH_EAST == board.enPassantSquare) {
				mask |= 1ULL << (from + NORTH_EAST);
			}
			if (file != 0 && from + NORTH_WEST == board.enPassantSquare) {
				mask |= 1ULL << (from + NORTH_WEST);
			}
		}
		
		// attacks
		if (file != 7 && Utils::GetBitboardValueOnIndex(enemyOcc, from + NORTH_EAST)) {
			mask |= 1ULL << (from + NORTH_EAST);
		}
		if (file != 0 && Utils::GetBitboardValueOnIndex(enemyOcc, from + NORTH_WEST))
			mask |= 1ULL << (from + NORTH_WEST);

		while (mask) {
			uint8_t to = Utils::PopLSB(mask);

			const bool isCapture = Utils::IsEnemyPieceAt(board, to);
			const int pieceType = Utils::GetPieceType(board, to);

			if (pieceType == 255 || isCapture) {
				currentMove.to = to;
				currentMove.capture = isCapture;
				currentMove.capturedPiece = pieceType;

				if (canPromote) {
					for (uint8_t promotion = 1; promotion < 5; promotion++) {
						currentMove.promotion = promotion;
						moves.push_back(currentMove);
					}
					continue;
				}
				else currentMove.promotion = 255;

				moves.push_back(currentMove);
			}
		}

		return;
	}

	// BLACK
	const Bitboard enemyOcc = Utils::GetAllBitboards(board.bitboards, WHITE);

	Bitboard mask = 1ULL << from;

	if (!Utils::GetBitboardValueOnIndex(occupancies, from + SOUTH))
		mask |= 1ULL << (from + SOUTH);

	if (rank == 6 && !Utils::GetBitboardValueOnIndex(occupancies, from + SOUTH * 2) && !Utils::GetBitboardValueOnIndex(occupancies, from + SOUTH))
		mask |= 1ULL << (from + SOUTH * 2);

	if (file != 7 && from + SOUTH_EAST == board.enPassantSquare) {
		mask |= 1ULL << (from + SOUTH_EAST);
	}
	if (file != 0 && from + SOUTH_WEST == board.enPassantSquare) {
		mask |= 1ULL << (from + SOUTH_WEST);
	}

	// attacks
	if (file != 7 && Utils::GetBitboardValueOnIndex(enemyOcc, from + SOUTH_EAST))
		mask |= 1ULL << (from + SOUTH_EAST);
	if (file != 0 && Utils::GetBitboardValueOnIndex(enemyOcc, from + SOUTH_WEST))
		mask |= 1ULL << (from + SOUTH_WEST);

	while (mask) {
		uint8_t to = Utils::PopLSB(mask);

		const bool isCapture = Utils::IsEnemyPieceAt(board, to);
		const int pieceType = Utils::GetPieceType(board, to);

		if (pieceType == 255 || isCapture) {
			currentMove.to = to;
			currentMove.capture = isCapture;
			currentMove.capturedPiece = pieceType;

			if (canPromote) {
				for (uint8_t promotion = 7; promotion < 11; promotion++) {
					currentMove.promotion = promotion;
					moves.push_back(currentMove);
				}
				continue;
			}
			else currentMove.promotion = 255;

			moves.push_back(currentMove);
		}
	}
}

void GenerateKnightMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	Move currentMove;
	currentMove.from = from;

	currentMove.movedPiece = board.turn == WHITE_TURN ? W_KNIGHT_I : B_KNIGHT_I;

	uint64_t mask = Engine::knightMasks[from];

	while (mask) {
		uint8_t to = Utils::PopLSB(mask);
		const bool isCapture = Utils::IsEnemyPieceAt(board, to);
		const int pieceType = Utils::GetPieceType(board, to);

		if (pieceType == 255 || isCapture) {
			currentMove.to = to;
			currentMove.capture = isCapture;
			currentMove.capturedPiece = pieceType;

			moves.push_back(currentMove);
		}
	}
}

// #define DEBUG_MAGICS

void GenerateBishopMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	Move currentMove;

	currentMove.promotion = 255;
	currentMove.capture = false;
	currentMove.from = from;
	currentMove.to = 0;

	currentMove.movedPiece = board.turn == WHITE_TURN ? W_BISHOP_I : B_BISHOP_I;

	currentMove.from = from;

#ifdef DEBUG_MAGICS
	{
		Bitboard occAll = Utils::GetAllBitboards(board.bitboards, BOTH);
		Bitboard fast = Engine::bishopAttackTable[from][((occAll & Engine::bishopMagics[from].mask) * Engine::bishopMagics[from].magic) >> Engine::bishopMagics[from].shift];
		Bitboard slow = Utils::GenerateBishopAttacks(from, occAll);
		if (fast != slow) {
			std::cerr << "Bishop magic mismatch from=" << int(from) << "\n";
			std::cout << "occAll: \n";
			Utils::DebugBitboard(occAll);

			std::cout << "magic: " << uint64_t(((occAll & Engine::bishopMagics[from].mask) * Engine::bishopMagics[from].magic) >> Engine::bishopMagics[from].shift) << "\n";
			Utils::DebugBitboard(fast);
			
			std::cout << "slow: \n";
			Utils::DebugBitboard(slow);
			abort();
		}
	}
#endif

	Bitboard occupancy = Utils::GetAllBitboards(board.bitboards, BOTH);
	int index = ((occupancy & Engine::bishopMagics[from].mask) * Engine::bishopMagics[from].magic) >> Engine::bishopMagics[from].shift;
	Bitboard mask = Engine::bishopAttackTable[from][index];

	while (mask) {
		uint8_t to = Utils::PopLSB(mask);

		const bool isEnemyPiece = Utils::IsEnemyPieceAt(board, to);
		const int pieceType = Utils::GetPieceType(board, to);

		if (pieceType == 255 || isEnemyPiece) {
			currentMove.to = to;
			currentMove.capture = isEnemyPiece;
			currentMove.capturedPiece = pieceType;

			moves.push_back(currentMove);
		}
	}
}

void GenerateRookMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {

	Move currentMove;

	currentMove.promotion = 255;
	currentMove.capture = false;
	currentMove.from = from;
	currentMove.to = 0;

	currentMove.movedPiece = board.turn == WHITE_TURN ? W_ROOK_I : B_ROOK_I;

#ifdef DEBUG_MAGICS
	{
		Bitboard occAll = Utils::GetAllBitboards(board.bitboards, BOTH);
		Bitboard fast = Engine::rookAttackTable[from][((occAll & Engine::rookMagics[from].mask) * Engine::rookMagics[from].magic) >> Engine::rookMagics[from].shift];
		Bitboard slow = Utils::GenerateRookAttacks(from, occAll); 
		if (fast != slow) {
			std::cerr << "Rook magic mismatch from=" << int(from) << "\n";
				Utils::DebugBitboard(occAll);
				Utils::DebugBitboard(fast);
				Utils::DebugBitboard(slow);
			abort(); 
		}
	}
#endif

	const Bitboard occupancy = Utils::GetAllBitboards(board.bitboards, BOTH);
	const int index = ((occupancy & Engine::rookMagics[from].mask) * Engine::rookMagics[from].magic) >> Engine::rookMagics[from].shift;
	Bitboard mask = Engine::rookAttackTable[from][index];

	while (mask) {
		uint8_t to = Utils::PopLSB(mask);

		const bool isEnemyPiece = Utils::IsEnemyPieceAt(board, to);
		const int pieceType = Utils::GetPieceType(board, to);

		if (pieceType == 255 || isEnemyPiece) {
			currentMove.to = to;
			currentMove.capture = isEnemyPiece;
			currentMove.capturedPiece = pieceType;
			moves.push_back(currentMove);
		}
	}
}

void GenerateQueenMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {

	Move currentMove;

	currentMove.promotion = 255;
	currentMove.capture = false;
	currentMove.from = from;
	currentMove.to = 0;

	currentMove.movedPiece = board.turn == WHITE_TURN ? W_QUEEN_I : B_QUEEN_I,

	currentMove.from = from;

	const Bitboard occupancy = Utils::GetAllBitboards(board.bitboards, BOTH);
	Bitboard mask = Utils::GenerateRookAttacksOptimized(from, occupancy) | Utils::GenerateBishopAttacksOptimized(from, occupancy);

	while (mask) {
		uint8_t to = Utils::PopLSB(mask);

		const bool isEnemyPiece = Utils::IsEnemyPieceAt(board, to);
		const int pieceType = Utils::GetPieceType(board, to);

		if (pieceType == 255 || isEnemyPiece) {
			currentMove.to = to;
			currentMove.capture = isEnemyPiece;
			currentMove.capturedPiece = pieceType;
			moves.push_back(currentMove);
		}
	}
}


void GenerateKingMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	Move currentMove;

	currentMove.promotion = 255;
	currentMove.capture = false;
	currentMove.from = from;
	currentMove.to = 0;

	currentMove.from = from;
	currentMove.movedPiece = board.turn == WHITE_TURN ? W_KING_I : B_KING_I;

	Bitboard mask = Engine::kingMasks[from];

	while (mask) {
		uint8_t to = Utils::PopLSB(mask);
		const bool isCapture = Utils::IsEnemyPieceAt(board, to);
		const int pieceType = Utils::GetPieceType(board, to);

		if (pieceType == 255 || isCapture) {
			currentMove.to = to;
			currentMove.capture = isCapture;
			currentMove.capturedPiece = pieceType;

			moves.push_back(currentMove);
		}
	}
}

void GenerateCastlingMoves(Board board, const uint8_t from, std::vector<Move>& moves)
{
	Move currentMove;
	currentMove.from = from;

	currentMove.castling = true;

	currentMove.movedPiece = board.turn == WHITE_TURN ? W_KING_I : B_KING_I;

	if (board.IsCheck(board.turn == !WHITE_TURN ? BLACK : WHITE, false))
		return;

	if (board.turn == WHITE_TURN)
	{
		currentMove.from = 4;
		if (board.wCastlingKing && !board.IsOccupied(5) && !board.IsOccupied(6) && 
			!board.IsSquareAttacked(board.turn == !WHITE_TURN? WHITE : BLACK, 5) && !board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 6)
			&& (board.bitboards[W_ROOK_I] & (1ULL << 7)))
		{
			currentMove.mode = true;
			currentMove.to = 7;
			moves.push_back(currentMove);
		}
		if (board.wCastlingQueen && !board.IsOccupied(1) && !board.IsOccupied(2) && !board.IsOccupied(3) && 
			!board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 2) && !board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 3)
			&& (board.bitboards[W_ROOK_I] & (1ULL << 0)))
		{
			currentMove.mode = false;
			currentMove.to = 0;
			moves.push_back(currentMove);
		}
	}

	else if (board.turn == BLACK_TURN)
	{
		currentMove.from = 60;
		if (board.bCastlingKing && !board.IsOccupied(62) && !board.IsOccupied(61) &&
			!board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 62) && !board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 61)
			&& (board.bitboards[B_ROOK_I] & (1ULL << 63)))
		{
			currentMove.to = 63;
			currentMove.mode = true;
			moves.push_back(currentMove);
		}
		if (board.bCastlingQueen && !board.IsOccupied(59) && !board.IsOccupied(58) && !board.IsOccupied(57) &&
			!board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 58) && !board.IsSquareAttacked(board.turn == !WHITE_TURN ? WHITE : BLACK, 59)
			&& (board.bitboards[B_ROOK_I] & (1ULL << 56)))
		{
			currentMove.to = 56;
			currentMove.mode = false;
			moves.push_back(currentMove);
		}
	}
}

void GeneratePseudoLegalMoves(const Board& board, std::vector<Move>& moves) {
	moves.clear();
	moves.reserve(218);

	bool isWhiteTurn = (board.turn == WHITE_TURN);
	size_t start = isWhiteTurn ? 0 : 6;
	size_t end = isWhiteTurn ? 6 : 12;

	for (size_t i = start; i < end; i++) {
		Bitboard b = board.bitboards[i];
		while (b) {
			size_t j = Utils::PopLSB(b);

			switch (i) {
			case 0: case 6: GeneratePawnMoves(board, j, moves); break;
			case 1: case 7: GenerateKnightMoves(board, j, moves); break;
			case 2: case 8: GenerateBishopMoves(board, j, moves); break;
			case 3: case 9: GenerateRookMoves(board, j, moves); break;
			case 4: case 10: GenerateQueenMoves(board, j, moves); break;
			case 5: case 11:
				GenerateKingMoves(board, j, moves);
				GenerateCastlingMoves(board, j, moves);
				break;
			}
		}
	}
}

#define COPY_MODE
void GenerateLegalMoves(Board& board, std::vector<Move>& pseudoLegalMoves)
{
	GeneratePseudoLegalMoves(board, pseudoLegalMoves);

	std::vector<Move> legalMoves;
	for (const Move& move : pseudoLegalMoves)
	{

#ifdef COPY_MODE
		Board copy = board;
#else
		UndoInfo info = Utils::CreateUndoInfo(board, move);
#endif // COPY_MODE


		if (copy.MovePieceFast(move)) {
			legalMoves.push_back(move);
#ifdef COPY_MODE	
#else 
			board.UndoMove(info);
#endif
		
		}
	}
	pseudoLegalMoves = legalMoves;
}

void MVV_LVA_Order(std::vector<Move>& moves, const Board& board) {
	auto partitionPoint = std::stable_partition(moves.begin(), moves.end(),
		[](const Move& move) { return move.capture; });

	std::sort(moves.begin(), partitionPoint, [&board](const Move& a, const Move& b) {
		const int aValue = MVV_LVA[a.capturedPiece][a.movedPiece];
		const int bValue = MVV_LVA[b.capturedPiece][a.movedPiece];
		return aValue > bValue;
	});
}

void SeparateByCaptures(const std::vector<Move>& moves, std::vector<Move>& captures) {
	captures.clear();
	captures.reserve(moves.size());
	for (const Move& move : moves) {
		if (move.capture) {
			captures.push_back(move);
		}
	}
}
