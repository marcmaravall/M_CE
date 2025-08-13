#pragma once

#include <iostream>
#include "board.h"
#include "constants.h"
#include "utils.h"
#include <iomanip>

class Evaluation
{
	struct EvalSetting {
		enum EvalMode {
			SIMPLE, NORMAL
		};

		EvalMode mode;
	};


public:
	static int SimpleEvaluation(const Board& board);
	static int Evaluate(const Board& board);

	// new 
	static int EvaluateV2(const Board& board);
	static int EvaluatePawnStruct(const Board& board, const GAME_PHASE phase);
	static int EvaluateKingsSafety(const Board& board, const GAME_PHASE	phase);
	static int EvaluateBishopPair(const Board& board, const GAME_PHASE phase);
	static GAME_PHASE GetGamePhase(const Board& board);
	static int EvaluatePawn(const uint8_t index, const Board& board, const GAME_PHASE phase);

	static int EvaluatePiece(const Board& board, const uint8_t index);
	static int EvaluatePieceSimple(const Board&, const uint8_t index);
	static int EvaluatePieceV2(const Board&, const uint8_t index);

	static void PrintEvaluation(const Board& board);
	static void PrintSimpleEvaluation(const Board& board);
	static void PrintEvaluationV2(const Board& board);
};

