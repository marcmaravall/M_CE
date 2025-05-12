#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include "utils.h"
#include "board.h"
#include "constants.h"

class Engine
{
private:

private:
	Board currentBoard;

	void ManageInput();
public:
	Engine();
	~Engine();
	

	void init();
	void start();
	void stop();
	void reset();

	void InitKnightMasks();

	void RunTest();

public:
	static Bitboard knightMasks[64];

};

#endif