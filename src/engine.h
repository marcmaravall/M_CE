#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include "utils.h"
#include "board.h"
#include "constants.h"
#include "evaluation.h"
#include "search.h"
#include <chrono>
#include <random>
#include <fstream>

class Engine
{
private:

	void GenerateZobristHash(int seed);

private:
	Board currentBoard;

	const int maxSearchDepth = 3;

	void ManageInput();
public:
	Engine();
	~Engine();

	void init();
	void start();
	void stop();
	void reset();

	void InitKnightMasks();
	void InitKingMasks();

	void RunTest();
	void PlayAgainistItself();
	void PlayAgainistHuman();

public:
	static Bitboard knightMasks[64];
	static Bitboard kingMasks[64];

	static std::vector<UndoInfo> undoStack;


	static ZobristHashSettings hashSettings;

};

#endif