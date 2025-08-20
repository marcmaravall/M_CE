#include "evaluation.h"

// TODO: simplify some parts of the code (only optional).

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
	return EvaluateV2(board);
	int res = 0;

	for (int i = 0; i < 12; i++) {
		Bitboard b = board.bitboards[i];
		while (b) {
			int sq = Utils::PopLSB(b);

			int eval = 0;

			switch (i) {
			case W_PAWN_I:   eval = STATIC_PIECE_VALUE[W_PAWN_I] + W_PAWN_BITMAP_OPENING[sq]; break;
			case W_KNIGHT_I: eval = STATIC_PIECE_VALUE[W_KNIGHT_I] + W_KNIGHT_BITMAP_OPENING[sq]; break;
			case W_BISHOP_I: eval = STATIC_PIECE_VALUE[W_BISHOP_I] + W_BISHOP_BITMAP_OPENING[sq]; break;
			case W_ROOK_I:   eval = STATIC_PIECE_VALUE[W_ROOK_I] + W_ROOK_BITMAP_OPENING[sq]; break;
			case W_QUEEN_I:  eval = STATIC_PIECE_VALUE[W_QUEEN_I] + W_QUEEN_BITMAP_OPENING[sq]; break;

			case B_PAWN_I:   eval = STATIC_PIECE_VALUE[B_PAWN_I] - B_PAWN_BITMAP_OPENING[sq]; break;
			case B_KNIGHT_I: eval = STATIC_PIECE_VALUE[B_KNIGHT_I] - B_KNIGHT_BITMAP_OPENING[sq]; break;
			case B_BISHOP_I: eval = STATIC_PIECE_VALUE[B_BISHOP_I] - B_BISHOP_BITMAP_OPENING[sq]; break;
			case B_ROOK_I:   eval = STATIC_PIECE_VALUE[B_ROOK_I] - B_ROOK_BITMAP_OPENING[sq]; break;
			case B_QUEEN_I:  eval = STATIC_PIECE_VALUE[B_QUEEN_I] - B_QUEEN_BITMAP_OPENING[sq]; break;
			}

			res += eval;
		}
	}

	return res;
}

int Evaluation::EvaluateV2(const Board& board)
{
	const GAME_PHASE gamePhase = GetGamePhase(board);
	const double gamePhaseValue = GetGamePhaseValue(board);

	int evaluation = 0;

	evaluation += EvaluatePawnStruct(board, gamePhase);
	evaluation += EvaluateKingsSafety(board, gamePhase);
	evaluation += EvaluateBishopPair(board, gamePhase);

	for (int i = 0; i < 12; i++) {
		Bitboard b = board.bitboards[i];
		while (b) {
			int sq = Utils::PopLSB(b);

			int eval = 0;

			switch (i) {
			case W_PAWN_I:	 eval = STATIC_PIECE_VALUE[W_PAWN_I];   break;
			case W_KNIGHT_I: eval = STATIC_PIECE_VALUE[W_KNIGHT_I]; break;
			case W_BISHOP_I: eval = STATIC_PIECE_VALUE[W_BISHOP_I]; break;
			case W_ROOK_I:   eval = STATIC_PIECE_VALUE[W_ROOK_I]  ; break;
			case W_QUEEN_I:  eval = STATIC_PIECE_VALUE[W_QUEEN_I]; break;

			case B_PAWN_I:	 eval = STATIC_PIECE_VALUE[B_PAWN_I];   break;
			case B_KNIGHT_I: eval = STATIC_PIECE_VALUE[B_KNIGHT_I]; break;
			case B_BISHOP_I: eval = STATIC_PIECE_VALUE[B_BISHOP_I]; break;
			case B_ROOK_I:   eval = STATIC_PIECE_VALUE[B_ROOK_I]  ; break;
			case B_QUEEN_I:  eval = STATIC_PIECE_VALUE[B_QUEEN_I] ; break;
			}

			eval += i < 6? GetPSEValue(sq, i, gamePhase, gamePhaseValue) : -GetPSEValue(sq, i, gamePhase, gamePhaseValue);

			evaluation += eval;
		}
	}

	return evaluation;
}

