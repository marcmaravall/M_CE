#include "board.h"
#include "utils.h"
#include "engine.h"

Board::Board(const char* fen)
{
	//std::cout << Utils::IsBlackPieceAt(*this, 7) << std::endl;
	ClearCurrentBitboard();

    std::string fenString(fen);
    size_t index = 0;

    for (int rank = 7; rank >= 0; rank--) {
        int file = 0;

        while (file < 8 && index < fenString.size()) {
            char c = fenString[index++];

            if (c == '/') {
				// break;		error solved
				// std::cout << "jumping rank\n";
            }
            else if (isdigit(c)) {
                file += (c - '0');
				// std::cout << "Jumping " << c << " spaces\n";;
            }
            else {
                bool pieceFound = false;
                for (size_t pieceIndex = 0; pieceIndex < 12; pieceIndex++) {
                    if (c == PIECE_CHAR[pieceIndex]) {
                        bitboards[pieceIndex] |= (1ULL << (rank * 8 + file));
                        pieceFound = true;
                        break;
                    }
                }

                if (pieceFound) {
                    file++;
					// std::cout << "piece found\n";
                }
                else {
                    std::cerr << "character not found " << c << std::endl;
                    file++;
                }
            }
        }
    }

	// some other things

    while (index < fenString.size() && fenString[index] != ' ') {
        index++;
    }

	index++;

	if (index < fenString.size()) {
		turn = (fenString[index] == 'w');
        index++;
    }

    while (index < fenString.size() && fenString[index] == ' ') {
        index++;
    }

	while (	fenString[index] == 'K' ||
			fenString[index] == 'Q' ||
			fenString[index] == 'k' ||
			fenString[index] == 'q')
	{
		switch (fenString[index])
		{
		case 'K':
			wCastlingKing = true;
			break;
		case 'Q':
			wCastlingQueen = true;
			break;
		case 'k':
			bCastlingKing = true;
			break;
		case 'q':
			bCastlingQueen = true;
			break;
		default:
			break;
		}
		index++;
	}

	while (index < fenString.size() && fenString[index] == ' ') {
		index++;
	}

	enPassantSquare = -1;
	if (index < fenString.size()) {
		if (fenString[index] == '-') {
			index++;
			// std::cout << "no en passant\n";
		}
		else if (index + 1 < fenString.size()) {
			std::cout << "en passant\n";
			char fileChar = fenString[index];
			char rankChar = fenString[index + 1];
			if (fileChar >= 'a' && fileChar <= 'h' && rankChar >= '1' && rankChar <= '8') {
				int file = fileChar - 'a';
				int rank = 8 - (rankChar - '0');
				enPassantSquare = rank * 8 + file;
				// std::cout << "En passant square set: " << fileChar << rankChar << " (index: " << enPassantSquare << ")\n";
			}
			index += 2;
		}
	}
}

void Board::ClearCurrentBitboard()
{
	for (int i = 0; i < 12; i++) {
		bitboards[i] = 0ULL;
	}
}


Board::Board()
{
}

Board::~Board()
{
}

/*std::string Board::ConvertToBoardPosition(uint8_t squareIndex) {
	return squareIndex < 64 ? (std::string)BOARD_STRINGS[squareIndex] : "null";
}*/

