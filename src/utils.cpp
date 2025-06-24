#include "utils.h"
#include "board.h"

void Utils::PrintBoard(const Board& board)
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
				if (board.bitboards[pieceIndex] & (1ULL << square))
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
		<< "FEN: " << ConvertToFEN(board) << "\n"
		<< ((board.GetTurn() == WHITE_TURN) ? "White" : "Black") << " to move.\n"
		<< "En passant: " << (board.enPassantSquare <= 64 ? ConvertToBoardPosition(board.enPassantSquare) : "no") << "\n"
		<< "Castling: "
		<< (board.wCastlingKing ? "K" : "-") << (board.wCastlingQueen ? "Q" : "-") << (board.bCastlingKing ? "k" : "-") << (board.bCastlingQueen ? "q" : "-") << "\n";
}

void Utils::PrintBitboards(const Board& board)
{
	std::array<int, 64> pieceCounts = { 0 };

	for (int i = 0; i < 12; ++i) {
		for (int square = 0; square < 64; ++square) {
			if (Utils::GetBitboardValueOnIndex(board.bitboards[i], square)) {
				pieceCounts[square]++;
			}
		}
	}

	for (int square = 0; square < 64; ++square) {
		if (pieceCounts[square] > 1) {
			std::cerr << "ERROR: Square " << square << " has " << pieceCounts[square] << " pieces!\n";
		}
		if (pieceCounts[square] == 0 && Utils::GetPieceType(board, square) != 255) {
			std::cerr << "ERROR: Square " << square << " has no bitboard piece but pieceType is set to "
				<< Utils::GetPieceType(board, square) << "\n";
		}
	}
}

std::string Utils::ConvertToFEN(const Board& board)
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
				if (board.bitboards[piece] & (1ULL << square))
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
	fen += (board.turn ? 'w' : 'b');

	std::string castling = "";
	if (board.wCastlingKing) castling += 'K';
	if (board.wCastlingQueen) castling += 'Q';
	if (board.bCastlingKing) castling += 'k';
	if (board.bCastlingQueen) castling += 'q';
	if (castling.empty()) castling = "-";
	fen += ' ' + castling;
	
	fen += ' ';
	if (board.enPassantSquare >= -1 && board.enPassantSquare < 64)
	{
		fen += ConvertToBoardPosition(board.enPassantSquare);
	}
	else
	{
		fen += '-';
	}

	fen += (" " + std::to_string(board.halfMoves) + " " + std::to_string(board.turns));

	return fen;
}

std::string Utils::ConvertToBoardPosition(uint8_t squareIndex) {
	return squareIndex < 64 ? Utils::ToLower((std::string)BOARD_STRINGS[squareIndex]) : "null";
}

int Utils::ConvertToIndexPosition(std::string& squarePosition) {
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

Bitboard Utils::GetFileMask(uint8_t file)
{
	Bitboard mask = 0;
	for (int rank = 0; rank < 8; rank++)
	{
		mask |= (1ULL << (rank * 8 + file));
	}
	return mask;
}

bool Utils::IsWhitePieceAt(const Board& board, uint8_t index) {
	return
		(GetBitboardValueOnIndex(board.bitboards[0], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[1], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[2], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[3], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[4], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[5], index) == 1);
}

bool Utils::IsBlackPieceAt(const Board& board, uint8_t index) {
	return
		(GetBitboardValueOnIndex(board.bitboards[6], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[7], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[8], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[9], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[10], index) == 1) ||
		(GetBitboardValueOnIndex(board.bitboards[11], index) == 1);
}


bool Utils::GetBitboardValueOnIndex(Bitboard bitboard, uint8_t index) {
	return bitboard & (1ULL << index);
}

std::string Utils::ToUpper(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		str[i] = toupper(str[i]);
	}
	return str;
}

Bitboard Utils::GetAllBitboards(const Bitboard b[12], PIECE_COLORS colors)
{
	Bitboard res = 0;
	for (size_t i = (colors != BLACK ? 0 : 6); i < (colors == BOTH ? 12 : (colors == WHITE ? 6 : 12)); i++) {
		res |= b[i];
	}
	return res;
}

