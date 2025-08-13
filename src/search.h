#pragma once

#include "evaluation.h"
#include "board.h"
#include "move.h"
#include "book.h"
#include "move_generation.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

extern int NODES;
extern int ALPHA_BETA_PRUNINGS;

struct MoveEval
{
	Move move;
	int eval = 0;
	int priority;

	int weight;	// for polyglot

	MoveEval operator-()
	{
		return { move, -eval, 0, 0 };
	}
};

enum BoundType
{
	LOWERBOUND,
	UPPERBOUND,
	EXACT
};

struct TTEntry
{
	ZobristHash hash;
	int depth;
	MoveEval bestMove;
	BoundType bound;
};

extern std::array<TTEntry, TT_SIZE> TranspositionTable;

MoveEval Minimax(Board& position, uint8_t depth, bool max);
MoveEval AlphaBeta(Board& position, const uint8_t depth, int alpha, int beta, const bool max, const int ply);
MoveEval Quiesce(Board& position, int alpha, int beta, bool max, int depth);

uint64_t Perft(Board& position, int depth);
void Divide(Board& pos, int position);
void AddToTranspositionTable(const Board& board);
