#pragma once

#include "evaluation.h"
#include "board.h"
#include "move.h"
#include "move_generation.h"
#include <iostream>
#include <vector>
#include <fstream>

extern int NODES;
extern int ALPHA_BETA_PRUNINGS;

struct MoveEval
{
	Move move;
	int eval;
	int priority;
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

extern TTEntry TranspositionTable[TT_SIZE];

MoveEval Minimax(Board& position, uint8_t depth, bool max);
MoveEval AlphaBeta(Board& position, uint8_t depth, int alpha, int beta, bool max, int ply);
uint64_t Perft(Board& position, int depth);
void Divide(Board& pos, int position);