// TODO: refatorize for struct Move				//doing it... I hate C++		did it
bool Board::MovePiece(const Move move)
{
	const Board buffer = *this;
	/*UndoInfo undo;
	undo.wCastlingKing = wCastlingKing;
	undo.wCastlingQueen = wCastlingQueen;
	undo.bCastlingKing = bCastlingKing;
	undo.bCastlingQueen = bCastlingQueen;
	undo.enPassantSquare = enPassantSquare;
	undo.turn = turn;
	undo.turns = turns;
	undo.move = move;
	undo.capturedPiece = Utils::GetPieceType(*this, move.to);
	undo.promotedPiece = (move.promotion != 255 ? move.promotion : 255);*/

	uint8_t from = move.from;
	uint8_t to = move.to;

	Board start = *this;

	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
			break;
		}
	}

	if (pieceType >= 12) {
		// std::cerr << "Piece not found, piece: " << pieceType << "\n";
		// Utils::DebugMove(move);
		// Utils::PrintBitboards(*this);
		return false;
	}

	Bitboard bitboard = bitboards[pieceType];

	if (turn == WHITE_TURN && pieceType >= 6) {
		// std::cerr << "ERROR: trying to move a black piece.\n";
		return false;
	}
	else if (turn == !WHITE_TURN && pieceType < 6) {
		// std::cerr << "ERROR: trying to move a white piece.\n";
		return false;
	}

	if (move.promotion < 12) {
		Promotion(move);

		// std::cerr << "PROMOTION DO\n";
	}
	else
	{
		// std::cerr << "DEBUG: " << Utils::MoveToStr(move) << " " << (int)(move.promotion) << "\n";
	}

	if (move.castling) {
		if (CanCastle(move))
		{
			Castle(move);
		}
		else
		{
			// std::cerr << "HI\n";
		}
	}

	switch (pieceType)
	{
	case 0:
		if (CanMovePawn(move))
		{
			MovePawn(move);
		}
		break;
	case 1:
		if (CanMoveKnight(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 2:
		if (CanMoveBishop(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 3:
		if (CanMoveRook(move))
		{
			if (from == 7)
				wCastlingQueen = false;
			if (from == 0)
				wCastlingQueen = false;

			MoveWithoutComprobe(from, to);

			enPassantSquare = 255;
		}
		break;
	case 4:
		if (CanMoveQueen(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 5:
		if (CanMoveKing(move))
		{
			MoveWithoutComprobe(from, to);
			wCastlingKing = false;
			wCastlingQueen = false;
			enPassantSquare = 255;

		}
		break;
	case 6:

		if (CanMovePawn(move))
		{
			MovePawn(move);
		}
		break;
	case 7:
		if (CanMoveKnight(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 8:
		if (CanMoveBishop(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;

		}
		break;
	case 9:
		if (CanMoveRook(move))
		{
			if (from == 63-7)
				bCastlingQueen = false;
			if (from == 63)
				bCastlingQueen = false;

			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 10:
		if (CanMoveQueen(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 11:
		if (CanMoveKing(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;

			bCastlingKing = false;
			bCastlingQueen = false;
		}

		break;
	}

	if (IsCheck((start.turn == WHITE_TURN) ? WHITE : BLACK, false)) {
		// std::cerr << "ERROR: move puts king in check.\n";
		// std::cerr << ((turn == WHITE_TURN) ? "WHITE":"BLACK") << "\n";
		// UndoMove(undo);
		*this = buffer;
		return false;
	}
	else
	{

	}

	return true;
}

bool Board::Promotion(const Move move)
{
	int from = move.from;
	int position = move.to;
	int promotion = move.promotion;
	// std::cerr << "PROMOTION ----" << from << " " << position << " " << promotion << "\n";

	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
		}
	}

	// std::cout << promotion;

	if (promotion == 0 || promotion == 5 || promotion == 6 || promotion == 11) {
		std::cerr << "ERROR: cannot promove to pawn or king.\n";
		return false;
	}

	if (turn == WHITE_TURN && pieceType != W_PAWN_I) {
		std::cerr << "ERROR: only pawns can promote." << pieceType << "\n";
		return false;
	}
	if (turn == !WHITE_TURN && pieceType != B_PAWN_I) {
		std::cerr << "ERROR: only pawns can promote.\n";
		return false;
	}


	if (turn == WHITE_TURN && position < 56) {
		std::cerr << "ERROR: cannot promote a piece that is not in the last rank.\n";
		return false;
	}
	else if (turn == !WHITE_TURN && position > 7) {
		std::cerr << "ERROR: cannot promote a piece that is not in the last rank.\n";
		return false;
	}

	if (!CanMovePawn(move)) {
		std::cerr << "ERROR: promotion error.\n";
		return false;
	}

	MoveWithoutComprobe(from, position);
	ClearBitInAllBitboards(position);

	SetBitboardBit(promotion, position);

	// std::cerr << "Board representation: \n";
	return true;
}

bool Board::CanMovePawn(const Move move, const bool _turn) const {
	const Bitboard allPieces = Utils::GetAllBitboards(bitboards, BOTH);
	const uint64_t occupied = allPieces;
	const uint64_t empty = ~occupied;

	const uint8_t from = move.from;
	const uint8_t to = move.to;

	const uint64_t fromBB = 1ULL << from;
	const uint64_t toBB = 1ULL << to;

	if (_turn == WHITE_TURN) {
		const uint64_t oneStep = fromBB << 8;
		const uint64_t twoSteps = fromBB << 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) << 7) |
			((fromBB & ~FILE_A_MASK) << 9);

		if ((to == (from + NORTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) != 1) {
			/*std::cout << "DEBUG: \n";
			Utils::DebugBitboard(allPieces);

			std::cout << "\n \n";
			*/
			return true;
		}
		else if ((toBB & twoSteps) && (fromBB & RANK_2_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			return true;
		}
		else if ((fromBB & ~FILE_H_MASK) && (to == from + NORTH_EAST) && Utils::IsBlackPieceAt(*this, to)) {
			return true;
		}
		else if ((fromBB & ~FILE_A_MASK) && (to == from + NORTH_WEST) && Utils::IsBlackPieceAt(*this, to)) {
			return true;
		}

		else if ((to == enPassantSquare) &&
			((from + NORTH_EAST == to) || (from + NORTH_WEST == to)) && to > 32 && (fromBB & ~FILE_H_MASK) && (fromBB & ~FILE_A_MASK)) {
			return true;
		}
	}
	else { // BLACK_TURN
		const uint64_t oneStep = fromBB >> 8;
		const uint64_t twoSteps = fromBB >> 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) >> 7) |
			((fromBB & ~FILE_A_MASK) >> 9);

		if ((to == (from + SOUTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) != 1) {
			return true;
		}
		else if ((toBB & twoSteps) && (fromBB & RANK_7_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			return true;
		}
		else if ((fromBB & ~FILE_H_MASK) && (to == from + SOUTH_EAST) && Utils::IsWhitePieceAt(*this, to)) {
			return true;
		}
		else if ((fromBB & ~FILE_A_MASK) && (to == from + SOUTH_WEST) && Utils::IsWhitePieceAt(*this, to)) {
			return true;
		}
		else if ((to == enPassantSquare) &&
			((from + SOUTH_EAST == to) || (from + SOUTH_WEST == to)) && to < 32 && (fromBB & ~FILE_H_MASK) && (fromBB & ~FILE_A_MASK)) {
			return true;
		}
	}

	return false;
}

bool Board::CanMovePawn(const Move move) const {
	return CanMovePawn(move, this->turn);
}

bool Board::CanMoveKnight(const Move move) const {
	return CanMoveKnight(move, this->turn);
}

bool Board::CanMoveBishop(const Move move) const {
	return CanMoveBishop(move, this->turn);
}

bool Board::CanMoveQueen(const Move move) const {
	return CanMoveQueen(move, this->turn);
}

bool Board::CanMoveRook(const Move move) const {
	return CanMoveRook(move, this->turn);
}

bool Board::CanMoveKing(const Move move) const {
	return CanMoveKing(move, this->turn);
}


void Board::MovePawn(const Move move) {
	if (!CanMovePawn(move))
		return;

	const Bitboard allPieces = Utils::GetAllBitboards(bitboards, BOTH);
	const uint64_t occupied = allPieces;
	const uint64_t empty = ~occupied;

	const uint8_t from = move.from;
	const uint8_t to = move.to;

	const uint64_t fromBB = 1ULL << from;
	const uint64_t toBB = 1ULL << to;

	if (turn == WHITE_TURN) {
		const uint64_t oneStep = fromBB << 8;
		const uint64_t twoSteps = fromBB << 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) << 7) |
			((fromBB & ~FILE_A_MASK) << 9);

		if ((to == (from + NORTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) != 1) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((toBB & twoSteps) && (fromBB & RANK_2_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = from+NORTH;
		}

		else if (((to == from + NORTH_EAST || to == from + NORTH_WEST)) &&
			Utils::IsBlackPieceAt(*this, to)) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((to == enPassantSquare) &&
			((from + NORTH_EAST == to) || (from + NORTH_WEST == to))) {

			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
	}

	else { // BLACK_TURN
		const uint64_t oneStep = fromBB >> 8;
		const uint64_t twoSteps = fromBB >> 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) >> 7) |
			((fromBB & ~FILE_A_MASK) >> 9);

		if ((to == (from + SOUTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) != 1) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((toBB & twoSteps) && (fromBB & RANK_7_MASK) &&
			((oneStep & empty) && (toBB & empty))) {

			MoveWithoutComprobe(from, to);
			enPassantSquare = from + SOUTH;
		}

		else if (((to == from + SOUTH_EAST || to == from + SOUTH_WEST)) &&
			Utils::IsWhitePieceAt(*this, to)) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((to == enPassantSquare) &&
			((from + SOUTH_EAST == to) || (from + SOUTH_WEST == to))) {

			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
	}
}

void Board::UndoMove(const UndoInfo& undo) {
	//std::cout << "DEBUG UNDO MOVE" << "\n";
	//Utils::PrintBoard(*this);

	bool debug = false;

	// std::cout << "FEN START: " << Utils::ConvertToFEN(*this) << "\n";

	if (Utils::ConvertToFEN(*this) == "rnbPk1nr/ppppbppp/8/4p3/4P3/P7/1PPP1PPP/RNBQKBNR w KQkq - 0 1") {
		debug = true;
		std::cout << "DEBUGGING POSITION WITH ERROR -------------------------\n";
	}

	Move reverseMove(undo.move.to, undo.move.from);

	MoveWithoutComprobe(reverseMove.from, reverseMove.to);

	if (undo.capturedPiece != 255) {
		SetBitboardBit(undo.capturedPiece, undo.move.to);
	}

	//if (undo.promotedPiece != 255) {
	//	ClearBitInAllBitboards(undo.move.to);
	//	SetBitboardBit((undo.turn == WHITE_TURN ? 0 : 6), undo.move.from);		TOD0: solve bug in this part.
	//}

	wCastlingKing = undo.wCastlingKing;
	wCastlingQueen = undo.wCastlingQueen;
	bCastlingKing = undo.bCastlingKing;
	bCastlingQueen = undo.bCastlingQueen;
	enPassantSquare = undo.enPassantSquare;
	turn = undo.turn;
	turns = undo.turns;

	// std::cout << "FEN REVERSED MOVE:" << Utils::ConvertToFEN(*this) << "\n";
}

bool Board::CanMoveKnight(const Move move, const bool _turn) const{
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (!((Engine::knightMasks[from] >> to) & 1ULL))
		return false;

	if (_turn == WHITE_TURN)
		return !Utils::IsWhitePieceAt(*this, to);
	else
		return !Utils::IsBlackPieceAt(*this, to);
}

bool Board::CanMoveBishop(const Move move, const bool _turn) const{
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (from == to)
		return false;

	const Bitboard fromBB = 1ULL << from;
	const Bitboard toBB = 1ULL << to;

	if (_turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	const int fromFile = from % 8;
	const int fromRank = from / 8;
	const int toFile = to % 8;
	const int toRank = to / 8;

	if (abs(toFile - fromFile) != abs(toRank - fromRank))
		return false;

	const int fileStep = (toFile - fromFile) > 0 ? 1 : -1;
	const int rankStep = (toRank - fromRank) > 0 ? 1 : -1;

	int file = fromFile + fileStep;
	int rank = fromRank + rankStep;

	while (file != toFile && rank != toRank) {
		int square = rank * 8 + file;
		if (IsOccupied(square))
			return false;

		file += fileStep;
		rank += rankStep;
	}

	return true;
}

bool Board::IsOccupied(uint8_t square) const{
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], square))
			return true;
	}
	return false;
}

bool Board::CanMoveRook(const Move move, const bool _turn) const{
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (from == to)
		return false;

	const int fromFile = from % 8;
	const int fromRank = from / 8;
	const int toFile = to % 8;
	const int toRank = to / 8;

	if (_turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	if (fromFile != toFile && fromRank != toRank)
		return false;

	int fileStep = 0;
	int rankStep = 0;

	if (fromFile != toFile)
		fileStep = (toFile - fromFile) > 0 ? 1 : -1;
	else
		rankStep = (toRank - fromRank) > 0 ? 1 : -1;

	int file = fromFile + fileStep;
	int rank = fromRank + rankStep;

	while (file != toFile || rank != toRank) {
		int square = rank * 8 + file;
		if (IsOccupied(square)) {
			// if (debug) std::cerr << "RETURNED FALSE FROM CAN MOVE ROOK\n";
			return false;
		}

		file += fileStep;
		rank += rankStep;
	}

	return true;
}


bool Board::CanMoveQueen(const Move move, const bool _turn) const{
	return CanMoveRook(move, _turn) || CanMoveBishop(move, _turn);
}

bool Board::CanMoveKing(const Move move, const bool _turn) const {
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (from == to || from >= 64 || to >= 64)
		return false;

	if (!(Engine::kingMasks[from] & (1ULL << to)))
		return false;

	if (_turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	return true;
}

void Board::Castle(const Move move)
{
	if (CanCastle(move))
		DoCastleMove(move);
}

bool Board::CanCastle(const Move move) {
	if (!move.castling)
		return false;

	const bool shortCastle = move.mode;

	const bool color = turn;

	if (color == WHITE_TURN) {
		if ((shortCastle && !wCastlingKing) || (!shortCastle && !wCastlingQueen))
			return false;

		if (shortCastle) {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 5) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 6)) {
				return true;
			}
		}
		else {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 1) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 2) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 3)) {
				return true;
			}
		}
	}
	else {
		if ((shortCastle && !bCastlingKing) || (!shortCastle && !bCastlingQueen))
			return false;

		if (shortCastle) {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 61) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 62)) {
				return true;
			}
		}
		else {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 57) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 58) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 59)) {
				return true;
			}
		}
	}

	return false;
}


