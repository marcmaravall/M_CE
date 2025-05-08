#ifndef MOVE_H
	#define MOVE_H 
#include <iostream>

struct Move
{
	uint8_t from;
	uint8_t where;

	bool castling;
	bool promotion;
	bool capture;
	bool check;
};

#endif
