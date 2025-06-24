#include "move_generation.h"

void GeneratePawnMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {

	Move currentMove =
	{
		.from = 0,
		.to = 0,
		.promotion = 255,
		.capture = false,
	};

	currentMove.from = from;

	bool isWhite = Utils::IsWhitePieceAt(board, from);
	uint8_t rank = from / 8;

	bool canPromote = (isWhite && rank == 6) || (!isWhite && rank == 1);

	uint8_t positionsComprobe[4];

	if (Utils::IsWhitePieceAt(board, from)) {
		positionsComprobe[0] = from + NORTH;
		positionsComprobe[1] = from + NORTH * 2;
		positionsComprobe[2] = from + NORTH_EAST;
		positionsComprobe[3] = from + NORTH_WEST;
	}
	else {
		positionsComprobe[0] = from + SOUTH;
		positionsComprobe[1] = from + SOUTH * 2;
		positionsComprobe[2] = from + SOUTH_EAST;
		positionsComprobe[3] = from + SOUTH_WEST;
	}

	uint8_t promoOptions[4];

	if (isWhite) {
		promoOptions[0] = W_QUEEN_I;
		promoOptions[1] = W_ROOK_I;
		promoOptions[2] = W_BISHOP_I;
		promoOptions[3] = W_KNIGHT_I;
	}
	else {
		promoOptions[0] = B_QUEEN_I;
		promoOptions[1] = B_ROOK_I;
		promoOptions[2] = B_BISHOP_I;
		promoOptions[3] = B_KNIGHT_I;
	}

	for (size_t i = 0; i < 4; i++)
	{
		currentMove.to = positionsComprobe[i];

		if (canPromote) {
			for (size_t j = 0; j < 4; j++)
			{
				currentMove.promotion = promoOptions[j];
				if (board.CanMovePawn(currentMove)) {
					if (Utils::GetPieceType(board, currentMove.to) < 12) {
						currentMove.capture = true;
						currentMove.capturedPiece = Utils::GetPieceType(board, currentMove.to);
					}
					else
						currentMove.capture = false;

					moves.push_back(currentMove);
				}
				currentMove.promotion = 255;
			}
		}
		else
		{
			if (board.CanMovePawn(currentMove)) {
				if (Utils::GetPieceType(board, currentMove.to) < 12) {
					currentMove.capture = true;
					currentMove.capturedPiece = Utils::GetPieceType(board, currentMove.to);
				}
				else
					currentMove.capture = false;

				moves.push_back(currentMove);
			}
		}
	}
}

void GenerateKnightMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	Move currentMove = {
		.from = from,
		.to = 0,
		.promotion = 255,
		.capture = false,
	};

	uint64_t mask = Engine::knightMasks[from];

	for (uint8_t to = 0; to < 64; ++to) {
		if (mask & (1ULL << to)) {
			if (Utils::GetPieceType(board, to) == 255 || Utils::IsEnemyPieceAt(board, to)) {
				currentMove.to = to;
				currentMove.capture = Utils::IsEnemyPieceAt(board, to);
				moves.push_back(currentMove);
			}
		}
	}
}


void GenerateBishopMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	Move currentMove =
	{
		.from = 0,
		.to = 0,
		.promotion = 255,
		.capture = false,
	};

	currentMove.from = from;

	Bitboard mask = Utils::GenerateBishopAttacks(from, Utils::GetAllBitboards(board.bitboards, BOTH));

	for (uint8_t to = 0; to < 64; ++to) {
		if (mask & (1ULL << to)) {
			if (Utils::GetPieceType(board, to) == 255 || Utils::IsEnemyPieceAt(board, to)) {
				currentMove.to = to;
				currentMove.capture = Utils::IsEnemyPieceAt(board, to);
				moves.push_back(currentMove);
			}
		}
	}
}

void GenerateRookMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {

	Move currentMove =
	{
		.from = 0,
		.to = 0,
		.promotion = 255,
		.capture = false,
	};

	currentMove.from = from;

	Bitboard mask = Utils::GenerateRookAttacks(from, Utils::GetAllBitboards(board.bitboards, BOTH));

	for (uint8_t to = 0; to < 64; ++to) {
		if (mask & (1ULL << to)) {
			if (Utils::GetPieceType(board, to) == 255 || Utils::IsEnemyPieceAt(board, to)) {
				currentMove.to = to;

				currentMove.capture = Utils::IsEnemyPieceAt(board, to);
				moves.push_back(currentMove);
			}
		}
	}
}

void GenerateQueenMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	GenerateBishopMoves(board, from, moves);
	GenerateRookMoves(board, from, moves);
}


void GenerateKingMoves(const Board& board, const uint8_t from, std::vector<Move>& moves) {
	Move currentMove =
	{
		.from = 0,
		.to = 0,
		.promotion = 255,
		.capture = false,
	};

	currentMove.from = from;

	const uint8_t positionsComprobe[8] = {
		from + NORTH, from + SOUTH, from + EAST, from + WEST,
		from + NORTH_EAST, from + NORTH_WEST, from + SOUTH_EAST, from + SOUTH_WEST
	};

	for (size_t i = 0; i < 8; i++)
	{
		currentMove.to = positionsComprobe[i];

		if (board.CanMoveKing(currentMove)) {
			if (Utils::GetPieceType(board, currentMove.to) < 12) {
				currentMove.capture = true;
				currentMove.capturedPiece = Utils::GetPieceType(board, currentMove.to);
			}
			else
				currentMove.capture = false;
			moves.push_back(currentMove);
		}
	}
}

void GenerateCastlingMoves(const Board& board, const uint8_t from, std::vector<Move>& moves)
{
	Move currentMove;

	if (board.turn == WHITE_TURN)
	{
		if (board.wCastlingKing && !board.IsOccupied(5) && !board.IsOccupied(6))
		{
			currentMove.castling = true;
			currentMove.mode = false;
			moves.push_back(currentMove);
		}
		else if (board.wCastlingKing && !board.IsOccupied(1) && !board.IsOccupied(2) && !board.IsOccupied(3))
		{
			currentMove.castling = true;
			currentMove.mode = true;
			moves.push_back(currentMove);
		}
	}

	else if (board.turn == BLACK_TURN)
	{
		if (board.bCastlingKing && !board.IsOccupied(62) && !board.IsOccupied(61))
		{
			currentMove.castling = true;
			currentMove.mode = false;
			moves.push_back(currentMove);
		}
		else if (board.bCastlingKing && !board.IsOccupied(59) && !board.IsOccupied(58) && !board.IsOccupied(57))
		{
			currentMove.castling = true;
			currentMove.mode = true;
			moves.push_back(currentMove);
		}
	}
}

void GeneratePseudoLegalMoves(const Board& board, std::vector<Move>& moves) {
	moves.clear();
	moves.reserve(64);

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

std::vector<Move> GenerateLegalMoves(Board& board)
{
	std::vector<Move> pseudoLegalMoves;
	GeneratePseudoLegalMoves(board, pseudoLegalMoves);

	std::vector<Move> legalMoves;
	for (const Move& move : pseudoLegalMoves)
	{
		UndoInfo info = Utils::CreateUndoInfo(board, move);
		// Board copy = board;

		if (board.MovePieceFast(move)) {
			legalMoves.push_back(move);
			
			board.UndoMove(info);
		}
	}
	return legalMoves;
}


std::vector<Move> MVV_LVA_Order(const std::vector<Move>& moves, const Board& board) {
	std::vector<Move> captures;
	std::vector<Move> nonCaptures;
	std::vector<Move> orderedMoves;

	for (const Move& move : moves) {
		if (move.capture) {
			captures.push_back(move);
			// std::cout << "Capture move\n";
		}
		else {
			nonCaptures.push_back(move);
			// std::cout << "Non capture move\n";
		}
	}


	std::sort(captures.begin(), captures.end(), [&board](const Move& a, const Move& b) {
		int aValue = 0;
		int bValue = 0;

		aValue = 10 * STATIC_PIECE_VALUE[a.capturedPiece] - STATIC_PIECE_VALUE[Utils::GetPieceType(board, a.from)];
		bValue = 10 * STATIC_PIECE_VALUE[b.capturedPiece] - STATIC_PIECE_VALUE[Utils::GetPieceType(board, b.from)];

		return aValue > bValue;
		});
	orderedMoves.assign(captures.begin(), captures.end());
	orderedMoves.insert(orderedMoves.end(), nonCaptures.begin(), nonCaptures.end());

	if (captures.size() > 0) {
		//Utils::PrintBoard(board);
		//std::cout << "CAPTURE 0: " << Utils::ConvertToBoardPosition(captures[0].from) << Utils::ConvertToBoardPosition(captures[0].to) << std::endl;
	}

	return orderedMoves;
}



