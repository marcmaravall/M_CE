#include "board.h"


Board::Board(const char* fen)
{
	std::string fenString(fen);
	size_t index = 0;
	for (int rank = 7; rank >= 0; rank--)
	{
		for (int file = 0; file < 8; file++)
		{
			if (index >= fenString.size())
				break;

			char c = fenString[index++];
			if (isdigit(c))
			{
				int emptySquares = c - '0';
				for (int i = 0; i < emptySquares; i++)
				{
					if (index < fenString.size()) index++;
				}
			}
			else if (c == '/')
			{
				continue;
			}
			else
			{
				for (size_t pieceIndex = 0; pieceIndex < 12; pieceIndex++)
				{
					if (c == PIECE_CHAR[pieceIndex])
					{
						bitboards[pieceIndex] |= (1ULL << ((rank * 8) + file));
						break;
					}
				}
			}
		}
	}
	
	if (index < fenString.size() && fenString[index] == ' ')
	{
		index++;
	}

	if (index < fenString.size() && fenString[index] == 'w')
	{
		turn = true;
	}
	else if (index < fenString.size() && fenString[index] == 'b')
	{
		turn = false;
	}

	index += 2;

	if (index < fenString.size() && fenString[index] != '-')
	{
		while (fenString[index] != ' ' && index < fenString.size())
		{
			index++;
		}
	}

	enPassantSquare = -1;

	if (index < fenString.size() && fenString[index] != ' ' && fenString[index] != '-')
	{
		enPassantSquare = -1;
	}
}



Board::Board()
{
}

Board::~Board()
{
}

void Board::PrintBoard()
{
	for (int rank = 7; rank >= 0; rank--)
	{
		std::cout << rank + 1 << " "; // Números de fila (8 a 1)
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			bool found = false;

			for (size_t pieceIndex = 0; pieceIndex < 12; pieceIndex++)
			{
				if (bitboards[pieceIndex] & (1ULL << square))
				{
					std::cout << PIECE_CHAR[pieceIndex] << " ";
					found = true;
					break;
				}
			}

			if (!found)
			{
				std::cout << ". ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "  a b c d e f g h" << std::endl; // Letras de columna
}


std::string Board::ConvertToFEN()
{
	std::string fen = "";

	for (int rank = 7; rank >= 0; rank--)
	{
		int emptyCount = 0;
		for (int file = 0; file < 8; file++)
		{
			bool pieceFound = false;
			int square = rank * 8 + file;

			for (int piece = 0; piece < 12; piece++)
			{
				if (bitboards[piece] & (1ULL << square))
				{
					if (emptyCount > 0)
					{
						fen += std::to_string(emptyCount);
						emptyCount = 0;
					}
					fen += PIECE_CHAR[piece];
					pieceFound = true;
					break;
				}
			}

			if (!pieceFound)
			{
				emptyCount++;
			}
		}
		if (emptyCount > 0)
			fen += std::to_string(emptyCount);
		if (rank != 0)
			fen += '/';
	}

	fen += ' ';
	fen += (turn ? 'w' : 'b');

	std::string castling = "";
	if (!wKingMoved && !wRookMoved[1]) castling += 'K';
	if (!wKingMoved && !wRookMoved[0]) castling += 'Q';
	if (!bKingMoved && !bRookMoved[1]) castling += 'k';
	if (!bKingMoved && !bRookMoved[0]) castling += 'q';
	if (castling.empty()) castling = "-";
	fen += ' ' + castling;

	fen += ' ';
	if (enPassantSquare != -1)
	{
		int file = enPassantSquare % 8;
		int rank = enPassantSquare / 8;
		fen += static_cast<char>('a' + file);
		fen += static_cast<char>('1' + rank);
	}
	else
	{
		fen += '-';
	}

	fen += " 0 1";

	return fen;
}
