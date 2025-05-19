#pragma once

#include <iostream>
#include "board.h"
#include "constants.h"
#include "utils.h"

class Evaluation
{
	struct EvalSetting {
		enum EvalMode {
			SIMPLE, NORMAL
		};

		EvalMode mode;
	};


public:
	static int SimpleEvaluation(const Board board);
	static int Evaluate(const Board& board);
};