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
	NODES++;
	if (depth <= 0)
	{
		return { Move(), Evaluation::Evaluate(position) };
	}

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


	std::vector<Move> moves = MVV_LVA_Order(GenerateLegalMoves(position), position, depth);
	if (moves.empty())
	{
		return { Move(), Evaluation::Evaluate(position) };
	}


	int alphaOrig = alpha;

	MoveEval bestMove;
	if (max)
	{
		bestMove.eval = -1000000;
		for (const Move& move : moves)
		{
			UndoInfo undo = Utils::CreateUndoInfo(position, move);
			Engine::undoStack.push_back(undo);
			
			// newPosition.MovePiece(move);
			position.MovePiece(move);

			MoveEval result = AlphaBeta(position, depth - 1, alpha, beta, false);
			if (result.eval > bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			alpha = std::max(alpha, bestMove.eval);
			
			position.UndoMove(undo);
			Engine::undoStack.pop_back();

			if (beta <= alpha)
			{
				ALPHA_BETA_PRUNINGS++;
				break;
			}
		}
	}
	else
	{
		bestMove.eval = 1000000;
		for (const Move& move : moves)
		{
			UndoInfo undo = Utils::CreateUndoInfo(position, move);
			Engine::undoStack.push_back(undo);

			// newPosition.MovePiece(move);
			position.MovePiece(move);

			MoveEval result = AlphaBeta(position, depth - 1, alpha, beta, true);
			if (result.eval < bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			beta = std::min(beta, bestMove.eval);

			position.UndoMove(undo);
			Engine::undoStack.pop_back();

			if (beta <= alpha)
			{
				ALPHA_BETA_PRUNINGS++;
				break;
			}
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
	const std::string& debugFilePath = "C:\\Users\\Marc\\source\\repos\\M_CE\\tests\\debug.txt";
	std::ofstream debugFile(debugFilePath, std::ios::app);
	if (!debugFile.is_open()) {
		throw std::runtime_error("error");
	}

	auto moves = GenerateLegalMoves(pos);
	uint64_t total = 0;
	for (const Move& move : moves) {
		UndoInfo undo = Utils::CreateUndoInfo(pos, move);
		pos.MovePiece(move);

		uint64_t nodes = Perft(pos, depth - 1);

		pos.UndoMove(undo);

		debugFile << Utils::ConvertToFEN(pos) << "		"
			<< Utils::ConvertToBoardPosition(move.from) << " "
			<< Utils::ConvertToBoardPosition(move.to) << ": "
			<< nodes << "\n";
		total += nodes;
	}
	debugFile << "Total nodes: " << total << "\n \n";
	debugFile.close();
}

uint64_t Perft(Board& position, int depth) {
	if (depth == 0) return 1;

	uint64_t nodes = 0;
	std::vector<Move> moves = GenerateLegalMoves(position);

	for (const Move& move : moves) {
		UndoInfo undo = Utils::CreateUndoInfo(position, move);
		position.MovePiece(move);

		if (depth == 1)
			Divide(position, depth);

		nodes += Perft(position, depth - 1);

		position.UndoMove(undo);
	}

	return nodes;
}


