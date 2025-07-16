#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include "utils.h"
#include "board.h"
#include "book.h"
#include "constants.h"
#include "evaluation.h"
#include "search.h"
// #include "uci.h"
#include "move.h"
#include <chrono>
#include <random>
#include <fstream>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;
struct MoveEval;

class Engine
{
private:
	const std::string programName = "M_CE";
	std::string programDir;
	fs::path getProjectDirectory();

private:

	void GenerateZobristHash(int seed);
	static uint32_t polyglotSeed;

	uint32_t getRandomU32();
	uint64_t getRandomU64();

	ZobristHashSettings generatePolyglotSettings();

private:
	// Board currentBoard;

	void ManageInput();
public:
	Board currentBoard = Board(START_FEN);

	Engine();
	~Engine();

	static const int maxSearchDepth = 5;

	void init();
	void start();
	void stop();
	void reset();

	// masks and tables
	void InitKnightMasks();
	void InitKingMasks();
	void InitBetweenTable();
	void InitBishopTable();
	void InitRookTable();

	void GenerateMagicNumbers();

	// tests
	void RunTest();
	void RunBookTest();

	void PlayAgainistItself();
	void PlayAgainistHuman();

	void StartPos();
	void SetPosition(const char* fen);

	MoveEval SearchTime(int time);
	MoveEval Search(int depth);
	void DivideTest(uint8_t depth);

	void MovePiece(const char* moveStr);

	void PrintBoard();

	void DebugTables();

public:
	static Bitboard knightMasks[64];
	static Bitboard kingMasks[64];

	static Bitboard rookAttackTable[64][4096];
	static Bitboard bishopAttackTable[64][4096];

	static uint64_t rookMagics[64];
	static uint64_t bishopMagics[64];
	
	static Bitboard between[64][64];

	static std::vector<UndoInfo> undoStack;

	static ZobristHashSettings hashSettings;
	static ZobristHashSettings polyglotSettings;

	static Book book;

public:
	int timeSearching = 0;
};

#endif
