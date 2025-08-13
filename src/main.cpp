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
24/06/2025: Searching why the engine is so bad, I think that is a problem with alphabeta implementation.					--.
25/06/2025: Optimized the engine to up 100k nps in depth 5.																	  |
26/06/2025: Searching why the engine is so bad with the optimizations that I've made.										  |
27/06/2025: Added some uci options support.																					  | - Vacation
28/06/2025: Nothing...																										  |
29/06/2025: Nothing...																										  | 
30/06/2025: Nothing...																										  |
01/07/2025: Nothing...																										--.
02/07/2025: I've solved a problem, now in perft 5 it works at 5m nps, but it hash problems with the move generation.
03/07/2025: Solved a bug with the debug bitboard function.
04/07/2025: Started to implement some tools for debugging.
05/07/2025: Added perft_checker, to compare the results of the engine with the results of other engines.
06/07/2025: Nothing, I was programming another project.
07/07/2025: Solved two bugs with move generation, now perft 5 works fine.
08/07/2025: Solved a little thing with MovePieceFast.
09/07/2025: Nothing...
10/07/2025: Started to implement magic bitboards.
11/07/2025: Started to implement magic numbers generation.
12/07/2025: Nothing...
13/07/2025: Implemented GenerateOccupancy.
14/07/2025: Implemented debug features, added ray generation for magic bitboards.
15/07/2025: Nothing...
16/07/2025: Solved a problem with ray generation for magic bitboards.
17/07/2025: Solved bishop tables generation.
18/07/2025: Solved another bug with bishop tables generation (it wasn't working). Added things for debug.
19/07/2025: Added an ascii board evaluation, to see what adds and what removes evaluation points.
20/07/2025: Tried to see if magic bitboards are working, it don't works...
21/07/2025: Added precalculed magics and tried to solve some errors, it don't works...
22/07/2025: Detected a silly bug in magics, now it's solved. Magic bitabords started to work at perft 3 and 4.
23/07/2025: Solved a bug with bishop magic generation perft 5 works. Magic bitboards implemented.
24/07/2025: Nothing...																							-----.
25/07/2025: Nothing...																								 |
26/07/2025: Nothing...																								 |
27/07/2025: Nothing...																								 | ---- Vacation
28/07/2025: Nothing...																								 |
29/07/2025: Nothing...																								 |
30/07/2025: Nothing...																								 |
31/07/2025: Nothing...																							-----.
01/08/2025: Started to search some optimizations for the engine with vs profiler.
02/08/2025: Started to optimize pawn move generation, basic pawn moves work well.
03/08/2025: Added en passant to the optimized move generation function, added COPY_MODE to search and perft to compare results.
04/08/2025: Added promotions to pawn generation optimized, and solved some bugs. Added more COPY_MODE code. Nps in perft 5 are over 5.5m.
05/08/2025: Added an optimization in move generation, it generates pseudolegal moves and then in search is when you comprobe if the move is legal or not. 
			The engine is faster, 800k - 1.2M nps in mid game, and 2.5M - 3.5M nps on some endgames/puzzles. Added search with time to the uci with go wtime... and movetime.
06/08/2025: Optimized the mvv_lva move ordering, the engine search much less nodes.
07/08/2025: Added movedPiece to the Move struct to optimize calculations in mvv_lva and other places. Started to implement quiescene search.
08/08/2025: Implemented quiescene search, now the engine is more stable at some positions, but it's very poor optimized.
09/08/2025: Implemented new pawn evaluation, king endgame evaluation, and the evaluation of the bishop pair.
10/08/2025: Nothing...
11/08/2025: Solved a bug in evaluation, started to implement a function to visualize evaluationV2.
12/08/2025: Implemented more of the visualize evaluationV2 function, there is a bug in the evaluate pawn function.
*/

#include <iostream>
#include "engine.h"
#include "uci.h"

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	UCI uci;
	
	std::cout	<< "M_CE v" << uci.GetVersion() 
				<< " compiled in " << uci.GetOsCompiled() 
				<< " by compiler " << uci.GetCompiler() << ". Author: Marc Maravall Diez.\n";

	while (true)
	{
		std::string input;
		std::getline(std::cin, input);
		uci.ManageInput(input.c_str());
	}

	return 0;
}
