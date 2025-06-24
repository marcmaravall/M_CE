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

	for (int i = 0; i < 12; i++) {
		Bitboard b = board.bitboards[i];
		while (b) {
			int sq = Utils::PopLSB(b);

			int eval = 0;

			switch (i) {
			case W_PAWN_I:   eval = STATIC_PIECE_VALUE[W_PAWN_I] + W_PAWN_BITMAP[sq]; break;
			case W_KNIGHT_I: eval = STATIC_PIECE_VALUE[W_KNIGHT_I] + W_KNIGHT_BITMAP[sq]; break;
			case W_BISHOP_I: eval = STATIC_PIECE_VALUE[W_BISHOP_I] + W_BISHOP_BITMAP[sq]; break;
			case W_ROOK_I:   eval = STATIC_PIECE_VALUE[W_ROOK_I] + W_ROOK_BITMAP[sq]; break;
			case W_QUEEN_I:  eval = STATIC_PIECE_VALUE[W_QUEEN_I] + W_QUEEN_BITMAP[sq]; break;

			case B_PAWN_I:   eval = STATIC_PIECE_VALUE[B_PAWN_I] - B_PAWN_BITMAP[sq]; break;
			case B_KNIGHT_I: eval = STATIC_PIECE_VALUE[B_KNIGHT_I] - B_KNIGHT_BITMAP[sq]; break;
			case B_BISHOP_I: eval = STATIC_PIECE_VALUE[B_BISHOP_I] - B_BISHOP_BITMAP[sq]; break;
			case B_ROOK_I:   eval = STATIC_PIECE_VALUE[B_ROOK_I] - B_ROOK_BITMAP[sq]; break;
			case B_QUEEN_I:  eval = STATIC_PIECE_VALUE[B_QUEEN_I] - B_QUEEN_BITMAP[sq]; break;
			}

			res += eval;
		}
	}

	return res;
}

