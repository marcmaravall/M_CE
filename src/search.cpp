#include "search.h"

int NODES = 0;
int ALPHA_BETA_PRUNINGS = 0;
TTEntry TranspositionTable[TT_SIZE];

MoveEval Minimax(Board& position, uint8_t depth, bool max)
{
	NODES++;
	if (depth == 0)
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

	std::vector<Move> moves;
	GenerateLegalMoves(position, moves);
	if (moves.empty())
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

	MoveEval bestMove;
	if (max)
	{
		bestMove.eval = -MATE_SCORE;
		for (const Move& move : moves)
		{
			Board newPosition = position;
			newPosition.MovePieceFast(move);

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
		bestMove.eval = MATE_SCORE;
		for (const Move& move : moves)
		{
			Board newPosition = position;
			newPosition.MovePieceFast(move);

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

#define COPY_MODE

// reduces complexity O(n^2) to O(n) by using alpha-beta pruning
MoveEval AlphaBeta(Board& position, uint8_t depth, int alpha, int beta, bool max, int ply)
{
	std::vector<Move> moves;
	GeneratePseudoLegalMoves(position, moves);
	MVV_LVA_Order(moves, position);

	NODES++;

	// TT implementation
	ZobristHash zobristHash = Utils::GetZobristHash(position, Engine::hashSettings);
	int index = zobristHash & (TT_SIZE - 1);
	TTEntry& entry = TranspositionTable[index];

	if (entry.hash == zobristHash && entry.depth >= depth) {
		if (entry.bound == EXACT)
			return { entry.bestMove };
		if (entry.bound == LOWERBOUND && entry.bestMove.eval >= beta)
			return { entry.bestMove };
		if (entry.bound == UPPERBOUND && entry.bestMove.eval <= alpha)
			return { entry.bestMove };
	}


	if (depth <= 0)
	{
		// TODO: implement quiescence search
		return { Move(), Evaluation::Evaluate(position) };
	}

	uint8_t movesGenerated = 0;

	const int alphaOrig = alpha;

	MoveEval bestMove;
	if (max)
	{
		bestMove.eval = -MATE_SCORE;
		for (const Move& move : moves)
		{
#ifdef COPY_MODE
			Board newPosition = position;
			if (!newPosition.MovePieceFast(move))
				continue;
			movesGenerated++;
			const MoveEval result = AlphaBeta(newPosition, depth - 1, alpha, beta, false, ply);
#else
			const UndoInfo undo = Utils::CreateUndoInfo(position, move);
			position.MovePieceFast(move);
			const MoveEval result = AlphaBeta(position, depth - 1, alpha, beta, false, ply);
#endif

			if (result.eval > bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			alpha = std::max(alpha, bestMove.eval);
#ifndef COPY_MODE
			position.UndoMove(undo);
#endif

			if (beta <= alpha)
			{
				ALPHA_BETA_PRUNINGS++;
				break;
			}
		}
	}
	else
	{
		bestMove.eval = MATE_SCORE;
		for (const Move& move : moves)
		{
#ifdef COPY_MODE
			Board newPosition = position;
			if (!newPosition.MovePieceFast(move))
				continue;
			movesGenerated++;
			const MoveEval result = AlphaBeta(newPosition, depth - 1, alpha, beta, true, ply);
#else
			const UndoInfo undo = Utils::CreateUndoInfo(position, move);
			if (!position.MovePieceFast(move)) {
				continue;
			}
			const MoveEval result = AlphaBeta(position, depth - 1, alpha, beta, true, ply);
			position.UndoMove(undo);
#endif

			if (result.eval < bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			beta = std::min(beta, bestMove.eval);

			if (beta <= alpha)
			{
				ALPHA_BETA_PRUNINGS++;
				break;
			}
		}
	}
	
	if (movesGenerated == 0)
	{
		if (position.IsCheck(position.turn == WHITE_TURN ? WHITE : BLACK)) {
			int mateScore = MATE_SCORE - (ply - depth);		// mate - max - depth
			std::cout << "MATE: " << mateScore << "\n";
			return { Move(), max ? -mateScore : mateScore };
		}
		else {
			return { Move(), 0 };
		}
	}

	// save stte into tt
	BoundType bound;
	if (bestMove.eval <= alphaOrig) bound = UPPERBOUND;
	else if (bestMove.eval >= beta) bound = LOWERBOUND;
	else bound = EXACT;

	TranspositionTable[index] = {
		.hash = zobristHash,
		.depth = depth,
		.bestMove = bestMove,
		.bound = bound
	};

	return bestMove;
}

void Divide(Board& pos, int depth) {
	// const std::string debugFilePath = "C:\\Users\\Marc\\source\\repos\\M_CE\\debug.txt";
	// std::ofstream debugFile(debugFilePath, std::ios::app);
	// if (!debugFile.is_open()) {
	// 	throw std::runtime_error("Cannot read debug file.");
	// }

	NODES = 0;

	std::vector<Move> moves;
	GenerateLegalMoves(pos, moves);
	uint64_t total = 0;

	auto start = std::chrono::high_resolution_clock::now();
	
	for (const Move& move : moves) {
#ifdef COPY_MODE
		Board newPosition = pos;
		if (!newPosition.MovePieceFast(move))
			continue;

		// if (move.to == 0)
		// 	std::cout << "ERROR: detected an error.\n";			awesome code .__.		...

		uint64_t nodes = Perft(newPosition, depth - 1);
#else
		UndoInfo undo = Utils::CreateUndoInfo(pos, move);
		pos.MovePieceFast(move);
		uint64_t nodes = Perft(pos, depth - 1);
		pos.UndoMove(undo);
#endif

		std::cout << Utils::MoveToStr(move) << ": "
			<< nodes << "\n";

		total += nodes;
	}


	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float, std::milli> duration = (end - start)/1000;

	std::cout << "Nodes Searched: " << total << std::fixed << std::setprecision(4) << " time: " << duration.count() << "s "<< std::setprecision(0) << "nps: " << std::dec << total / duration.count() << "\n\n";
}

uint64_t Perft(Board& position, int depth) {
	if (depth == 0) {
		NODES++;
		return 1;
	}

	uint64_t nodes = 0;
	std::vector<Move> moves;
	GenerateLegalMoves(position, moves);

	for (Move& move : moves) {
#ifdef COPY_MODE
		Board newPosition = position;
		if (!newPosition.MovePieceFast(move)) {
			continue;
		}
		nodes += Perft(newPosition, depth - 1);

#else
		UndoInfo undo = Utils::CreateUndoInfo(position, move);
		position.MovePieceFast(move);
		nodes += Perft(position, depth - 1);
		position.UndoMove(undo);
#endif
	}
	return nodes;
}
