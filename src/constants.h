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
constexpr uint64_t FILE_B_MASK = 0x0202020202020202ULL;
constexpr uint64_t FILE_C_MASK = 0x0404040404040404ULL;
constexpr uint64_t FILE_D_MASK = 0x0808080808080808ULL;
constexpr uint64_t FILE_E_MASK = 0x1010101010101010ULL;
constexpr uint64_t FILE_F_MASK = 0x2020202020202020ULL;
constexpr uint64_t FILE_G_MASK = 0x4040404040404040ULL;
constexpr uint64_t FILE_H_MASK = 0x8080808080808080ULL;

inline constexpr uint64_t FILES_MASKS[8] = {
	FILE_A_MASK, FILE_B_MASK, FILE_C_MASK, FILE_D_MASK,
	FILE_E_MASK, FILE_F_MASK, FILE_G_MASK, FILE_H_MASK
};

constexpr uint64_t RANK_2_MASK = 0x000000000000FF00ULL;
constexpr uint64_t RANK_7_MASK = 0x00FF000000000000ULL;

constexpr const uint8_t CENTIPAWN_VAL = 100;

// white pawns
inline constexpr const int W_PAWN_BITMAP_OPENING[64] = {
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
         -  5, -2,  2,  5,  5,  2, -2, -5,
          - 8,  0,  5, 12, 12,  5,  0, -8,
          - 10,  2,  8, 18, 18,  8,  2,-10,
          - 12,  5, 12, 25, 25, 12,  5,-12,
          - 15, 10, 20, 35, 35, 20, 10,-15,
            0,  0,  0,  0,  0,  0,  0,  0,
};

inline constexpr const int W_PAWN_BITMAP_MIDGAME[64] = {
        0,  0,  0,  0,  0,  0,  0,  0,
        2,  5,  8, 10, 10,  8,  5,  2,
        0,  3,  8, 12, 12,  8,  3,  0,
        0,  5, 10, 15, 15, 10,  5,  0,
        0,  8, 12, 20, 20, 12,  8,  0,
       10, 10, 18, 30, 30, 18, 10, 10,
       15, 15, 25, 40, 40, 25, 15, 15,
        0,  0,  0,  0,  0,  0,  0,  0,
};

inline constexpr const int W_PAWN_BITMAP_ENDGAME[64] = {
       0,  0,  0,  0,  0,  0,  0,  0,
       10, 15, 20, 25, 25, 20, 15, 10,
       20, 25, 30, 35, 35, 30, 25, 20,
       35, 40, 45, 50, 50, 45, 40, 35,
       50, 55, 60, 65, 65, 60, 55, 50,
       70, 75, 80, 90, 90, 80, 75, 70,
      100,110,120,140,140,120,110,100,
        0,  0,  0,  0,  0,  0,  0,  0,
};
inline constexpr const int W_KNIGHT_BITMAP_OPENING[64] = {
    -40,-20,-15,-10,-10,-15,-20,-40,
    -20, -8, -2,  2,  2, -2, -8,-20,
    -15, -2,  8, 12, 12,  8, -2,-15,
    -10,  2, 12, 18, 18, 12,  2,-10,
    -10,  2, 12, 18, 18, 12,  2,-10,
    -15, -2,  8, 12, 12,  8, -2,-15,
    -20, -5,  2,  5,  5,  2, -5,-20,
    -40,-20,-10, -5, -5,-10,-20,-40
};

inline constexpr const int W_KNIGHT_BITMAP_MIDGAME[64] = {
    -35,-15,-10, -5, -5,-10,-15,-35,
    -15, -5,  5, 10, 10,  5, -5,-15,
    -10,  5, 15, 20, 20, 15,  5,-10,
     -5, 10, 20, 25, 25, 20, 10, -5,
     -5, 10, 20, 25, 25, 20, 10, -5,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -15, -2,  8, 12, 12,  8, -2,-15,
    -35,-15,-10, -5, -5,-10,-15,-35
};

inline constexpr const int W_KNIGHT_BITMAP_ENDGAME[64] = {
    -20,-10, -5, -2, -2, -5,-10,-20,
    -10,  0,  5,  8,  8,  5,  0,-10,
     -5,  5, 12, 15, 15, 12,  5, -5,
     -2,  8, 15, 18, 18, 15,  8, -2,
     -2,  8, 15, 18, 18, 15,  8, -2,
     -5,  5, 12, 15, 15, 12,  5, -5,
    -10,  0,  5,  8,  8,  5,  0,-10,
    -20,-10, -5, -2, -2, -5,-10,-20
};

