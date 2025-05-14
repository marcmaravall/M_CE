#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include "constants.h"
#include "move.h"

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

	bool MovePiece(const Move move);


	void MoveWithoutComprobe(int from, int position);

	bool Promotion(const Move move);

	bool CanMovePawn	(const Move move);
	bool CanMoveKnight	(const Move move);
	bool CanMoveBishop	(const Move move);
	bool CanMoveRook	(const Move move);
	bool CanMoveQueen	(const Move move);
	bool CanMoveKing	(const Move move);

	void Castle(const Move move);
	bool CanCastle(const Move move);
	void DoCastleMove(const Move move);

	bool IsSquareAttacked(uint8_t square);

	bool IsOccupied(uint8_t indexPosition);
	bool IsCheck(uint8_t indexPosition);

	Bitboard GetKingAttacks(uint8_t square);

	Bitboard bitboards[12];

	bool GetTurn() const { return turn; }
};

#endif
