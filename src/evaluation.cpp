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

// this is only for print evaluation
int Evaluation::EvaluatePiece(const Board& board, const uint8_t index)
{
	const uint8_t type = Utils::GetPieceType(board, index);
	const uint8_t sq = index;

	int eval = 0;

	switch (type) {
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

	return eval;
}

int Evaluation::EvaluatePieceSimple(const Board& board, const uint8_t index)
{
	const uint8_t type = Utils::GetPieceType(board, index);
	const uint8_t sq = index;

	int eval = 0;

	switch (type) {
		case W_PAWN_I:   eval = STATIC_PIECE_VALUE[W_PAWN_I]	 ;break;
		case W_KNIGHT_I: eval = STATIC_PIECE_VALUE[W_KNIGHT_I]	 ;break;
		case W_BISHOP_I: eval = STATIC_PIECE_VALUE[W_BISHOP_I]	 ;break;
		case W_ROOK_I:   eval = STATIC_PIECE_VALUE[W_ROOK_I]	 ;break;
		case W_QUEEN_I:  eval = STATIC_PIECE_VALUE[W_QUEEN_I]	 ;break;
																 ;break;
		case B_PAWN_I:   eval = STATIC_PIECE_VALUE[B_PAWN_I]	 ;break;
		case B_KNIGHT_I: eval = STATIC_PIECE_VALUE[B_KNIGHT_I]	 ;break;
		case B_BISHOP_I: eval = STATIC_PIECE_VALUE[B_BISHOP_I]	 ;break;
		case B_ROOK_I:   eval = STATIC_PIECE_VALUE[B_ROOK_I] 	 ;break;
		case B_QUEEN_I:  eval = STATIC_PIECE_VALUE[B_QUEEN_I];    break;
	}

	return eval;
}

// TODO: unifique PrintEvaluation and PrintSimpleEvaluation
void Evaluation::PrintEvaluation(const Board& board)
{
	auto CenterText = [](const std::string& text, int width) -> std::string {
		int padding = width - (int)text.size();
		if (padding <= 0) return text;
		int left = padding / 2;
		int right = padding - left;
		return std::string(left, ' ') + text + std::string(right, ' ');
	};

	std::cout << "\n";
	for (int rank = 7; rank >= 0; rank--) {
		std::cout << "+-------+-------+-------+-------+-------+-------+-------+-------+\n";

		for (size_t i = 0; i < 8; i++) {
			if (Utils::GetPieceType(board, rank * 8 + i) > 11) {
				std::cout << "|       ";
			}
			else {
				std::string piece(1, PIECE_CHAR[Utils::GetPieceType(board, rank * 8 + i)]);
				std::cout << "|" << CenterText(piece, 7);
			}
		}
		std::cout << "|\n";

		for (size_t i = 0; i < 8; i++) {
			float score = (float)(EvaluatePiece(board, rank * 8 + i)) / 100.0f;
			if (Utils::GetPieceType(board, rank * 8 + i) > 11) {
				std::cout << "|       ";
				continue;
			}

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << score;
			std::string scoreStr = ss.str();
			if (score > 0) scoreStr = "+" + scoreStr;

			std::cout << "|" << CenterText(scoreStr, 7);
		}
		std::cout << "|\n";
	}

	std::cout << "+-------+-------+-------+-------+-------+-------+-------+-------+\n\n";
	std::cout << "Final Evaluation: " << std::fixed << std::setprecision(2) << (float)Evaluate(board) / 100 << ".\n\n";
}


void Evaluation::PrintSimpleEvaluation(const Board& board)
{
	auto CenterText = [](const std::string& text, int width) -> std::string {
		int padding = width - (int)text.size();
		if (padding <= 0) return text;
		int left = padding / 2;
		int right = padding - left;
		return std::string(left, ' ') + text + std::string(right, ' ');
		};

	std::cout << "\n";
	for (int rank = 7; rank >= 0; rank--) {
		std::cout << "+-------+-------+-------+-------+-------+-------+-------+-------+\n";

		for (size_t i = 0; i < 8; i++) {
			if (Utils::GetPieceType(board, rank * 8 + i) > 11) {
				std::cout << "|       ";
			}
			else {
				std::string piece(1, PIECE_CHAR[Utils::GetPieceType(board, rank * 8 + i)]);
				std::cout << "|" << CenterText(piece, 7);
			}
		}
		std::cout << "|\n";

		for (size_t i = 0; i < 8; i++) {
			float score = (float)(EvaluatePieceSimple(board, rank * 8 + i)) / 100.0f;
			if (Utils::GetPieceType(board, rank * 8 + i) > 11) {
				std::cout << "|       ";
				continue;
			}

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << score;
			std::string scoreStr = ss.str();
			if (score > 0) scoreStr = "+" + scoreStr;

			std::cout << "|" << CenterText(scoreStr, 7);
		}
		std::cout << "|\n";
	}

	std::cout << "+-------+-------+-------+-------+-------+-------+-------+-------+\n\n";
	std::cout << "Final Evaluation: " << std::fixed << std::setprecision(2) << (float)SimpleEvaluation(board) / 100 << ".\n\n";
}

