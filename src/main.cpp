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
15/05/2025: Implemented pseudo legal moves generation, minimax implemented
*/


#include <iostream>
#include "engine.h"

int main()
{
	Engine engine;
	engine.PlayAgainistHuman();
	return 0;
}