inline constexpr const int W_BISHOP_BITMAP_OPENING[64] = {
    -10, -5,  0,  2,  2,  0, -5,-10,
     -5,  5,  8, 10, 10,  8,  5, -5,
      0,  8, 12, 15, 15, 12,  8,  0,
      2, 10, 15, 18, 18, 15, 10,  2,
      2, 10, 15, 18, 18, 15, 10,  2,
      0,  8, 12, 15, 15, 12,  8,  0,
     -2, 10,  5, 12, 12,  5, 10, -2,
    -10, -5,-20,  0,  0,-20, -5,-10
};

inline constexpr const int W_BISHOP_BITMAP_MIDGAME[64] = {
     -5,  0,  2,  5,  5,  2,  0, -5,
      0,  8, 10, 12, 12, 10,  8,  0,
      2, 10, 15, 18, 18, 15, 10,  2,
      5, 12, 18, 22, 22, 18, 12,  5,
      5, 12, 18, 22, 22, 18, 12,  5,
      2, 10, 15, 18, 18, 15, 10,  2,
      0, 15,  8, 12, 12,  8, 15,  0,
     -5,  0,-15,  2,  2,-15,  0, -5
};

inline constexpr const int W_BISHOP_BITMAP_ENDGAME[64] = {
      0,  5,  8, 10, 10,  8,  5,  0,
      5, 12, 15, 18, 18, 15, 12,  5,
      8, 15, 20, 25, 25, 20, 15,  8,
     10, 18, 25, 30, 30, 25, 18, 10,
     10, 18, 25, 30, 30, 25, 18, 10,
      8, 15, 20, 25, 25, 20, 15,  8,
      5, 20, 12, 18, 18, 12, 20,  5,
      0,  5,  0, 10, 10,  0,  5,  0
};

inline constexpr const int W_ROOK_BITMAP_OPENING[64] = {
      0,  2,  5,  8,  8,  5,  2,  0,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
      8, 12, 15, 18, 18, 15, 12,  8,
      0,  5,  8, 10, 10,  8,  5,  0
};

inline constexpr const int W_ROOK_BITMAP_MIDGAME[64] = {
      2,  5,  8, 12, 12,  8,  5,  2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     15, 20, 25, 30, 30, 25, 20, 15,
      5, 10, 12, 15, 15, 12, 10,  5
};

inline constexpr const int W_ROOK_BITMAP_ENDGAME[64] = {
      5, 10, 12, 15, 15, 12, 10,  5,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
     20, 25, 30, 35, 35, 30, 25, 20,
     10, 15, 18, 22, 22, 18, 15, 10
};


inline constexpr const int W_KING_BITMAP_OPENING[64] = {
     30, 40, 20,  0,  0, 20, 40, 30,
     20, 30, 20, 10, 10, 20, 30, 20,
      0, 10, 20, 30, 30, 20, 10,  0,
    -10,  0, 10, 20, 20, 10,  0,-10,
    -20,-10,  0, 10, 10,  0,-10,-20,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -40,-30,-20,-10,-10,-20,-30,-40,
    -50,-40,-30,-20,-20,-30,-40,-50,
};

inline constexpr const int W_KING_BITMAP_MIDGAME[64] = {
     20, 30, 15,  0,  0, 15, 30, 20,
     10, 20, 15,  5,  5, 15, 20, 10,
    -10,  0, 10, 20, 20, 10,  0,-10,
    -20,-10,  0, 10, 10,  0,-10,-20,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -40,-30,-20,-10,-10,-20,-30,-40,
    -50,-40,-30,-20,-20,-30,-40,-50,
    -60,-50,-40,-30,-30,-40,-50,-60,
};

inline constexpr const int W_KING_BITMAP_ENDGAME[64] = {
    -20,-10,  0, 10, 10,  0,-10,-20,
    -10,  0, 10, 20, 20, 10,  0,-10,
      0, 10, 20, 30, 30, 20, 10,  0,
     10, 20, 30, 40, 40, 30, 20, 10,
     10, 20, 30, 40, 40, 30, 20, 10,
      0, 10, 20, 30, 30, 20, 10,  0,
    -10,  0, 10, 20, 20, 10,  0,-10,
    -20,-10,  0, 10, 10,  0,-10,-20
};

inline constexpr const int W_QUEEN_BITMAP_OPENING[64] = {
    -80, -80, -80,   0,   0, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80
};

