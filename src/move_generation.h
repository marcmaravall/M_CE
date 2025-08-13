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
void GenerateCastlingMoves		(Board board, const uint8_t from, std::vector<Move>& moves);
void GenerateLegalMoves			(Board& board, std::vector<Move>& pseudoLegalMoves);
void GeneratePseudoLegalMoves	(const Board& board, std::vector<Move>& moves);

void MVV_LVA_Order(std::vector<Move>& moves, const Board& board);
void SeparateByCaptures(const std::vector<Move>& moves, std::vector<Move>& captures);
void GenerateCaptures(const Board& board, const uint8_t from, std::vector<Move>& moves);
