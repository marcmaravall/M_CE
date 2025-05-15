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

// TODO: refatorize for struct Move				//doing it... I hate C++
bool Board::MovePiece(const Move move)
{
	uint8_t from = move.from;
	uint8_t to = move.to;

	Board start = *this;

	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
		}
	}

	if (pieceType >= 12) {
		std::cerr << "Piece not found\n";
		return false;
	}

	Bitboard bitboard = bitboards[pieceType];

	if (turn == WHITE_TURN && pieceType >= 6) {
		//std::cerr << "ERROR: trying to move a black piece.\n";
		return false;
	}
	else if (turn == !WHITE_TURN && pieceType < 6) {
		//std::cerr << "ERROR: trying to move a white piece.\n";
		return false;
	}

	if (move.promotion < 12) {
		Promotion(move);
	}

	switch (pieceType)
	{
	case 0:
		if (CanMovePawn(move))
		{
			MoveWithoutComprobe(from, to);
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
			MoveWithoutComprobe(from, to);
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

	if (turn == WHITE_TURN ? IsCheck(GetBlackKingPosition()) : IsCheck(GetWhiteKingPosition())) {
		// std::cerr << "ERROR: move puts king in check.\n";
		*this = start;
		return false;
	}

	return true;
}

bool Board::Promotion(const Move move)
{
	int from = move.from;
	int position = move.to;
	int promotion = move.promotion;
	std::cout << from << " " << position << " " << promotion << "\n";

	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
		}
	}

	std::cout << promotion;

	if (promotion == 0 || promotion == 5 || promotion == 6 || promotion == 11) {
		std::cerr << "ERROR: cannot promove to pawn or king.\n";
		return false;
	}

	if (turn == WHITE_TURN && pieceType != W_PAWN_I) {
		std::cerr << "ERROR: only pawns can promote.\n";
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
	return true;
}

bool Board::CanMovePawn(Move move) {
	auto allPieces = Utils::GetAllBitboards(this->bitboards);
	uint64_t occupied = allPieces;
	uint64_t empty = ~occupied;

	uint8_t from = move.from;
	uint8_t to = move.to;

	uint64_t fromBB = 1ULL << from;
	uint64_t toBB = 1ULL << to;

	if (turn == WHITE_TURN) {
		uint64_t oneStep = fromBB << 8;
		uint64_t twoSteps = fromBB << 16;
		uint64_t captures = (fromBB << 7 & ~FILE_H_MASK) | (fromBB << 9 & ~FILE_A_MASK);

		if ((toBB & oneStep) && (toBB & empty)) {
			enPassantSquare = 255;
			return true;
		}

		if ((toBB & twoSteps) && (fromBB & RANK_2_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			enPassantSquare = to - 8;
			return true;
		}

		if ((toBB & captures) && Utils::IsBlackPieceAt(*this, to)) {
			enPassantSquare = 255;
			return true;
		}

		if ((to == enPassantSquare) &&
			((from + NORTH_EAST == to) || (from + NORTH_WEST == to))) {
			ClearBitInAllBitboards(enPassantSquare + SOUTH);
			enPassantSquare = 255;
			return true;
		}
	}

	else { // BLACK_TURN
		uint64_t oneStep = fromBB >> 8;
		uint64_t twoSteps = fromBB >> 16;
		uint64_t captures = (fromBB >> 7 & ~FILE_A_MASK) | (fromBB >> 9 & ~FILE_H_MASK);

		if ((toBB & oneStep) && (toBB & empty)) {
			enPassantSquare = 255;
			return true;
		}

		if ((toBB & twoSteps) && (fromBB & RANK_7_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			enPassantSquare = to + 8;
			return true;
		}

		if ((toBB & captures) && Utils::IsWhitePieceAt(*this, to)) {
			enPassantSquare = 255;
			return true;
		}

		if ((to == enPassantSquare) &&
			((from + SOUTH_EAST == to) || (from + SOUTH_WEST == to))) {
			ClearBitInAllBitboards(enPassantSquare - 8);
			enPassantSquare = 255;
			return true;
		}
	}

	return false;
}


bool Board::CanMoveKnight(const Move move) {
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (!((Engine::knightMasks[from] >> to) & 1ULL))
		return false;

	if (turn == WHITE_TURN)
		return !Utils::IsWhitePieceAt(*this, to);
	else
		return !Utils::IsBlackPieceAt(*this, to);
}

bool Board::CanMoveBishop(const Move move) {
	uint8_t from = move.from;
	uint8_t to = move.to;

	if (from == to)
		return false;

	Bitboard fromBB = 1ULL << from;
	Bitboard toBB = 1ULL << to;

	if (turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	int fromFile = from % 8;
	int fromRank = from / 8;
	int toFile = to % 8;
	int toRank = to / 8;

	if (abs(toFile - fromFile) != abs(toRank - fromRank))
		return false;

	int fileStep = (toFile - fromFile) > 0 ? 1 : -1;
	int rankStep = (toRank - fromRank) > 0 ? 1 : -1;

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

bool Board::IsOccupied(uint8_t square) {
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], square))
			return true;
	}
	return false;
}