int Evaluation::EvaluateBishopPair(const Board& board, const GAME_PHASE phase)
{
	int evaluation = 0;

	Bitboard whiteBishops = board.bitboards[W_BISHOP_I];
	Bitboard blackBishops = board.bitboards[B_BISHOP_I];

	uint8_t wBishopCount = 0, bBishopCount = 0;

	while (whiteBishops)
	{
		const uint8_t index = Utils::PopLSB(whiteBishops);

		wBishopCount++;
	}

	while (blackBishops)
	{
		const uint8_t index = Utils::PopLSB(blackBishops);
		bBishopCount++;
	}
	
	if (wBishopCount >= 2) {
		if (phase == OPENING)
			evaluation += 30;
		else if (phase == MIDGAME)
			evaluation += 50;
		else if (phase == ENDGAME)
			evaluation += 75;
	}

	if (bBishopCount >= 2) {
		if (phase == OPENING)
			evaluation -= 30;
		else if (phase == MIDGAME)
			evaluation -= 50;
		else if (phase == ENDGAME)
			evaluation -= 75;
	}

	return evaluation; 
}

// Structural evaluation of pawns ----------
// if a pawn is defended by another pawn, it is a good structure.  +10
// if a pawn is isolated, it is a bad structure.				   -20
// if a pawn is doubled, it is a bad structure.					   -30
// if a pawn is backward, it is a bad structure.				   -10
// if a pawn is passed, it is a good structure.					   +40

inline constexpr const int defendedScore = +10;
inline constexpr const int isolatedScore = -20;
inline constexpr const int doubledScore = -30;
inline constexpr const int backwardScore = -10;
inline constexpr const int passedScore = +40;

int Evaluation::EvaluatePawnStruct(const Board& board, const GAME_PHASE phase) {
	int evaluation = 0;
	Bitboard whitePawns = board.bitboards[0];
	Bitboard blackPawns = board.bitboards[6];
	Bitboard wpCopy = whitePawns;

	while (wpCopy)
	{
		int pawnEvaluation = 100;

		const uint8_t index = Utils::PopLSB(wpCopy);
		const uint8_t rank = index / 8;
		const uint8_t file = index % 8;

		const Bitboard fileBitboard = FILES_MASKS[file];
		const Bitboard pawnMask = (1ULL << index);

		if (file != 0 && (whitePawns & (pawnMask >> 9))) {
			pawnEvaluation += defendedScore;
		}
		if (file != 7 && (whitePawns & (pawnMask >> 7))) {
			pawnEvaluation += defendedScore;
		}

		if (fileBitboard & (whitePawns & ~pawnMask)) {
			pawnEvaluation += doubledScore;
		}

		bool noLeft = (file == 0) || !(whitePawns & FILES_MASKS[file - 1]);
		bool noRight = (file == 7) || !(whitePawns & FILES_MASKS[file + 1]);
		if (noLeft && noRight) {
			pawnEvaluation += isolatedScore;
		}

		/*Bitboard frontMask = PASSED_PAWN_MASKS_WHITE[file][rank];
		if ((frontMask & blackPawns) == 0) {
			pawnEvaluation += passedScore;
		}*/

		evaluation += pawnEvaluation;
	}

	Bitboard bpCopy = blackPawns;

	while (bpCopy)
	{
		int pawnEvaluation = -100;

		const uint8_t index = Utils::PopLSB(bpCopy);
		const uint8_t rank = index / 8;
		const uint8_t file = index % 8;

		const Bitboard fileBitboard = FILES_MASKS[file];
		const Bitboard pawnMask = (1ULL << index);

		if (file != 0 && (blackPawns & (pawnMask << 7))) {
			pawnEvaluation -= defendedScore;
		}
		if (file != 7 && (blackPawns & (pawnMask << 9))) {
			pawnEvaluation -= defendedScore;
		}

		if (fileBitboard & (blackPawns & ~pawnMask)) {
			pawnEvaluation -= doubledScore;
		}

		bool noLeft = (file == 0) || !(blackPawns & FILES_MASKS[file - 1]);
		bool noRight = (file == 7) || !(blackPawns & FILES_MASKS[file + 1]);
		if (noLeft && noRight) {
			pawnEvaluation -= isolatedScore;
		}

		/*
		Bitboard frontMask = PASSED_PAWN_MASKS_BLACK[file][rank];
		if ((frontMask & whitePawns) == 0) {
			pawnEvaluation -= passedScore;
		}*/

		evaluation += pawnEvaluation;
	}

	return evaluation;
}

