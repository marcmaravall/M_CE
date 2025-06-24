#include "board.h"
#include "constants.h"
#include "evaluation.h"
#include "utils.h"
#include "move.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

void GeneratePawnMoves			(const Board& board, const uint8_t from, std::vector<Move>& moves);
void GenerateKnightMoves		(const Board& board, const uint8_t from, std::vector<Move>& moves);
void GenerateBishopMoves		(const Board& board, const uint8_t from, std::vector<Move>& moves);
void GenerateRookMoves			(const Board& board, const uint8_t from, std::vector<Move>& moves);
void GenerateQueenMoves			(const Board& board, const uint8_t from, std::vector<Move>& moves);
void GenerateKingMoves			(const Board& board, const uint8_t from, std::vector<Move>& moves);
void GenerateCastlingMoves		(const Board& board, const uint8_t from, std::vector<Move>& moves);
std::vector<Move> GenerateLegalMoves			(Board& board);
void GeneratePseudoLegalMoves	(const Board& board, std::vector<Move>& moves);
std::vector<Move> MVV_LVA_Order				(const std::vector<Move>& moves, const Board& board);

