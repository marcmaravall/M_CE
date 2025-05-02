#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <string>
#include "constants.h"

struct Board
{
private:
	uint64_t bitboards[12];
	bool wKingMoved;
	bool bKingMoved;

	bool wRookMoved[2];
	bool bRookMoved[2];

	bool turn;	// true = white, false = black
	int turns;

	uint8_t enPassantSquare;

public:
	Board(const char* fen);
	Board();
	~Board();

	void PrintBoard();
	std::string ConvertToFEN();
};

#endif
