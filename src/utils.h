#pragma once

#include <iostream>
#include <array>
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

	static int ConvertToIndexPosition(std::string squarePosition);

	static Bitboard GetFileMask(uint8_t file);

	static bool GetBitboardValueOnIndex(const Bitboard bitboard, const uint8_t index);

	static bool IsWhitePieceAt(const Board& board, uint8_t index);
	static bool IsBlackPieceAt(const Board& board, uint8_t index);

	static Bitboard GetAllBitboards(const Bitboard b[12], PIECE_COLORS color = WHITE);

	static std::string ToUpper(std::string str);

	static Bitboard GetRankMask(uint8_t rank);
	static Bitboard GetDiagonalMask(uint8_t index);
	static uint8_t GetPieceType(const Board& board, const uint8_t index);

	static ZobristHash GetZobristHash(const Board& board, const ZobristHashSettings& settings);

	static int PopLSB(uint64_t& bitboard);

	static bool IsEnemyPieceAt(const Board& board, uint8_t position);

	static Bitboard RayAttacks(const uint8_t from, const int dir, const Bitboard occupancy);
	static Bitboard RayAttacksForMagics(const uint8_t from, const int dir, const Bitboard occupancy);

	static Bitboard GenerateBishopAttacks(const int square, const Bitboard occupancy);
	static Bitboard GenerateBishopAttacksForMagics(const int square, const Bitboard occupancy);

	static Bitboard GenerateRookAttacks(const int square, const Bitboard occupancy);
	static Bitboard GenerateRookAttacksForMagics(const int square, const Bitboard occupancy);

	static UndoInfo CreateUndoInfo(const Board& board, const Move& move);

	static void DebugMove(const Move& move);

	static void DebugBitboard(Bitboard bitboard);

	static std::string ToLower(std::string str);
	char ToLower(const char str);

	static uint8_t GetPromotionPiece(char letter, bool isWhite);

	static std::string MoveToStr(const Move& move);

	static uint8_t BitScanForward(const uint64_t bb);

	static int CountBits(Bitboard bb);
	static Bitboard SetOccupancy(const int index, const int bitsInMask, const Bitboard mask);

	static bool HasRepeated(const uint64_t arr[4092]);
	static Bitboard GenerateOccupancy(const Bitboard mask, const int index);

	static uint64_t Rand64();

	static std::string ToBin(const uint64_t n);
};
