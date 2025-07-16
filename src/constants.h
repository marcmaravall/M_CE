#pragma once
#define M_CE 1.0
#include "move.h"
#include <iostream>
#include <cstdint>

enum PIECE
{
	W_PAWN = 'P', W_KNIGHT = 'N', W_BISHOP = 'B', W_ROOK = 'R', W_QUEEN = 'Q', W_KING = 'K',
	B_PAWN = 'p', B_KNIGHT = 'n', B_BISHOP = 'b', B_ROOK = 'r', B_QUEEN = 'q', B_KING = 'k'
};

inline constexpr const char PIECE_CHAR[12] = {
	'P', 'N', 'B', 'R', 'Q', 'K',
	'p', 'n', 'b', 'r', 'q', 'k'
};

inline constexpr int STATIC_PIECE_VALUE[12] = {
	 100,  300,  330,  500,  900, 0,
	-100, -300, -330, -500, -900, 0,
};

inline constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline constexpr const char* TEST_FEN = "r1b5/pppp1Q1p/3k2p1/8/6P1/2N5/PPP2P1P/R1B1KB1R w KQ - 0 1";
// r2q3r/pp1k1pb1/2np3p/5b2/3N2p1/3P4/PP1QNPPP/R3KB1R b KQ - 0 1

// inline constexpr const std::pair<const char*, const uint64_t> FEN_TESTS[] = {{" "}};

inline constexpr const uint8_t NULL_SQUARE = 255;

typedef uint64_t Bitboard;
typedef uint64_t MagicNumber;
typedef uint64_t ZobristHash;

typedef struct ZobristHashSettings
{
	uint64_t zobristPieces[12][64];
	uint64_t zobristCastling[4];
	uint64_t zobristEnPassant[8];
	uint64_t zobristTurn;
} ZobristHashSettings;

inline constexpr int TT_SIZE = 1 << 20;

inline constexpr const char* BOARD_STRINGS[64] = {
	"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
	"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
	"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
	"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
	"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
	"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
	"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
	"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};

enum DIRECTIONS
{
	SOUTH = -8,
	WEST = -1,
	NORTH = -SOUTH,
	EAST = -WEST,
	SOUTH_WEST = SOUTH + WEST,
	SOUTH_EAST = SOUTH - WEST,
	NORTH_WEST = NORTH + WEST,
	NORTH_EAST = NORTH - WEST
};

typedef enum 
{
	WHITE = 0,
	BLACK = 1,

	BOTH = 2

} PIECE_COLORS;

inline constexpr const bool WHITE_TURN = true;
inline constexpr const bool BLACK_TURN = !WHITE_TURN;

constexpr uint64_t FILE_A_MASK = 0x0101010101010101ULL;
constexpr uint64_t FILE_H_MASK = 0x8080808080808080ULL;
constexpr uint64_t RANK_2_MASK = 0x000000000000FF00ULL;
constexpr uint64_t RANK_7_MASK = 0x00FF000000000000ULL;

constexpr const uint8_t CENTIPAWN_VAL = 100;

inline constexpr const int W_KNIGHT_BITMAP[64] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50
};

inline constexpr const int W_BISHOP_BITMAP[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};

inline constexpr const int W_ROOK_BITMAP[64] = {
	 0,  0,  0,  5,  5,  0,  0,  0,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	 5, 10, 10, 10, 10, 10, 10,  5,
	 0,  0,  0,  0,  0,  0,  0,  0
};

inline constexpr const int W_KING_BITMAP_MIDGAME[64] = {
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	 20, 20,  0,  0,  0,  0, 20, 20,
	 20, 30, 10,  0,  0, 10, 30, 20
};

inline constexpr const int W_KING_BITMAP_ENDGAME[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

inline constexpr const int W_QUEEN_BITMAP[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	  0,  0,  5,  5,  5,  5,  0, -5,
	-10,  5,  5,  5,  5,  5,  0,-10,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};

inline constexpr const int W_PAWN_BITMAP[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	 5, 10, 10,-20,-20, 10, 10,  5,
	 5, -5,-10,  0,  0,-10, -5,  5,
	 0,  0,  0, 20, 20,  0,  0,  0,
	 5,  5, 10, 25, 25, 10,  5,  5,
	10, 10, 20, 30, 30, 20, 10, 10,
	50, 50, 50, 50, 50, 50, 50, 50,
	 0,  0,  0,  0,  0,  0,  0,  0
};

inline constexpr const int B_KNIGHT_BITMAP[64] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50
};

inline constexpr const int B_BISHOP_BITMAP[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};

inline constexpr const int B_ROOK_BITMAP[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	 5, 10, 10, 10, 10, 10, 10,  5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	 0,  0,  0,  5,  5,  0,  0,  0
};

inline constexpr const int B_KING_BITMAP_MIDGAME[64] = {
	 20, 30, 10,  0,  0, 10, 30, 20,
	 20, 20,  0,  0,  0,  0, 20, 20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30
};

inline constexpr const int B_QUEEN_BITMAP[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-10,  5,  5,  5,  5,  5,  0,-10,
	  0,  0,  5,  5,  5,  5,  0, -5,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};

inline constexpr const int B_PAWN_BITMAP[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	 5,  5, 10, 25, 25, 10,  5,  5,
	 0,  0,  0, 20, 20,  0,  0,  0,
	 5, -5,-10,  0,  0,-10, -5,  5,
	 5, 10, 10,-20,-20, 10, 10,  5,
	 0,  0,  0,  0,  0,  0,  0,  0
};

inline constexpr const int* PIECE_SQUARE_TABLES[12] = {
	W_PAWN_BITMAP,
	W_KNIGHT_BITMAP,
	W_BISHOP_BITMAP,
	W_ROOK_BITMAP,
	W_QUEEN_BITMAP,
	W_KING_BITMAP_MIDGAME,
	B_PAWN_BITMAP,
	B_KNIGHT_BITMAP,
	B_BISHOP_BITMAP,
	B_ROOK_BITMAP,
	B_QUEEN_BITMAP,
	B_KING_BITMAP_MIDGAME
};


const typedef enum
{
	W_PAWN_I = 0, W_KNIGHT_I = 1, W_BISHOP_I = 2, W_ROOK_I = 3, W_QUEEN_I = 4, W_KING_I = 5,
	B_PAWN_I = 6, B_KNIGHT_I = 7, B_BISHOP_I = 8, B_ROOK_I = 9, B_QUEEN_I = 10, B_KING_I = 11,
} PIECE_INDEX;

// attacker = rows, victim = columns
// 0: Pawn, 1: Knight, 2: Bishop, 3: Rook, 4: Queen, 5: King (
inline constexpr int MVV_LVA[6][6] = {
	// P    N    B    R    Q    K
	{105, 205, 305, 405, 505, 0}, // attacker = P
	{104, 204, 304, 404, 504, 0}, // attacker = N
	{103, 203, 303, 403, 503, 0}, // attacker = B
	{102, 202, 302, 402, 502, 0}, // attacker = R
	{101, 201, 301, 401, 501, 0}, // attacker = Q
	{100, 200, 300, 400, 500, 0}, // attacker = K
};


struct UndoInfo {
	bool wCastlingKing;
	bool wCastlingQueen;
	bool bCastlingKing;
	bool bCastlingQueen;

	uint8_t enPassantSquare;

	bool turn;
	int turns;

	Move move;

	uint8_t capturedPiece;
	uint8_t promotedPiece;

	uint8_t movedPiece;
};

inline const int MATE_SCORE = 1'000'000'000;
