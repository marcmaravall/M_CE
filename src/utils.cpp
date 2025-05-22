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
	for (size_t i = 0; i < 12; i++) {
		std::cout << "\n" << board.bitboards[i] << "\n";
		for (int j = 0; j < 64; j++) {
			if (j % 8 == 0)
				std::cout << "\n";
			std::cout << GetBitboardValueOnIndex(board.bitboards[i], j);
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

	fen += " 0 1";

	return fen;
}

std::string Utils::ConvertToBoardPosition(uint8_t squareIndex) {
	return squareIndex < 64 ? (std::string)BOARD_STRINGS[squareIndex] : "null";
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
		uint64_t bitboard = board.bitboards[i];
		while (bitboard)
		{
			int square = PopLSB(bitboard);
			hash ^= settings.zobristPieces[i][square];
		}
	}

	if (board.wCastlingKing)
		hash ^= settings.zobristCastling[0];
	if (board.wCastlingQueen)
		hash ^= settings.zobristCastling[1];
	if (board.bCastlingKing)
		hash ^= settings.zobristCastling[2];
	if (board.bCastlingQueen)
		hash ^= settings.zobristCastling[3];


	if (board.enPassantSquare != (uint8_t)-1)
	{
		int file = board.enPassantSquare % 8;
		hash ^= settings.zobristEnPassant[file];
	}

	if (board.turn == BLACK_TURN)
		hash ^= settings.zobristTurn;

	return hash;
}

bool Utils::IsEnemyPieceAt(const Board& board, uint8_t position)
{
	return (board.turn == WHITE_TURN ? Utils::IsBlackPieceAt(board, position) : IsWhitePieceAt(board, position));
}

Bitboard Utils::RayAttacks(uint8_t from, uint8_t dir, Bitboard occupancy) {
	Bitboard attacks = 0ULL;
	int to = from;

	while (true) {
		to += dir;

		if (to < 0 || to >= 64) break;

		if (abs((to % 8) - (from % 8)) > 1 && (dir == -9 || dir == -1 || dir == 7 || dir == 1 || dir == 9)) break;

		Bitboard toBB = 1ULL << to;
		attacks |= toBB;

		if (occupancy & toBB) break;

		from = to;
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

	return undo;
}