void Utils::DebugBitboard(Bitboard bitboard)
{
	for (int square = 0; square < 64; ++square) {
		if (square % 8 == 0) std::cout << "\n";
		
		if (Utils::GetBitboardValueOnIndex(bitboard, square)) {
			std::cout << "1";
		}
		else std::cout << "_";
	}
}

Bitboard Utils::GetRankMask(uint8_t rank)
{
	Bitboard mask = 0;
	for (int file = 0; file < 8; file++)
	{
		mask |= (1ULL << (rank * 8 + file));
	}
	return mask;
}

Bitboard Utils::GetDiagonalMask(uint8_t index)
{
	Bitboard mask = 0;
	int rank = index / 8;
	int file = index % 8;

	for (int i = -7; i <= 7; ++i)
	{
		if (i != 0)
		{
			int r = rank + i;
			int f = file + i;
			if (r >= 0 && r < 8 && f >= 0 && f < 8)
			{
				mask |= (1ULL << (r * 8 + f));
			}
		}
	}

	return mask;
}

uint8_t Utils::GetPieceType(const Board& board, uint8_t index) {
	for (int i = 0; i < 12; i++) {
		if (GetBitboardValueOnIndex(board.bitboards[i], index)) {
			return i;
		}
	}
	return 255;
}


int Utils::PopLSB(uint64_t& bb) {
	int index;

#if defined(_MSC_VER)
	unsigned long idx;
	_BitScanForward64(&idx, bb);
	index = (int)idx;
#else
	index = __builtin_ctzll(bb);
#endif

	bb &= bb - 1;
	return index;
}

ZobristHash Utils::GetZobristHash(const Board& board, const ZobristHashSettings& settings)
{
	ZobristHash hash = 0;

	for (int i = 0; i < 12; i++)
	{
		uint8_t piece = i;
		uint64_t bitboard = board.bitboards[piece];
		while (bitboard)
		{
			int square = PopLSB(bitboard);
			hash ^= settings.zobristPieces[piece][square];
			// std::cout << "XORed " << settings.zobristPieces[piece][square] << " SQUARE: " << Utils::ConvertToBoardPosition(square) << " PIECE: " << square << "\n";
		}
	}

	if (board.wCastlingKing) {
		hash ^= settings.zobristCastling[0];
		// std::cout << "XORed wcastking\n";
	}
	if (board.wCastlingQueen) {
		hash ^= settings.zobristCastling[1];
		// std::cout << "XORed wcastkingqueen\n";
	}
	if (board.bCastlingKing) {
		// std::cout << "XORed bcastking\n";
		hash ^= settings.zobristCastling[2];
	}
	if (board.bCastlingQueen) {
		hash ^= settings.zobristCastling[3];
		// std::cout << "XORed bcastkingqueen\n";
	}

	if (board.enPassantSquare != 255)
	{
		// std::cout << "Adding en passant \n";

		int rank = board.enPassantSquare / 8;
		int file = board.enPassantSquare % 8;

		if (board.turn == WHITE_TURN && rank == 5)
		{
			if ((file > 0 && board.bitboards[6] & (1ULL << (board.enPassantSquare - 1))) || 
				(file < 7 && board.bitboards[6] & (1ULL << (board.enPassantSquare + 1))))   
			{
				hash ^= settings.zobristEnPassant[file];
			}
		}
		else if (board.turn == BLACK_TURN && rank == 2)
		{
			if ((file > 0 && board.bitboards[0] & (1ULL << (board.enPassantSquare - 1))) ||
				(file < 7 && board.bitboards[0] & (1ULL << (board.enPassantSquare + 1))))
			{
				hash ^= settings.zobristEnPassant[file];
			}
		}
	}


	if (board.turn == WHITE_TURN) {
		// std::cout << "adding turn \n";
		hash ^= settings.zobristTurn;
	}

	return hash;
}

bool Utils::IsEnemyPieceAt(const Board& board, uint8_t position)
{
	return (board.turn == WHITE_TURN ? Utils::IsBlackPieceAt(board, position) : IsWhitePieceAt(board, position));
}

