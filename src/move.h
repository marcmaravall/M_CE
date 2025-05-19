#ifndef MOVE_H
	#define MOVE_H 
#include <iostream>

struct Move
{
	uint8_t from;
	uint8_t to;

	bool castling;
	bool mode;

	uint8_t promotion;
	bool capture;
	uint8_t capturedPiece;

	bool check;
	bool checkmate;
};

#endif
