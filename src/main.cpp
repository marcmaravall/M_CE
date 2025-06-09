/*
05/05/2025: FEN loading and conversion completed
06/05/2025: Conversion to index and conversion to string completed
07/05/2025: Index changed to a1:0 a2:1 ... h8:63	Completed pawn movements up and double at start
08/05/2025: En passant and captures with pawns, promotion completed
			ALL PAWN MOVEMENTS COMPLETED
09/05/2025: Refactored Board class, added utils.h	Solved a bug with linking
10/05/2025: Added masks
11/05/2025: Added bishop, rook, queen and king movements, checks comprobed
12/05/2025: Nothing because I'm so lazy
13/05/2025: Implemented castling (a litle)
14/05/2025: Implemented castling, simple evaluation function based on bitmaps, started to implement pseudo legal moves generation
15/05/2025: Implemented pseudo legal moves generation, minimax with alphabeta implemented, max depth is 5
16/05/2025: Optimized search and move generation (a bit), avg(nps) is more than 200k, implemented move ordering, max depth is 7
17/05/2025: Transposition table implemented, search at depth 6 more accesible
18/05/2025: Started to optimize code.
19/05/2025: Nothing.
20/05/2025: Added bitboards and masks to move generation, not very usefull...
21/05/2025: Solved some bugs with move generation.
22/05/2025: Solved king checks errors and added Perft and Divide for debug.
23/05/2025: Searching why don't works perft tests, and solved a bug.
24/05/2025: Nothing.
25/05/2025: Solved bugs of pawns and masks, PERFT(3) works perftectly, but perft(4) is very bad. 
26/05/2025: Triying to solve a bug with UndoMove, it's not corrected.
27/05/2025: Added debugging to some parts, it's more easy to find what is the problem.
28/05/2025: Solved a common bug with masks into GenerateRayAtacks, now some tests are good.
29/05/2025: Solved a bug with black pawns captures, perft(4) is working better.
30/05/2025: Solved a bug with white pawn en passant, perft(4) is perftectly working, perft(5) is not working, added some uci commands (with bugs...).
31/05/2025: UCI protocol is working, with bugs, but working.
01/06/2025: Solved promotion bugs and added castling to UCI, solving some problems with checkmate evaluation.
02/06/2025: Solved a problem with checkmate evaluation.
03/06/2025: Tested the engine with other engines and humans.
04/06/2025: Tested the engine playing against it.
05/06/2025: Started to implement polyglot books.
06/06/2025: Added book class and .bin loading (it doesn't work very well).
07/06/2025: 
*/


#include <iostream>
#include "engine.h"
#include "uci.h"

int main()
{
	/*UCI uci;
	while (true)
	{
		std::string input;
		std::getline(std::cin, input);
		uci.ManageInput(input.c_str());
	}*/

	Engine engine;
	engine.RunBookTest();
	// engine.PlayAgainistItself();
	return 0;
}

