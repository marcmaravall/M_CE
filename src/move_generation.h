#include "board.h"
#include "evaluation.h"
#include "utils.h"
#include "move.h"
#include <iostream>
#include <vector>
#include <cstdint>

std::vector<Move> GeneratePawnMoves(Board board, uint8_t from);
std::vector<Move> GenerateKnightMoves(Board board, uint8_t from);
std::vector<Move> GenerateBishopMoves(Board board, uint8_t from);
std::vector<Move> GenerateRookMoves(Board board, uint8_t from);
std::vector<Move> GenerateQueenMoves(Board board, uint8_t from);
std::vector<Move> GenerateKingMoves(Board board, uint8_t from);
std::vector<Move> GenerateLegalMoves(Board board);
std::vector<Move> GeneratePseudoLegalMoves(Board board);