inline constexpr const int W_QUEEN_BITMAP_MIDGAME[64] = {
    -10, -5,  0,  2,  2,  0, -5,-10,
     -5,  2,  5,  8,  8,  5,  2, -5,
      0,  5,  5,  5,  5,  5,  5,  0,
      2,  8,  5,  5,  5,  5,  8,  2,
      2,  8,  5,  5,  5,  5,  8,  2,
      0,  5,  5,  5,  5,  5,  5,  0,
     -5,  0,  5,  8,  8,  5,  0, -5,
    -10, -5,  0,  2,  2,  0, -5,-10
};

inline constexpr const int W_QUEEN_BITMAP_ENDGAME[64] = {
     -5,  0,  5, 10, 10,  5,  0, -5,
      0,  8, 12, 15, 15, 12,  8,  0,
      5, 12, 18, 22, 22, 18, 12,  5,
     10, 15, 22, 28, 28, 22, 15, 10,
     10, 15, 22, 28, 28, 22, 15, 10,
      5, 12, 18, 22, 22, 18, 12,  5,
      0,  8, 12, 15, 15, 12,  8,  0,
     -5,  0,  5, 10, 10,  5,  0, -5
};

inline constexpr const int B_PAWN_BITMAP_OPENING[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
    -15, 10, 20, 35, 35, 20, 10,-15,
    -12,  5, 12, 25, 25, 12,  5,-12,
    -10,  2,  8, 18, 18,  8,  2,-10,
     -8,  0,  5, 12, 12,  5,  0, -8,
     -5, -2,  2,  5,  5,  2, -2, -5,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
};

inline constexpr const int B_PAWN_BITMAP_MIDGAME[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
    -10, 15, 25, 40, 40, 25, 15,-10,
     -8, 10, 18, 30, 30, 18, 10, -8,
     -5,  8, 12, 20, 20, 12,  8, -5,
     -2,  5, 10, 15, 15, 10,  5, -2,
      0,  3,  8, 12, 12,  8,  3,  0,
      2,  5,  8, 10, 10,  8,  5,  2,
      0,  0,  0,  0,  0,  0,  0,  0,
};

inline constexpr const int B_PAWN_BITMAP_ENDGAME[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
    100,110,120,140,140,120,110,100,
     70, 75, 80, 90, 90, 80, 75, 70,
     50, 55, 60, 65, 65, 60, 55, 50,
     35, 40, 45, 50, 50, 45, 40, 35,
     20, 25, 30, 35, 35, 30, 25, 20,
     10, 15, 20, 25, 25, 20, 15, 10,
      0,  0,  0,  0,  0,  0,  0,  0,
};

inline constexpr const int B_KNIGHT_BITMAP_OPENING[64] = {
    -40,-20,-10, -5, -5,-10,-20,-40,
    -20, -5,  2,  5,  5,  2, -5,-20,
    -15, -2,  8, 12, 12,  8, -2,-15,
    -10,  2, 12, 18, 18, 12,  2,-10,
    -10,  2, 12, 18, 18, 12,  2,-10,
    -15, -2,  8, 12, 12,  8, -2,-15,
    -20, -8, -2,  2,  2, -2, -8,-20,
    -40,-20,-15,-10,-10,-15,-20,-40,
};

inline constexpr const int B_KNIGHT_BITMAP_MIDGAME[64] = {
    -35,-15,-10, -5, -5,-10,-15,-35,
    -15, -2,  8, 12, 12,  8, -2,-15,
    -10,  5, 15, 20, 20, 15,  5,-10,
     -5, 10, 20, 25, 25, 20, 10, -5,
     -5, 10, 20, 25, 25, 20, 10, -5,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -15, -5,  5, 10, 10,  5, -5,-15,
    -35,-15,-10, -5, -5,-10,-15,-35
};

inline constexpr const int B_KNIGHT_BITMAP_ENDGAME[64] = {
    -20,-10, -5, -2, -2, -5,-10,-20,
    -10,  0,  5,  8,  8,  5,  0,-10,
     -5,  5, 12, 15, 15, 12,  5, -5,
     -2,  8, 15, 18, 18, 15,  8, -2,
     -2,  8, 15, 18, 18, 15,  8, -2,
     -5,  5, 12, 15, 15, 12,  5, -5,
    -10,  0,  5,  8,  8,  5,  0,-10,
    -20,-10, -5, -2, -2, -5,-10,-20
};

