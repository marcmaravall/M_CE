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
		bestMove.eval = 1000000;
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

// reduces complexity O(n^2) to O(n) by using alpha-beta pruning
MoveEval AlphaBeta(Board& position, uint8_t depth, int alpha, int beta, bool max, int ply)
{
	const std::vector<Move> moves = MVV_LVA_Order(GenerateLegalMoves(position), position);

	NODES++;
	if (depth <= 0)
	{
		//if (GenerateLegalMoves(position).empty())									why i did that?
		//{
		//	// std::cerr << "MOVES.EMPTY 0\n";
		//	if (position.IsCheck(position.turn==WHITE_TURN? WHITE : BLACK)) {
		//		std::cerr << "MATE 0\n";
		//		int mateScore = MATE_SCORE - (ply - depth);
		//		return { Move(), max ? -mateScore : mateScore };
		//	}
		//	else
		//		return { Move(), 0 };
		//}

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

	if (moves.empty())
	{
		// std::cout << "MOVES.EMPTY\n";
		if (position.IsCheck(position.turn == WHITE_TURN ? WHITE : BLACK)) {
			int mateScore = MATE_SCORE - (ply - depth);		// 1000000 - max - depth
			std::cout << "MATE: " << mateScore << " " << max << "\n";
			return { Move(), max ? -mateScore : mateScore };
		}
		else {
			std::cerr << "No moves available, returning 0 evaluation.\n";
			Utils::PrintBoard(position);
			return { Move(), 0 };
		}
	}

	const int alphaOrig = alpha;

	MoveEval bestMove;
	if (max)
	{
		bestMove.eval = -1'000'000;
		for (const Move& move : moves)
		{
			const UndoInfo undo = Utils::CreateUndoInfo(position, move);
			// Engine::undoStack.push_back(undo);

			// newPosition.MovePiece(move);
			position.MovePiece(move);

			const MoveEval result = AlphaBeta(position, depth - 1, alpha, beta, false, ply);
			if (result.eval > bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			alpha = std::max(alpha, bestMove.eval);

			position.UndoMove(undo);
			// position = buffer;

			if (beta <= alpha)
			{
				ALPHA_BETA_PRUNINGS++;
				break;
			}
		}
	}
	else
	{
		bestMove.eval = 1'000'000;
		for (const Move& move : moves)
		{
			const UndoInfo undo = Utils::CreateUndoInfo(position, move);

			// newPosition.MovePiece(move);
			if (!position.MovePieceFast(move))
			{
				static_assert("Move Piece Fast cannot move");
			}

			const MoveEval result = AlphaBeta(position, depth - 1, alpha, beta, true, ply);
			if (result.eval < bestMove.eval)
			{
				bestMove.eval = result.eval;
				bestMove.move = move;
			}
			beta = std::min(beta, bestMove.eval);

			position.UndoMove(undo);

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
	// const std::string debugFilePath = "C:\\Users\\Marc\\source\\repos\\M_CE\\debug.txt";
	// std::ofstream debugFile(debugFilePath, std::ios::app);
	// if (!debugFile.is_open()) {
	// 	throw std::runtime_error("Cannot read debug file.");
	// }

	NODES = 0;

	std::vector<Move> moves = GenerateLegalMoves(pos);
	uint64_t total = 0;

	auto start = std::chrono::high_resolution_clock::now();
	
	for (const Move& move : moves) {
		UndoInfo undo = Utils::CreateUndoInfo(pos, move);
		// Board copy = pos;
		pos.MovePiece(move);

		uint64_t nodes = Perft(pos, depth - 1);

		std::cout << Utils::ConvertToBoardPosition(move.from) << " "
			<< Utils::ConvertToBoardPosition(move.to) << ": "
			<< nodes << "\n";

		total += nodes;
		pos.UndoMove(undo);
	}


	auto end = std::chrono::high_resolution_clock::now();

	double duration = duration_cast<std::chrono::seconds>(end - start).count(); 

	std::cout << "Total nodes: " << total << " time: " << duration<< "s nps: " << std::dec << NODES / duration << "\n\n";
}

uint64_t Perft(Board& position, int depth) {
	if (depth == 0) {
		NODES++;
		return 1;
	}

	uint64_t nodes = 0;
	std::vector<Move> moves = GenerateLegalMoves(position);

	for (Move& move : moves) {
		UndoInfo undo = Utils::CreateUndoInfo(position, move);

		position.MovePiece(move);

		nodes += Perft(position, depth - 1);

		position.UndoMove(undo);
	}
	return nodes;
}
