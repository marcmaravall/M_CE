#include "board.h"
#include "constants.h"
#include "evaluation.h"
#include "utils.h"
#include "move.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

std::vector<Move> GeneratePawnMoves			(const Board& board, const uint8_t from);
std::vector<Move> GenerateKnightMoves		(const Board& board, const uint8_t from);
std::vector<Move> GenerateBishopMoves		(const Board& board, const uint8_t from);
std::vector<Move> GenerateRookMoves			(const Board& board, const uint8_t from);
std::vector<Move> GenerateQueenMoves		(const Board& board, const uint8_t from);
std::vector<Move> GenerateKingMoves			(const Board& board, const uint8_t from);
std::vector<Move> GenerateCastlingMoves		(const Board& board, const uint8_t from);
std::vector<Move> GenerateLegalMoves		(Board& board);
std::vector<Move> GeneratePseudoLegalMoves	(const Board& board);
std::vector<Move> MVV_LVA_Order				(const std::vector<Move>& moves, const Board& board, uint8_t depth);