inline constexpr const int B_BISHOP_BITMAP_OPENING[64] = {
    -10, -5,-20,  0,  0,-20, -5,-10,
     -2, 10,  5, 12, 12,  5, 10, -2,
      0,  8, 12, 15, 15, 12,  8,  0,
      2, 10, 15, 18, 18, 15, 10,  2,
      2, 10, 15, 18, 18, 15, 10,  2,
      0,  8, 12, 15, 15, 12,  8,  0,
     -5,  5,  8, 10, 10,  8,  5, -5,
    -10, -5,  0,  2,  2,  0, -5,-10,
};

inline constexpr const int B_BISHOP_BITMAP_MIDGAME[64] = {
     -5,  0,-15,  2,  2,-15,  0, -5,
      0, 15,  8, 12, 12,  8, 15,  0,
      2, 10, 15, 18, 18, 15, 10,  2,
      5, 12, 18, 22, 22, 18, 12,  5,
      5, 12, 18, 22, 22, 18, 12,  5,
      2, 10, 15, 18, 18, 15, 10,  2,
      0,  8, 10, 12, 12, 10,  8,  0,
     -5,  0,  2,  5,  5,  2,  0, -5,
};

inline constexpr const int B_BISHOP_BITMAP_ENDGAME[64] = {
      0,  5,  0, 10, 10,  0,  5,  0,
      5, 20, 12, 18, 18, 12, 20,  5,
      8, 15, 20, 25, 25, 20, 15,  8,
     10, 18, 25, 30, 30, 25, 18, 10,
     10, 18, 25, 30, 30, 25, 18, 10,
      8, 15, 20, 25, 25, 20, 15,  8,
      5, 12, 15, 18, 18, 15, 12,  5,
      0,  5,  8, 10, 10,  8,  5,  0,
};

inline constexpr const int B_ROOK_BITMAP_OPENING[64] = {
      0,  5,  8, 10, 10,  8,  5,  0,
      8, 12, 15, 18, 18, 15, 12,  8,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
     -5,  0,  2,  5,  5,  2,  0, -5,
      0,  2,  5,  8,  8,  5,  2,  0,
};

inline constexpr const int B_ROOK_BITMAP_MIDGAME[64] = {
      5, 10, 12, 15, 15, 12, 10,  5,
     15, 20, 25, 30, 30, 25, 20, 15,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
     -2,  2,  5,  8,  8,  5,  2, -2,
      2,  5,  8, 12, 12,  8,  5,  2,
};

inline constexpr const int B_ROOK_BITMAP_ENDGAME[64] = {
     10, 15, 18, 22, 22, 18, 15, 10,
     20, 25, 30, 35, 35, 30, 25, 20,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      2,  5,  8, 12, 12,  8,  5,  2,
      5, 10, 12, 15, 15, 12, 10,  5,
};

inline constexpr const int B_KING_BITMAP_OPENING[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -40,-30,-20,-10,-10,-20,-30,-40,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -20,-10,  0, 10, 10,  0,-10,-20,
    -10,  0, 10, 20, 20, 10,  0,-10,
      0, 10, 20, 30, 30, 20, 10,  0,
     20, 30, 20, 10, 10, 20, 30, 20,
     30, 40, 20,  0,  0, 20, 40, 30,
};

inline constexpr const int B_KING_BITMAP_MIDGAME[64] = {
    -60,-50,-40,-30,-30,-40,-50,-60,
    -50,-40,-30,-20,-20,-30,-40,-50,
    -40,-30,-20,-10,-10,-20,-30,-40,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -20,-10,  0, 10, 10,  0,-10,-20,
    -10,  0, 10, 20, 20, 10,  0,-10,
     10, 20, 15,  5,  5, 15, 20, 10,
     20, 30, 15,  0,  0, 15, 30, 20,
};

inline constexpr const int B_KING_BITMAP_ENDGAME[64] = {
    -20,-10,  0, 10, 10,  0,-10,-20,
    -10,  0, 10, 20, 20, 10,  0,-10,
      0, 10, 20, 30, 30, 20, 10,  0,
     10, 20, 30, 40, 40, 30, 20, 10,
     10, 20, 30, 40, 40, 30, 20, 10,
      0, 10, 20, 30, 30, 20, 10,  0,
    -10,  0, 10, 20, 20, 10,  0,-10,
    -20,-10,  0, 10, 10,  0,-10,-20,
};

// black queen

inline constexpr const int B_QUEEN_BITMAP_OPENING[64] = {
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80, -80, -80, -80, -80, -80,
    -80, -80, -80,   0,   0, -80, -80, -80
};