void Board::DoCastleMove(const Move move)
{
	const bool color = turn == WHITE_TURN ? WHITE_TURN : BLACK_TURN;
	const bool shortCastle = move.mode;

	if (color == WHITE_TURN) {

		if (shortCastle) {
			MoveWithoutComprobe(4, 6);
			MoveWithoutComprobe(7, 5);
		}
		else {
			MoveWithoutComprobe(4, 2);
			MoveWithoutComprobe(0, 3);
		}

		wCastlingKing = false;
		wCastlingQueen = false;
	}
	else
	{
		if (shortCastle) {
			MoveWithoutComprobe(60, 62);
			MoveWithoutComprobe(63, 61);
		}
		else {
			MoveWithoutComprobe(60, 58);
			MoveWithoutComprobe(56, 59);
		}
		
		bCastlingKing = false;
		bCastlingQueen = false;
	}

	turn = !turn;
}


Bitboard Board::GetKingAttacks(uint8_t square) {
	Bitboard attacks = 0;
	const int file = square % 8;
	const int rank = square / 8;

	for (int dr = -1; dr <= 1; dr++) {
		for (int df = -1; df <= 1; df++) {
			if (dr == 0 && df == 0) continue;

			int r = rank + dr;
			int f = file + df;

			if (r >= 0 && r < 8 && f >= 0 && f < 8) {
				attacks |= (1ULL << (r * 8 + f));
			}
		}
	}

	return attacks;
}

