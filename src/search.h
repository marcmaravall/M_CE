#include "evaluation.h"
#include "board.h"
#include "move.h"
#include "move_generation.h"
#include <iostream>
#include <vector>

struct MoveEval
{
	Move move;
	int eval;
};
MoveEval Minimax(Board& position, uint8_t depth, bool max);
MoveEval AlphaBeta(Board& position, uint8_t depth, int alpha, int beta, bool max);