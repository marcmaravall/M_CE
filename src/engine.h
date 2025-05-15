#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include "utils.h"
#include "board.h"
#include "constants.h"
#include "evaluation.h"
#include "search.h"
#include <chrono>

class Engine
{
private:

private:
	Evaluation eval;
	Board currentBoard;

	const int maxSearchDepth = 5;

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
	void PlayAgainistItself();
	void PlayAgainistHuman();

public:
	static Bitboard knightMasks[64];

};

#endif