bool isOnFileA(uint8_t pos) { return pos % 8 == 0;  }
bool isOnFileH(uint8_t pos) { return pos % 8 == 7;  }

Bitboard Utils::RayAttacks(uint8_t from, int dir, Bitboard occupancy) {
	Bitboard attacks = 0ULL;
	int to = from;

	while (true) {
		if ((dir == -9 || dir == -1 || dir == 7) && isOnFileA(to)) break;
		if ((dir == -7 || dir == 1 || dir == 9) && isOnFileH(to)) break;

		to += dir;
		if (to < 0 || to >= 64) break;

		Bitboard toBB = 1ULL << to;
		attacks |= toBB;
		if (occupancy & toBB) break;
	}


	return attacks;
}

Bitboard Utils::GenerateBishopAttacks(int square, Bitboard occupancy) {
	Bitboard attacks = 0;
	attacks |= RayAttacks(square, -9, occupancy);
	attacks |= RayAttacks(square, -7, occupancy);
	attacks |= RayAttacks(square, 9, occupancy);
	attacks |= RayAttacks(square, 7, occupancy);
	return attacks;
}

Bitboard Utils::GenerateRookAttacks(int square, Bitboard occupancy) {
	Bitboard attacks = 0;
	attacks |= RayAttacks(square, -8, occupancy);
	attacks |= RayAttacks(square, 8, occupancy);
	attacks |= RayAttacks(square, -1, occupancy);
	attacks |= RayAttacks(square, 1, occupancy); 
	
	return attacks;
}

UndoInfo Utils::CreateUndoInfo(const Board& board, const Move& move)
{

	UndoInfo undo;
	undo.wCastlingKing = board.wCastlingKing;
	undo.wCastlingQueen =	board.wCastlingQueen;
	undo.bCastlingKing =	board.bCastlingKing;
	undo.bCastlingQueen =	board.bCastlingQueen;
	undo.enPassantSquare =	board.enPassantSquare;
	undo.turn =				board.turn;
	undo.turns =			board.turns;
	undo.move = move;
	undo.capturedPiece = Utils::GetPieceType(board, move.to);
	undo.promotedPiece = (move.promotion != 255 ? move.promotion : 255);
	undo.movedPiece = Utils::GetPieceType(board, move.from);

	return undo;
}

void Utils::DebugMove(const Move& move)
{
	std::cout
		<< "\nMOVE\n"
		<< "from: " << move.from << "\n"
		<< "to: " << move.to << "\n";
}

std::string Utils::ToLower(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

char Utils::ToLower(const char str) {
	return tolower(str);
}

uint8_t Utils::GetPromotionPiece(char letter, bool isWhite) {
	switch (tolower(letter)) {
	case 'q': return isWhite ? W_QUEEN_I	: B_QUEEN_I;
	case 'r': return isWhite ? W_ROOK_I		: B_ROOK_I;
	case 'b': return isWhite ? W_BISHOP_I	: B_BISHOP_I;
	case 'n': return isWhite ? W_KNIGHT_I	: B_KNIGHT_I;
	default: return 255;
	}
}

std::string Utils::MoveToStr(const Move& move)
{
	std::string res = "";

	if (!move.castling)
		res += Utils::ConvertToBoardPosition(move.from) + Utils::ConvertToBoardPosition(move.to) + ((move.promotion != 255 && move.promotion != 0) ? (char)move.promotion/*static_cast<char>(PIECE_CHAR[move.promotion])*/ : ' ');
	else
	{
		if (move.from == 4)
		{
			if (move.to == 7)
			{
				res += "e1g1";
			}
			else if (move.to == 0)
			{
				res += "e1c1";
			}
		}
		else if (move.from == 60)
		{
			if (move.to == 63)
			{
				res += "e8g8";
			}
			else if (move.to == 56)
			{
				res += "e8c8";
			}
		}
	}

	return res;
}

uint8_t Utils::BitScanForward(uint64_t bb) {
	if (bb == 0) return 64; // o 255 para invalid

	uint8_t index = 0;
	while ((bb & 1) == 0) {
		bb >>= 1;
		index++;
	}
	return index;
}

