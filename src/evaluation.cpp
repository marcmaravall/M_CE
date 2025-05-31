#include "evaluation.h"

int Evaluation::SimpleEvaluation(const Board& board)
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

	for (size_t i = 0; i < 64; i++) {
		if (board.IsOccupied(i))
		{
			uint8_t piece = 255;
			for (size_t j = 0; j < 12; j++) {
				if (Utils::GetBitboardValueOnIndex(board.bitboards[j], i))
				{
					if (j == W_PAWN_I)
						res += (STATIC_PIECE_VALUE[j] + W_PAWN_BITMAP[i]);
					else if (j == W_KNIGHT_I)
						res += (STATIC_PIECE_VALUE[j] + W_KNIGHT_BITMAP[i]);
					else if (j == W_BISHOP_I)
						res += (STATIC_PIECE_VALUE[j] + W_BISHOP_BITMAP[i]);
					else if (j == W_ROOK_I)
						res += (STATIC_PIECE_VALUE[j] + W_ROOK_BITMAP[i]);
					else if (j == W_QUEEN_I)
						res += (STATIC_PIECE_VALUE[j] + W_QUEEN_BITMAP[i]);


					else if (j == B_PAWN_I)
						res += (STATIC_PIECE_VALUE[j] - B_PAWN_BITMAP[i]);
					else if (j == B_KNIGHT_I)
						res += (STATIC_PIECE_VALUE[j] - B_KNIGHT_BITMAP[i]);
					else if (j == B_BISHOP_I)
						res += (STATIC_PIECE_VALUE[j] - B_BISHOP_BITMAP[i]);
					else if (j == B_ROOK_I)
						res += (STATIC_PIECE_VALUE[j] - B_ROOK_BITMAP[i]);
					else if (j == B_QUEEN_I)
						res += (STATIC_PIECE_VALUE[j] - B_QUEEN_BITMAP[i]);
				}
			}
		}
	}

	return res;
}