bool Board::CanMoveRook(const Move move) {
	uint8_t from = move.from;
	uint8_t to = move.to;

	if (from == to)
		return false;

	int fromFile = from % 8;
	int fromRank = from / 8;
	int toFile = to % 8;
	int toRank = to / 8;

	if (turn == WHITE_TURN) {
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
		if (IsOccupied(square))
			return false;

		file += fileStep;
		rank += rankStep;
	}

	return true;
}


bool Board::CanMoveQueen(const Move move) {
	return CanMoveRook(move) || CanMoveBishop(move);
}

bool Board::CanMoveKing(const Move move) {
	uint8_t from = move.from;
	uint8_t to = move.to;

	if (from == to)
		return false;

	int fromFile = from % 8;
	int fromRank = from / 8;
	int toFile = to % 8;
	int toRank = to / 8;

	if (turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else if (turn == BLACK_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}

	if (abs(toFile - fromFile) > 1 || abs(toRank - fromRank) > 1)
		return false;

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

	bool shortCastle = move.mode;

	bool color = turn;

	if (color == WHITE_TURN) {
		if ((shortCastle && !wCastlingKing) || (!shortCastle && !wCastlingQueen))
			return false;

		if (shortCastle) {
			// Enroque corto blanco: casillas f1 (5) y g1 (6) deben estar vacías
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
			// Enroque corto negro: f8 (61), g8 (62)
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 61) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 62)) {
				return true;
			}
		}
		else {
			// Enroque largo negro: c8 (58), d8 (59), e8 (60)
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
	bool color = turn == WHITE_TURN ? WHITE_TURN : BLACK_TURN;
	bool shortCastle = move.mode;

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
	int file = square % 8;
	int rank = square / 8;

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

bool Board::IsSquareAttacked(uint8_t square) {
	bool attackingColor = turn == WHITE_TURN ? BLACK_TURN : WHITE_TURN;

	for (size_t i = 0; i < 12; i++) {
		if ((attackingColor == WHITE_TURN && i < 6) ||
			(attackingColor == BLACK_TURN && i >= 6)) {
			continue;
		}

		for (uint8_t j = 0; j < 64; j++) {
			if (!Utils::GetBitboardValueOnIndex(bitboards[i], j)) {
				continue;
			}
			
			switch (i % 6) {
			case 0: if (CanMovePawn(Move {j, square} )) return true; break;
			case 1: if (CanMoveKnight(Move{ j, square })) return true; break;
			case 2: if (CanMoveBishop(Move{ j, square })) return true; break;
			case 3: if (CanMoveRook(Move{ j, square })) return true; break;
			case 4: if (CanMoveQueen(Move{ j, square })) return true; break;
			case 5: if (CanMoveKing(Move{ j, square })) return true; break;
			}
		}
	}

	return false;
}

bool Board::IsCheck(uint8_t indexPosition) {

	return IsSquareAttacked(indexPosition);
}

// wtf is this, it's not usefull
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

uint8_t Board::GetWhiteKingPosition()
{
	for (size_t i = 0; i < 64; i++)
	{
		if (Utils::GetBitboardValueOnIndex(bitboards[5], i)) {
			// std::cout << "White king found at: " << i << std::endl;
			return i;
		}
	}
	return 255;
}

uint8_t Board::GetBlackKingPosition()
{
	for (size_t i = 0; i < 64; i++)
	{
		if (Utils::GetBitboardValueOnIndex(bitboards[11], i))
			return i;
	}
	return 255;
}
