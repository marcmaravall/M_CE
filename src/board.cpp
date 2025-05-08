#include "board.h"


Board::Board(const char* fen)
{
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

void Board::PrintBitboards()
{
	for (size_t i = 0; i < 12; i++) {
		std::cout << "\n" << bitboards[i] << "\n";
		for (int j = 0; j < 64; j++) {
			if (j % 8 == 0)
				std::cout << "\n";
			std::cout << GetBitboardValueOnIndex(bitboards[i], j);
		}
	}
}

void Board::PrintBoard()
{
	for (int rank = 7; rank >= 0; rank--)
	{
		std::cout << rank + 1 << " ";
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			bool found = false;

			for (size_t pieceIndex = 0; pieceIndex < 12; pieceIndex++)
			{
				if (bitboards[pieceIndex] & (1ULL << square))
				{
					std::cout << PIECE_CHAR[pieceIndex] << " ";
					found = true;
					break;
				}
			}

			if (!found) {
				std::cout << ". ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "  a b c d e f g h\n" << std::endl
		<< ((turn == WHITE_TURN) ? "White" : "Black") << " to move.\n"
		<< "En passant: " << (enPassantSquare <= 64 ? ConvertToBoardPosition(enPassantSquare) : "no") << "\n"
		<< "Castling: "
		<< (wCastlingKing ? "K" : "-") << (wCastlingQueen ? "Q" : "-") << (bCastlingKing ? "k" : "-") << (bCastlingQueen ? "q" : "-") << "\n";
}

std::string Board::ConvertToBoardPosition(uint8_t squareIndex) {
	return squareIndex < 64 ? (std::string)BOARD_STRINGS[squareIndex] : "null";
}

int Board::ConvertToIndexPosition(std::string squarePosition) {
	int ret = 255;
	squarePosition = ToUpper(squarePosition);
	// std::cout << squarePosition << "\n";
	for (int i = 0; i < 64; i++)
	{
		if (squarePosition == BOARD_STRINGS[i]) {
			
			ret = i;
			// std::cout << "i equals " << ret << "\n";
			return ret;
		}
	}

	return ret;
}

std::string Board::ToUpper(std::string str)
{
	std::string res = "";

	for (size_t i = 0; i < str.size(); i++) {
		res += toupper(str[i]);
	}

	return res;
}

std::string Board::ConvertToFEN()
{
	std::string fen = "";

	for (int rank = 7; rank >= 0; rank--)
	{
		int emptyCount = 0;
		for (int file = 0; file < 8; file++)
		{
			bool pieceFound = false;
			int square = rank * 8 + file;

			for (int piece = 0; piece < 12; piece++)
			{
				if (bitboards[piece] & (1ULL << square))
				{
					if (emptyCount > 0)
					{
						fen += std::to_string(emptyCount);
						emptyCount = 0;
					}
					fen += PIECE_CHAR[piece];
					pieceFound = true;
					break;
				}
			}

			if (!pieceFound)
			{
				emptyCount++;
			}
		}
		if (emptyCount > 0)
			fen += std::to_string(emptyCount);
		if (rank != 0)
			fen += '/';
	}

	fen += ' ';
	fen += (turn ? 'w' : 'b');

	std::string castling = "";
	if (wCastlingKing) castling += 'K';
	if (wCastlingQueen) castling += 'Q';
	if (bCastlingKing) castling += 'k';
	if (bCastlingQueen) castling += 'q';
	if (castling.empty()) castling = "-";
	fen += ' ' + castling;

	fen += ' ';
	if (enPassantSquare >= -1 && enPassantSquare < 64)
	{
		fen += ConvertToBoardPosition(enPassantSquare);
	}
	else
	{
		fen += '-';
	}

	fen += " 0 1";

	return fen;
}

// TODO: refatorize for struct Move
bool Board::MovePiece(uint8_t piecePos, uint8_t position, uint8_t promotion)
{
	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (GetBitboardValueOnIndex(bitboards[i], piecePos)) {
			pieceType = i;
		}
	}

	if (pieceType >= 12) {
		std::cerr << "Piece not found\n";
		return false;
	}

	Bitboard bitboard = bitboards[pieceType];

	/*if (pieceType < 6 && turn == WHITE_TURN || pieceType > 5 && turn == WHITE_TURN) {
		std::cout << "ERROR: triying to move a piece in a turn that is not.\n" << "Turn: " << turn << "\n" << "PieceType: " << pieceType << "\n";
		return false;
	}*/

	if (promotion < 12) {
		Promotion(piecePos, position, promotion);
	}

	switch (pieceType)
	{
	case 0:
		if (CanMovePawn(piecePos, position))
		{
			MoveWithoutComprobe(piecePos, position);
		}
		break;
	case 6:

		if (CanMovePawn(piecePos, position))
		{
			MoveWithoutComprobe(piecePos, position);
		}
		break;
	default:
		break;
	}

	turn = !turn;
}

bool Board::Promotion(uint8_t piece, uint8_t position, uint8_t promotion)
{
	if (!CanMovePawn(piece, position)) {
		std::cerr << "ERROR: promotion error.\n";
		return false;
	}

	MoveWithoutComprobe(piece, position);
	ClearBitInAllBitboards(position);

	switch (promotion)
	{
	case W_QUEEN:
	case W_ROOK:
	case W_BISHOP:
	case W_KNIGHT:
	case B_QUEEN:
	case B_ROOK:
	case B_BISHOP:
	case B_KNIGHT:
		SetBitboardBit(promotion, position);
		return true;

	default:
		std::cerr << "ERROR: invalid promotion.\n";
		return false;
	}
}


bool Board::CanMovePawn(uint8_t from, uint8_t _where) {
	auto all = GetAllBitboards(this->bitboards);

	if (turn == WHITE_TURN) {
		if (from + NORTH == _where && GetBitboardValueOnIndex(all, _where) != 1) {
			enPassantSquare = 255;
			return true;
		}
		else if (from + (2 * NORTH) == _where &&
			GetBitboardValueOnIndex(all, _where) != 1 &&
			GetBitboardValueOnIndex(all, from + NORTH) != 1 &&
			from >= 8 && from < 16)
		{
			enPassantSquare = _where + SOUTH;
			return true;
		}
		else if ((from + NORTH_EAST == _where || from + NORTH_WEST == _where) ||
				 (from + NORTH_EAST == enPassantSquare || from + NORTH_WEST == enPassantSquare) &&
				  IsBlackPieceAt(_where)) {
			ClearBitInAllBitboards(enPassantSquare + SOUTH);

			enPassantSquare = 255;
			return true;
		}
	}

	else {
		if (from + SOUTH == _where && GetBitboardValueOnIndex(all, _where) != 1) {
			enPassantSquare = 255;
			return true;
		}
		else if (from + (2 * SOUTH) == _where &&
			GetBitboardValueOnIndex(all, _where) != 1 &&
			GetBitboardValueOnIndex(all, from + SOUTH) != 1 &&
			from >= 48 && from < 56)
		{
			enPassantSquare = _where + NORTH;
			return true;
		}
		else if ((from + SOUTH_EAST == _where || from + SOUTH_WEST == _where) &&
			_where == enPassantSquare)
		{
			ClearBitInAllBitboards(enPassantSquare + NORTH);
			enPassantSquare = 255;
			return true;
		}
	}

	return false;
}

Bitboard Board::GetAllBitboards(Bitboard b[12], PIECE_COLORS colors)
{

	Bitboard res = 0;
	for (size_t i = (colors != BLACK ? 0 : 6); i < (colors == BOTH ? 12: (colors == WHITE ? 6: 12)); i++) {
		res |= b[i];
	}
	return res;
}

// wtf is this, it's not usefull
void Board::SetAllBitboards(uint8_t indexPosition) {
	for (size_t i = 0; i < 12; i++) {
		bitboards[i] |= (1ULL << indexPosition);
	}
}

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

bool Board::GetBitboardValueOnIndex(Bitboard bitboard, uint8_t index) {
	return bitboard & (1ULL << index);
}

void Board::MoveWithoutComprobe(int from, int position) {
	int pieceType = 255;

	// std::cout << "FROM: " << from << " POSITION: " << position << "\n";
	for (int i = 0; i < 12; i++) {
		if (GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
		}
	}

	ClearBitInAllBitboards(from);
	ClearBitInAllBitboards(position);
	SetBitboardBit(pieceType, position);
}

bool Board::IsWhitePieceAt(uint8_t index) {
	return	((GetBitboardValueOnIndex(bitboards[0], index) == 1) ||
			 (GetBitboardValueOnIndex(bitboards[1], index) == 1) ||
			 (GetBitboardValueOnIndex(bitboards[2], index) == 1) ||
			 (GetBitboardValueOnIndex(bitboards[3], index) == 1) ||
			 (GetBitboardValueOnIndex(bitboards[4], index) == 1) ||
			 (GetBitboardValueOnIndex(bitboards[5], index) == 1));
}

bool Board::IsBlackPieceAt(uint8_t index) {
	return	
		((GetBitboardValueOnIndex(bitboards[6], index) == 1) ||
		(GetBitboardValueOnIndex(bitboards[7], index) == 1) ||
		(GetBitboardValueOnIndex(bitboards[8], index) == 1) ||
		(GetBitboardValueOnIndex(bitboards[9], index) == 1) ||
		(GetBitboardValueOnIndex(bitboards[10], index) == 1) ||
		(GetBitboardValueOnIndex(bitboards[11], index) == 1));
}

