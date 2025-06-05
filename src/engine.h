#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include "utils.h"
#include "board.h"
#include "constants.h"
#include "evaluation.h"
#include <chrono>
#include <random>
#include <fstream>

struct MoveEval;

class Engine
{
private:

	void GenerateZobristHash(int seed);
	static uint32_t polyglotSeed;

	uint32_t getRandomU32();
	uint64_t getRandomU64();

	static ZobristHashSettings polyglotSettings;
	ZobristHashSettings generatePolyglotSettings();

private:
	Board currentBoard;

	void ManageInput();
public:
	Engine();
	~Engine();

	static const int maxSearchDepth = 5;

	void init();
	void start();
	void stop();
	void reset();

	void InitKnightMasks();
	void InitKingMasks();

	void RunTest();
	void PlayAgainistItself();
	void PlayAgainistHuman();

	void StartPos();
	void SetPosition(const char* fen);

	MoveEval SearchTime(int time);
	MoveEval Search(int depth);
	void DivideTest(uint8_t depth);

	void MovePiece(const char* moveStr);

	void PrintBoard();

public:
	static Bitboard knightMasks[64];
	static Bitboard kingMasks[64];

	static std::vector<UndoInfo> undoStack;


	static ZobristHashSettings hashSettings;

};

#endif