#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include "constants.h"

class Utils;
class Engine;

struct Board
{
private:
public:
	bool wCastlingKing = false;
	bool wCastlingQueen = false;

	bool bCastlingKing = false;
	bool bCastlingQueen = false;

	uint8_t enPassantSquare = -1;

	bool turn;	// true = white, false = black
	int turns;


	uint8_t GetWhiteKingPosition();
	uint8_t GetBlackKingPosition();

	Board(const char* fen);
	Board();
	~Board();

	void SetAllBitboards(uint8_t indexPosition);
	void SetBitboardBit(int pieceType, int indexPosition);
	void ClearBitInAllBitboards(uint8_t indexPosition);
	void ClearCurrentBitboard();
	// std::string ConvertToBoardPosition(uint8_t squareIndex);

	bool MovePiece(uint8_t piece, uint8_t position, uint8_t promotion = 255);


	void MoveWithoutComprobe(int from, int position);

	bool Promotion(uint8_t piece, uint8_t position, uint8_t promotion);

	bool CanMovePawn	(uint8_t from, uint8_t _where);
	bool CanMoveKnight	(uint8_t from, uint8_t _where);
	bool CanMoveBishop	(uint8_t from, uint8_t _where);
	bool CanMoveRook	(uint8_t from, uint8_t _where);
	bool CanMoveQueen	(uint8_t from, uint8_t _where);
	bool CanMoveKing	(uint8_t from, uint8_t _where);

	bool Castle(uint8_t from, uint8_t _where);

	bool IsSquareAttacked(uint8_t square);

	bool IsOccupied(uint8_t indexPosition);
	bool IsCheck(uint8_t indexPosition);

	Bitboard GetKingAttacks(uint8_t square);

	Bitboard bitboards[12];

	bool GetTurn() const { return turn; }
};

#endif
