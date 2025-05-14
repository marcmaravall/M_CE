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

	int SimpleEvaluation(Board board);

public:
	int Evaluate(Board board);
};