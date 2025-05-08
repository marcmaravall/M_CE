#pragma once
#define M_CE 1.0

enum PIECE
{
	W_PAWN = 'P', W_KNIGHT = 'N', W_BISHOP = 'B', W_ROOK = 'R', W_QUEEN = 'Q', W_KING = 'K',
	B_PAWN = 'p', B_KNIGHT = 'n', B_BISHOP = 'b', B_ROOK = 'r', B_QUEEN = 'q', B_KING = 'k'
};

inline constexpr const char PIECE_CHAR[12] = {
	'P', 'N', 'B', 'R', 'Q', 'K',
	'p', 'n', 'b', 'r', 'q', 'k'
};

inline constexpr int STATIC_PIECE_VALUE[10] = {
	 100,  300,  330,  500,  900, 
	-100, -300, -330, -500, -900,
};

inline constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline constexpr const char* TEST_FEN = "8/P7/8/8/8/8/8/8 w - - 0 1";

typedef uint64_t Bitboard;

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


