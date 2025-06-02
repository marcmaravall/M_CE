#pragma once

#ifndef MOVE_H
	#define MOVE_H 
#include <iostream>

struct Move
{
	int from;
	int to;

	bool castling;
	bool mode;

	int promotion = 255;
	bool capture;
	uint8_t capturedPiece;

	bool check;
	bool checkmate;
};

#endif