int Evaluation::EvaluateKingsSafety(const Board& position, const GAME_PHASE phase)
{
	int evaluation = 0;

	if (phase == OPENING || phase == MIDGAME)
	{
		Bitboard wKing = position.bitboards[W_KING_I];
		while (wKing)
		{
			const uint8_t index = Utils::PopLSB(wKing);
			if (index < 16)
				evaluation += 20;
		}

		Bitboard bKing = position.bitboards[B_KING_I];
		while (bKing)
		{
			const uint8_t index = Utils::PopLSB(bKing);
			if (index >= 48)
				evaluation -= 20;
		}
	}
	else // ENDGAME
	{
		Bitboard wKing = position.bitboards[W_KING_I];
		while (wKing)
		{
			const uint8_t index = Utils::PopLSB(wKing);
			// evaluation += KING_ENDGAME_BITMAP[index];
		}

		Bitboard bKing = position.bitboards[B_KING_I];
		while (bKing)
		{
			const uint8_t index = Utils::PopLSB(bKing);
			// evaluation -= KING_ENDGAME_BITMAP[index];
		}
	}

	return evaluation;
}

GAME_PHASE Evaluation::GetGamePhase(const Board& board)
{
	int whiteMaterial = 0;
	int blackMaterial = 0;
	for (int i = 0; i < 12; i++) {
		if (i == 5 || i == 11)
			continue;
		Bitboard b = board.bitboards[i];
		while (b) {
			int sq = Utils::PopLSB(b);
			if (i < 6) {
				whiteMaterial = GAME_PHASE_VALUE[i];
			}
			else {
				blackMaterial -= GAME_PHASE_VALUE[i-6];
			}
		}
	}
	if (whiteMaterial - blackMaterial <= ENDGAME_VALUE) {
		return ENDGAME;
	}
	else if (whiteMaterial - blackMaterial <= MIDGAME_VALUE) {
		return MIDGAME;
	}
	else {
		return OPENING;
	}
}

