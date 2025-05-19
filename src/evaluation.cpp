#include "evaluation.h"

int Evaluation::SimpleEvaluation(const Board board)
{
	int res = 0;

	for (size_t i = 0; i < 64; i++) {
		for (size_t i = 0; i < 12; i++) {
			res += STATIC_PIECE_VALUE[i];
		}
	}

	return res;
}


int Evaluation::Evaluate(const Board& board)
{
	int res = 0;

	for (int i = 0; i < 6; i++)
	{
		uint64_t bitboard = board.bitboards[i];
		while (bitboard)
		{
			int square = Utils::PopLSB(bitboard);
			res += STATIC_PIECE_VALUE[i] + PIECE_SQUARE_TABLES[i][square];
		}
	}

	for (int i = 6; i < 12; i++)
	{
		uint64_t bitboard = board.bitboards[i];
		while (bitboard)
		{
			int square = Utils::PopLSB(bitboard);
			res += STATIC_PIECE_VALUE[i] - PIECE_SQUARE_TABLES[i][square];
		}
	}

	return res;
}