inline constexpr const int B_QUEEN_BITMAP_MIDGAME[64] = {
    -10, -5,  0,  2,  2,  0, -5,-10,
     -5,  2,  5,  8,  8,  5,  2, -5,
      0,  5,  5,  5,  5,  5,  5,  0,
      2,  8,  5,  5,  5,  5,  8,  2,
      2,  8,  5,  5,  5,  5,  8,  2,
      0,  5,  5,  5,  5,  5,  5,  0,
     -5,  0,  5,  8,  8,  5,  0, -5,
    -10, -5,  0,  2,  2,  0, -5,-10
};

inline constexpr const int B_QUEEN_BITMAP_ENDGAME[64] = {
    -5,  0,  5, 10, 10,  5,  0, -5,
      0,  8, 12, 15, 15, 12,  8,  0,
      5, 12, 18, 22, 22, 18, 12,  5,
     10, 15, 22, 28, 28, 22, 15, 10,
     10, 15, 22, 28, 28, 22, 15, 10,
      5, 12, 18, 22, 22, 18, 12,  5,
      0,  8, 12, 15, 15, 12,  8,  0,
     -5,  0,  5, 10, 10,  5,  0, -5
};

inline constexpr const int* PSE_OPENING[12]{
	W_PAWN_BITMAP_OPENING,
	W_KNIGHT_BITMAP_OPENING,
	W_BISHOP_BITMAP_OPENING,
	W_ROOK_BITMAP_OPENING,
	W_QUEEN_BITMAP_OPENING,
	W_KING_BITMAP_OPENING,
	B_PAWN_BITMAP_OPENING,
	B_KNIGHT_BITMAP_OPENING,
	B_BISHOP_BITMAP_OPENING,
	B_ROOK_BITMAP_OPENING,
	B_QUEEN_BITMAP_OPENING,
	B_KING_BITMAP_OPENING,
};

inline constexpr const int* PSE_MIDGAME[12]{
	W_PAWN_BITMAP_MIDGAME,
	W_KNIGHT_BITMAP_MIDGAME,
	W_BISHOP_BITMAP_MIDGAME,
	W_ROOK_BITMAP_MIDGAME,
	W_QUEEN_BITMAP_MIDGAME,
	W_KING_BITMAP_MIDGAME,
	B_PAWN_BITMAP_MIDGAME,
	B_KNIGHT_BITMAP_MIDGAME,
	B_BISHOP_BITMAP_MIDGAME,
	B_ROOK_BITMAP_MIDGAME,
	B_QUEEN_BITMAP_MIDGAME,
	B_KING_BITMAP_MIDGAME,
};

inline constexpr const int* PSE_ENDGAME[12]{
	W_PAWN_BITMAP_ENDGAME,
	W_KNIGHT_BITMAP_ENDGAME,
	W_BISHOP_BITMAP_ENDGAME,
	W_ROOK_BITMAP_ENDGAME,
	W_QUEEN_BITMAP_ENDGAME,
	W_KING_BITMAP_ENDGAME,
	B_PAWN_BITMAP_ENDGAME,
	B_KNIGHT_BITMAP_ENDGAME,
	B_BISHOP_BITMAP_ENDGAME,
	B_ROOK_BITMAP_ENDGAME,
	B_QUEEN_BITMAP_ENDGAME,
	B_KING_BITMAP_ENDGAME,
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
	bool turn;

	uint8_t enPassantSquare;

	uint8_t capturedPiece = 255;
	uint8_t promotedPiece = 255;

	uint8_t movedPiece;
	Move move;
	
	int turns;
};

inline const int MATE_SCORE = 1'000'000'000;

typedef struct {
	Bitboard mask;
	Bitboard magic;
	uint8_t shift;
} Magic;

typedef enum {
	OPENING = 0,
	MIDGAME = 1,
	ENDGAME = 2

} GAME_PHASE;

// game phase value
/*typedef enum {
	PAWN_GP_VALUE = 1,
	KNIGHT_GP_VALUE = 2,
	BISHOP_GP_VALUE = 2,
	ROOK_GP_VALUE = 3,
	KNIGHT_GP_VALUE = 4,
} GAME_PHASE_VALUE;*/

// < 24 midgame, < 10 endgame
inline constexpr const uint8_t MID_GAME_SCORE = 24;
inline constexpr const uint8_t END_GAME_SCORE = 10;

inline constexpr const uint8_t MAX_PLY = 256;

inline constexpr const int OPENING_VALUE = 5200;
inline constexpr const int MIDGAME_VALUE = 2300;
inline constexpr const int ENDGAME_VALUE = 1500;

inline constexpr const int GAME_PHASE_VALUE[5] = {
//   P    K    B    R    Q
	100, 200, 200, 300, 400
};
