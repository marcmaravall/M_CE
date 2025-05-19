#pragma once

#include <iostream>
#include "constants.h"
#include "engine.h"

struct Board;

class Utils
{
public:

	static void PrintBoard(const Board& board);

	static void PrintBitboards(const Board& board);

	static std::string ConvertToFEN(const Board& board);

	static std::string ConvertToBoardPosition(uint8_t squareIndex);

	static int ConvertToIndexPosition(std::string& squarePosition);

	static Bitboard GetFileMask(uint8_t file);

	static bool GetBitboardValueOnIndex(Bitboard bitboard, uint8_t index);

	static bool IsWhitePieceAt(const Board& board, uint8_t index);
	static bool IsBlackPieceAt(const Board& board, uint8_t index);

	static Bitboard GetAllBitboards(const Bitboard b[12], PIECE_COLORS color = WHITE);

	static std::string ToUpper(std::string str);

	static Bitboard GetRankMask(uint8_t rank);
	static Bitboard GetDiagonalMask(uint8_t index);
	static uint8_t GetPieceType(const Board& board, uint8_t index);

	static ZobristHash GetZobristHash(const Board& board, const ZobristHashSettings& settings);

	static int PopLSB(uint64_t& bitboard);

	static bool IsEnemyPieceAt(const Board& board, uint8_t position);
};