// this is only for print evaluation
int Evaluation::EvaluatePiece(const Board& board, const uint8_t index)
{
	const uint8_t type = Utils::GetPieceType(board, index);
	const uint8_t sq = index;

	int eval = 0;

	switch (type) {
		case W_PAWN_I:   eval = STATIC_PIECE_VALUE[W_PAWN_I] + W_PAWN_BITMAP_OPENING[sq]; break;
		case W_KNIGHT_I: eval = STATIC_PIECE_VALUE[W_KNIGHT_I] + W_KNIGHT_BITMAP_OPENING[sq]; break;
		case W_BISHOP_I: eval = STATIC_PIECE_VALUE[W_BISHOP_I] + W_BISHOP_BITMAP_OPENING[sq]; break;
		case W_ROOK_I:   eval = STATIC_PIECE_VALUE[W_ROOK_I] + W_ROOK_BITMAP_OPENING[sq]; break;
		case W_QUEEN_I:  eval = STATIC_PIECE_VALUE[W_QUEEN_I] + W_QUEEN_BITMAP_OPENING[sq]; break;

		case B_PAWN_I:   eval = STATIC_PIECE_VALUE[B_PAWN_I] - B_PAWN_BITMAP_OPENING[sq]; break;
		case B_KNIGHT_I: eval = STATIC_PIECE_VALUE[B_KNIGHT_I] - B_KNIGHT_BITMAP_OPENING[sq]; break;
		case B_BISHOP_I: eval = STATIC_PIECE_VALUE[B_BISHOP_I] - B_BISHOP_BITMAP_OPENING[sq]; break;
		case B_ROOK_I:   eval = STATIC_PIECE_VALUE[B_ROOK_I] - B_ROOK_BITMAP_OPENING[sq]; break;
		case B_QUEEN_I:  eval = STATIC_PIECE_VALUE[B_QUEEN_I] - B_QUEEN_BITMAP_OPENING[sq]; break;
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
	return Evaluation::PrintEvaluationV2(board);
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


void Evaluation::PrintEvaluationV2(const Board& position)
{
	std::cout << "+------------+------------+\n";
	std::cout << "Pawn Structure Evaluation: " << EvaluatePawnStruct(position, GetGamePhase(position)) << "\n";
	std::cout << "Kings Safety Evaluation  : " << EvaluateKingsSafety(position, GetGamePhase(position)) << "\n";
	std::cout << "Bishop Pair Evaluation   : " << EvaluateBishopPair(position, GetGamePhase(position)) << "\n";
	std::cout << "+------------+------------+\n\n";
	
	auto CenterText = [](const std::string& text, int width) -> std::string {
		int padding = width - (int)text.size();
		if (padding <= 0) return text;
		int left = padding / 2;
		int right = padding - left;
		return std::string(left, ' ') + text + std::string(right, ' ');
	};

	for (int rank = 7; rank >= 0; rank--) {
		std::cout << "+-------+-------+-------+-------+-------+-------+-------+-------+\n";

		for (size_t i = 0; i < 8; i++) {
			if (Utils::GetPieceType(position, rank * 8 + i) > 11) {
				std::cout << "|       ";
			}
			else {
				std::string piece(1, PIECE_CHAR[Utils::GetPieceType(position, rank * 8 + i)]);
				std::cout << "|" << CenterText(piece, 7);
			}
		}
		std::cout << "|\n";

		for (size_t i = 0; i < 8; i++) {
			float score = (float)(EvaluatePieceV2(position, rank * 8 + i)) / 100.0f;
			if (Utils::GetPieceType(position, rank * 8 + i) > 11) {
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


	std::cout << "Final Evaluation: " << std::fixed << std::setprecision(2) << (float)EvaluateV2(position) / 100 << ".\n\n";
}

int Evaluation::EvaluatePawn(const uint8_t index, const Board& board, const GAME_PHASE phase)
{
	const uint8_t type = Utils::GetPieceType(board, index);
	const bool isWhite = type < 6; 
	int evaluation = 0; //isWhite? +100 : -100;

	const Bitboard whitePawns = board.bitboards[W_PAWN_I];
	const Bitboard blackPawns = board.bitboards[B_PAWN_I];

	const Bitboard myPieces = isWhite ? whitePawns : blackPawns;
	const Bitboard oponentPawns = isWhite ? blackPawns : whitePawns;

	const uint8_t directionDefendedE = isWhite ? SOUTH_EAST : NORTH_EAST;
	const uint8_t directionDefendedW = isWhite ? SOUTH_WEST : NORTH_WEST;

	const uint8_t rank = index / 8;
	const uint8_t file = index % 8;

	const Bitboard fileBitboard = FILES_MASKS[file];
	const Bitboard pawnMask = (1ULL << index);

	if (file != 0 && (myPieces & (1ULL << (index + directionDefendedW)))) {
		evaluation += isWhite ? defendedScore:-defendedScore;
	}
	if (file != 7 && (myPieces & (1ULL << (index + directionDefendedE)))) {
		evaluation += isWhite ? defendedScore : -defendedScore;
	}

	if (fileBitboard & (myPieces & ~pawnMask)) {
		evaluation += isWhite? doubledScore : -doubledScore;
	}

	const bool noLeft = (file == 0) || !(myPieces & FILES_MASKS[file - 1]);
	const bool noRight = (file == 7) || !(myPieces & FILES_MASKS[file + 1]);
	if (noLeft && noRight) {
		evaluation += isWhite? isolatedScore : -isolatedScore;
	}

	return evaluation;
}

// TODO: simplify some logic.
int Evaluation::EvaluatePieceV2(const Board& board, const uint8_t index)
{
	const uint8_t type = Utils::GetPieceType(board, index);
	int evaluation = 0;
	if (type > 11) return 0;

	if (type == 0 || type == 6) {
		evaluation += EvaluatePawn(index, board, GetGamePhase(board));
	}

	switch (type) {
	case W_PAWN_I:   evaluation += STATIC_PIECE_VALUE[W_PAWN_I]  ; break;
	case W_KNIGHT_I: evaluation += STATIC_PIECE_VALUE[W_KNIGHT_I]; break;
	case W_BISHOP_I: evaluation += STATIC_PIECE_VALUE[W_BISHOP_I]; break;
	case W_ROOK_I:   evaluation += STATIC_PIECE_VALUE[W_ROOK_I]  ; break;
	case W_QUEEN_I:  evaluation += STATIC_PIECE_VALUE[W_QUEEN_I] ; break;
																 ; 
	case B_PAWN_I:   evaluation += STATIC_PIECE_VALUE[B_PAWN_I]  ; break;
	case B_KNIGHT_I: evaluation += STATIC_PIECE_VALUE[B_KNIGHT_I]; break;
	case B_BISHOP_I: evaluation += STATIC_PIECE_VALUE[B_BISHOP_I]; break;
	case B_ROOK_I:   evaluation += STATIC_PIECE_VALUE[B_ROOK_I]  ; break;
	case B_QUEEN_I:  evaluation += STATIC_PIECE_VALUE[B_QUEEN_I] ; break;
	}


	evaluation += (type < 6 ? GetPSEValue(index, type, GetGamePhase(board), GetGamePhaseValue(board)) : -GetPSEValue(index, type, GetGamePhase(board), GetGamePhaseValue(board)));

	return evaluation;
}

// returns a value from 0 to 1 to indicate the game phase who the game is, for example, if it's 0, 
// the game phase is opening, mid game or endgame. When the position is closer to 1 indicates that 
// the position is closer to the next game phase. For example: 
// OPENING: 0.00	start of the game
// MIDGAME: 0.86	midgame close to the endgame.
double Evaluation::GetGamePhaseValue(const Board& position) {
	int whiteMaterial = 0;
	int blackMaterial = 0;

	for (int i = 0; i < 12; i++) {
		if (i == 5 || i == 11)
			continue;

		Bitboard b = position.bitboards[i];
		while (b) {
			int sq = Utils::PopLSB(b);
			if (i < 6) {
				whiteMaterial += GAME_PHASE_VALUE[i];
			}
			else {
				blackMaterial -= GAME_PHASE_VALUE[i-6];
			}
		}
	}

	const int score = whiteMaterial - blackMaterial;

	if (score <= ENDGAME_VALUE) {
		return 1;
	}
	else if (score <= MIDGAME_VALUE) {
		const double scoreToEndgame = score - ENDGAME_VALUE;
		const double distanceBetween = MIDGAME_VALUE - ENDGAME_VALUE;
		return double(1.0-scoreToEndgame/distanceBetween);
	}
	else {
		const double scoreToMidgame = score - MIDGAME_VALUE;
		const double distanceBetween = OPENING_VALUE - MIDGAME_VALUE;
		return double(1.0 - scoreToMidgame / distanceBetween);
	}
}

int Evaluation::GetPSEValue(const uint8_t position, const uint8_t piece, const GAME_PHASE phase, const double gameValue) {

	switch (phase)
	{
	case OPENING:
		return int(Utils::InterpolateInt(PSE_OPENING[piece][position], PSE_MIDGAME[piece][position], gameValue, false));
		break;
	case MIDGAME:
		return int(Utils::InterpolateInt(PSE_MIDGAME[piece][position], PSE_ENDGAME[piece][position], gameValue, false));
		break;

	case ENDGAME:
		return PSE_ENDGAME[piece][position];
		break;
	default:
		break;
	}

	std::cout << "ERROR: unknown game phase.\n";
	return 0;
}