#pragma once

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
	void MovePawn(const Move move);

public:
	bool wCastlingKing = false;
	bool wCastlingQueen = false;

	bool bCastlingKing = false;
	bool bCastlingQueen = false;

	uint8_t enPassantSquare = -1;

	bool turn;	// true = white, false = black

	int halfMoves = 0;
	int turns = 0;

	uint8_t GetWhiteKingPosition(const bool debug = false);
	uint8_t GetBlackKingPosition(const bool debug = false);

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

	bool CanMovePawn	(const Move move) const;
	bool CanMoveKnight	(const Move move) const;
	bool CanMoveBishop	(const Move move) const;
	bool CanMoveRook	(const Move move) const;
	bool CanMoveQueen	(const Move move) const;
	bool CanMoveKing	(const Move move) const;

	bool CanMovePawn	(const Move move, const bool _turn) const;
	bool CanMoveKnight	(const Move move, const bool _turn) const;
	bool CanMoveBishop	(const Move move, const bool _turn) const;
	bool CanMoveRook	(const Move move, const bool _turn) const;
	bool CanMoveQueen	(const Move move, const bool _turn) const;
	bool CanMoveKing	(const Move move, const bool _turn) const;

	void Castle(const Move move);
	bool CanCastle(const Move move);
	void DoCastleMove(const Move move);

	bool IsSquareAttacked(const PIECE_COLORS attackerColor, const int square, const bool debug = false);

	bool IsOccupied(uint8_t indexPosition) const;
	bool IsCheck(const PIECE_COLORS color, const bool debug = false);

	void UndoMove(const UndoInfo& undo);

	Bitboard GetKingAttacks(uint8_t square);

	Bitboard bitboards[12];

	bool GetTurn() const { return turn; }
};

#endif
