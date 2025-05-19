#pragma once

#include "evaluation.h"
#include "board.h"
#include "move.h"
#include "move_generation.h"
#include <iostream>
#include <vector>

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
MoveEval AlphaBeta(Board& position, uint8_t depth, int alpha, int beta, bool max);
