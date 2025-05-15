#include "move_generation.h"

std::vector<Move> GeneratePawnMoves(Board board, uint8_t from) {
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

	for (size_t i = 0; i < 64; i++)
	{
		currentMove.to = i;

		if (canPromote) {
			for (size_t j = 0; j < 12; j++)
			{
				currentMove.promotion = j;
				if (board.CanMovePawn(currentMove))
					moves.push_back(currentMove);
			}
		}
		else
		{
			if (board.CanMovePawn(currentMove))
				moves.push_back(currentMove);
		}
	}

	return moves;
}

std::vector<Move> GenerateKnightMoves(Board board, uint8_t from) {
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

		if (board.CanMoveKnight(currentMove))
			moves.push_back(currentMove);
	}

	return moves;
}

std::vector<Move> GenerateBishopMoves(Board board, uint8_t from) {
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

		if (board.CanMoveBishop(currentMove))
			moves.push_back(currentMove);
	}

	return moves;
}

std::vector<Move> GenerateRookMoves(Board board, uint8_t from) {
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

		if (board.CanMoveRook(currentMove))
			moves.push_back(currentMove);
	}

	return moves;
}

std::vector<Move> GenerateQueenMoves(Board board, uint8_t from) {
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

		if (board.CanMoveQueen(currentMove))
			moves.push_back(currentMove);
	}

	return moves;
}

std::vector<Move> GenerateKingMoves(Board board, uint8_t from) {
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

		if (board.CanMoveKing(currentMove))
			moves.push_back(currentMove);
	}

	return moves;
}

std::vector<Move> GeneratePseudoLegalMoves(const Board board)
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
					std::vector<Move> pawnMoves = GeneratePawnMoves(board, j);
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



std::vector<Move> GenerateLegalMoves(const Board board)
{
	std::vector<Move> pseudoLegalMoves = GeneratePseudoLegalMoves(board);

	Board buffer = board;

	for (int i = pseudoLegalMoves.size() - 1; i >= 0; --i)
	{
		buffer = board;
		Move currentMove = pseudoLegalMoves[i];

		if (!buffer.MovePiece(currentMove))
			pseudoLegalMoves.erase(pseudoLegalMoves.begin() + i);
	}
	return pseudoLegalMoves;
}