bool Board::IsSquareAttacked(const PIECE_COLORS attackerColor, const int square, const bool debug) {
	const bool attackingColor = attackerColor;

	for (size_t i = 0; i < 12; i++) {
		if ((attackingColor == WHITE_TURN && i < 6) ||
			(attackingColor == BLACK_TURN && i >= 6)) {
			continue;
		}

		for (int j = 0; j < 64; j++) {
			if (!Utils::GetBitboardValueOnIndex(bitboards[i], j)) {
				continue;
			}

			switch (i%6) {
			case 0: if (CanMovePawn		(Move{ j, square }, !attackerColor)) {return true; } break;
			case 1: if (CanMoveKnight	(Move{ j, square }, !attackerColor)) {return true; } break;
			case 2: if (CanMoveBishop	(Move{ j, square }, !attackerColor)) {return true; } break;
			case 3: if (CanMoveRook		(Move{ j, square }, !attackerColor)) {return true; } break;
			case 4: if (CanMoveQueen	(Move{ j, square }, !attackerColor)) {return true; } break;
			case 5: if (CanMoveKing		(Move{ j, square }, !attackerColor)) {return true; } break;
			}
		}
	}

	if (debug)
	{
		//std::cerr << "DEBUG: \n"
		//	<< "Square checked: " << square << "\n";
	}

	return false;
}

