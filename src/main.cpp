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
07/06/2025: Tried to solve a bug with book loading, but I don't know how to solve it.
08/06/2025: I solved a bug, but is not working for other bug...
09/06/2025: I solved the bug, it was a problem with the index of the book, now is working.
10/06/2025: Added weight for polyglot books for doing multiple moves.
11/06/2025: Added functions for getting the path of the program.
12/06/2025: Nothing.
13/06/2025: Nothing.
14/06/2025: Added some things to README.md.
15/06/2025: Created a discord server for the engine, you can join here: https://discord.gg/GjPHXuyzmD.
16/06/2025: Played some games.
17/06/2025: Nothing.
18/06/2025: Added half moves counter and move counter for board.
19/06/2025: Added 50 moves rule and 3 repetitions rule.
20/06/2025: Proving the engine in Ubuntu, it works fine, but it needs some changes.
21/06/2025: Added changes for linux file system, now I can load books.
22/06/2025: Solved a minor bug with en passant, now perft 5 works perfectly.
23/06/2025: Optimizing search and move generation, now the engine is x2 faster, but is very bad.
24/06/2025: Searching why the engine is so bad, I think that is a problem with alphabeta implementation.
25/06/2025: Optimized the engine to up 100k nps in depth 5.
26/06/2025: Searching why the engine is so bad with the optimizations that I've made.
27/06/2025: Added some uci options support.												--.
28/06/2025: Nothing...																	  |
29/06/2025: Nothing...																	  | - Relaxing
30/06/2025: Nothing...																	  |
01/06/2025: Nothing...																	--.
*/


#include <iostream>
#include "engine.h"
#include "uci.h"

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	UCI uci;

	// uci.NPSTest();

	while (true)
	{
		std::string input;
		std::getline(std::cin, input);
		uci.ManageInput(input.c_str());
	}

	/* Engine engine;
	engine.RunBookTest();*/
	// engine.PlayAgainistItself();
	return 0;
}


