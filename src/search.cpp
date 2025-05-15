#include "search.h"

MoveEval Minimax(Board& position, uint8_t depth, bool max)
{
	if (depth == 0)
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

	std::vector<Move> moves = GenerateLegalMoves(position);
	if (moves.empty())
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

	MoveEval bestMove;
	if (max)
	{
		bestMove.eval = -1000000;
		for (const Move& move : moves)
		{
			Board newPosition = position;
			newPosition.MovePiece(move);

			MoveEval result = Minimax(newPosition, depth - 1, false);
			if (result.eval > bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
		}
	}
	else
	{
		bestMove.eval = 1000000;
		for (const Move& move : moves)
		{
			Board newPosition = position;
			newPosition.MovePiece(move);

			MoveEval result = Minimax(newPosition, depth - 1, true);
			if (result.eval < bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
		}
	}

	return bestMove;
}

// reduces complexity O(n^2) to O(n) by using alpha-beta pruning
MoveEval AlphaBeta(Board& position, uint8_t depth, int alpha, int beta, bool max)
{
	if (depth == 0)
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

	std::vector<Move> moves = GenerateLegalMoves(position);
	if (moves.empty())
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

	MoveEval bestMove;
	if (max)
	{
		bestMove.eval = -1000000;
		for (const Move& move : moves)
		{
			Board newPosition = position;
			newPosition.MovePiece(move);

			MoveEval result = AlphaBeta(newPosition, depth - 1, alpha, beta, false);
			if (result.eval > bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			alpha = std::max(alpha, bestMove.eval);
			if (beta <= alpha)
			{
				break;
			}
		}
	}
	else
	{
		bestMove.eval = 1000000;
		for (const Move& move : moves)
		{
			Board newPosition = position;
			newPosition.MovePiece(move);

			MoveEval result = AlphaBeta(newPosition, depth - 1, alpha, beta, true);
			if (result.eval < bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			beta = std::min(beta, bestMove.eval);
			if (beta <= alpha)
			{
				break;
			}
		}
	}

	return bestMove;
}