bool Board::IsCheck(const PIECE_COLORS king, const bool debug) {
	const int kingSquare = (king == WHITE)
		? GetWhiteKingPosition()
		: GetBlackKingPosition();

	if (debug) std::cerr << ((king == WHITE) ? "WHITE " : "BLACK ") << kingSquare << "\n";

	return IsSquareAttacked( king==WHITE? BLACK:WHITE, kingSquare, debug);
}

// wtf is this
/*
void Board::SetAllBitboards(uint8_t indexPosition) {
	for (size_t i = 0; i < 12; i++) {
		bitboards[i] |= (1ULL << indexPosition);
	}
}*/

void Board::SetBitboardBit(int piece, int indexPosition) {
	if (piece > 12)
		return;

	this->bitboards[piece] |= (1ULL << indexPosition);
}

void Board::ClearBitInAllBitboards(uint8_t  indexPosition) {
	for (size_t i = 0; i < 12; i++) {
		bitboards[i] &= ~(1ULL << indexPosition);
	}
}

void Board::MoveWithoutComprobe(int from, int position) {
	int pieceType = 255;

	// std::cout << "FROM: " << from << " POSITION: " << position << "\n";
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
		}
	}

	ClearBitInAllBitboards(from);
	ClearBitInAllBitboards(position);
	SetBitboardBit(pieceType, position);

	turn = !turn;
}

uint8_t Board::GetWhiteKingPosition(const bool debug)
{
	for (size_t i = 0; i < 64; i++)
	{
		if (Utils::GetBitboardValueOnIndex(bitboards[5], i)) {
			// if (debug) std::cerr << "White king found at: " << i << std::endl;
			return i;
		}
	}
	return 255;
}

uint8_t Board::GetBlackKingPosition(const bool debug)
{
	for (size_t i = 0; i < 64; i++)
	{
		if (Utils::GetBitboardValueOnIndex(bitboards[11], i)) {
			// std::cerr << "Black king found at " << i << "\n";
			return i;
		}
	}

	if (debug) { 
		std::cerr << "Black king not found\n";

		// debug bitboard:
		std::cerr << bitboards[11] << std::endl;
		Utils::PrintBoard(*this);
	}
	return 255;
}
