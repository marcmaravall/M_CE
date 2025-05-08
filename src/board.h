#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <string>
#include "constants.h"

struct Board
{
private:
	Bitboard bitboards[12];
	bool wCastlingKing = false;
	bool wCastlingQueen = false;

	bool bCastlingKing = false;
	bool bCastlingQueen = false;

	uint8_t enPassantSquare = -1;

	bool turn;	// true = white, false = black
	int turns;

	std::string ToUpper(std::string str);
public:
	Board(const char* fen);
	Board();
	~Board();

	void PrintBoard();
	void PrintBitboards();
	std::string ConvertToFEN();

	void SetAllBitboards(uint8_t indexPosition);
	void SetBitboardBit(int pieceType, int indexPosition);
	void ClearBitInAllBitboards(uint8_t indexPosition);
	void ClearCurrentBitboard();
	std::string ConvertToBoardPosition(uint8_t squareIndex);

	bool MovePiece(uint8_t piece, uint8_t position, uint8_t promotion = 255);
	Bitboard GetAllBitboards(Bitboard b[12], PIECE_COLORS color = WHITE);
	int ConvertToIndexPosition(std::string squarePosition);


	void MoveWithoutComprobe(int from, int position);

	bool Promotion(uint8_t piece, uint8_t position, uint8_t promotion);

	bool CanMovePawn	(uint8_t from, uint8_t _where);
	bool CanMoveKnight	(uint8_t from, uint8_t _where);
	bool CanMoveBishop	(uint8_t from, uint8_t _where);
	bool CanMoveRook	(uint8_t from, uint8_t _where);
	bool CanMoveQueen	(uint8_t from, uint8_t _where);
	bool CanMoveKing	(uint8_t from, uint8_t _where);

	bool GetBitboardValueOnIndex(Bitboard bitboard, uint8_t index);

	bool IsWhitePieceAt(uint8_t index);
	bool IsBlackPieceAt(uint8_t index);
};

#endif
