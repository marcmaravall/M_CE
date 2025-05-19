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

	bool canPromote = false;

	if (from < 8 || from > 55)
		canPromote = true;

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

	Move currentMove =
	{
			.from = 0,
			.to = 0,
			.promotion = 255,
			.capture = false,
	};

	currentMove.from = from;

	uint8_t positionsComprobe[8] = {
		from + 17, from + 15, from + 10, from + 6,
		from - 17, from - 15, from - 10, from - 6
	};

	for (size_t i = 0; i < 8; i++)
	{
		currentMove.to = positionsComprobe[i];

		if (board.CanMoveKnight(currentMove)) {
			if (Utils::GetPieceType(board, currentMove.to) < 12) {
				currentMove.capture = true;
				currentMove.capturedPiece = Utils::GetPieceType(board, currentMove.to);
			}
			else currentMove.capture = false;
			moves.push_back(currentMove);
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

	for (size_t i = 0; i < 64; i++)
	{
		currentMove.to = i;

		if (board.CanMoveBishop(currentMove)) {
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

	for (size_t i = 0; i < 64; i++)
	{
		currentMove.to = i;

		if (board.CanMoveRook(currentMove)) {
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

std::vector<Move> GenerateQueenMoves(const Board& board, const uint8_t from) {
	std::vector<Move> moves;

	Move currentMove =
	{
			.from = 0,
			.to = 0,
			.promotion = 255,
			.capture = false,
	};

	currentMove.from = from;

	for (size_t i = 0; i < 64; i++)
	{
		currentMove.to = i;

		if (board.CanMoveQueen(currentMove)) {
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
				}
			}
		}
	}

	return moves;
}

std::vector<Move> GenerateLegalMoves(const Board& board)
{
	std::vector<Move> pseudoLegalMoves = GeneratePseudoLegalMoves(board);

	std::vector<Move> legalMoves;
	for (const Move& move : pseudoLegalMoves)
	{
		Board copy = board;
		if (copy.MovePiece(move)) // deja el rey fuera de jaque
			legalMoves.push_back(move);
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
