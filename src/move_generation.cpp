#include "move_generation.h"

std::vector<Move> GeneratePawnMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

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

	for (size_t i = 0; i < 4; i++)
	{
		currentMove.to = positionsComprobe[i];

		if (canPromote) {
			for (size_t j = 0; j < 12; j++)
			{
				currentMove.promotion = j;
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

	return moves;
}

std::vector<Move> GenerateKnightMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

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

	return moves;
}


std::vector<Move> GenerateBishopMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

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

	return moves;
}

std::vector<Move> GenerateRookMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

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

	return moves;
}

std::vector<Move> GenerateQueenMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

	std::vector<Move> bishopMoves = GenerateBishopMoves(board, from);
	std::vector<Move> rookMoves = GenerateRookMoves(board, from);

	moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
	moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());

	return moves;
}


std::vector<Move> GenerateKingMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

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

	return moves;
}

std::vector<Move> GenerateCastlingMoves(const Board& board, const uint8_t from)
{
	std::vector<Move> moves;

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

	return moves;
}

std::vector<Move> GeneratePseudoLegalMoves(const Board& board)
{
	std::vector<Move> moves;

	for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = 0; j < 64; j++)
		{
			if (Utils::GetBitboardValueOnIndex(board.bitboards[i], j) == 1)
			{
				if (i == 0 || i == 6)
				{
					const std::vector<Move> pawnMoves = GeneratePawnMoves(board, j);
					moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
				}
				else if (i == 1 || i == 7)
				{
					std::vector<Move> knightMoves = GenerateKnightMoves(board, j);
					moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
				}
				else if (i == 2 || i == 8)
				{
					std::vector<Move> bishopMoves = GenerateBishopMoves(board, j);
					moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
				}
				else if (i == 3 || i == 9)
				{
					std::vector<Move> rookMoves = GenerateRookMoves(board, j);
					moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
				}
				else if (i == 4 || i == 10)
				{
					std::vector<Move> queenMoves = GenerateQueenMoves(board, j);
					moves.insert(moves.end(), queenMoves.begin(), queenMoves.end());
				}
				else if (i == 5 || i == 11)
				{
					std::vector<Move> kingMoves = GenerateKingMoves(board, j);
					moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());

					std::vector<Move> castlingMoves = GenerateCastlingMoves(board, j);
					moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());
				}
			}
		}
	}

	return moves;
}

std::vector<Move> GenerateLegalMoves(Board& board)
{
	std::vector<Move> pseudoLegalMoves = GeneratePseudoLegalMoves(board);

	std::vector<Move> legalMoves;
	for (const Move& move : pseudoLegalMoves)
	{
		UndoInfo info = Utils::CreateUndoInfo(board, move);

		if (board.MovePiece(move)) {
			legalMoves.push_back(move);
			board.UndoMove(info);
		}
		else
		{
			// std::cout << "ahjhsakjdj";
		}
	}
	return legalMoves;
}


std::vector<Move> MVV_LVA_Order(const std::vector<Move>& moves, const Board& board, uint8_t depth) {
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
