#include "uci.h"
#include "engine.h"

void UCI::uci()
{
	std::cout << "id name " << m_uciName << "\n";
	std::cout << "id author " << m_uciAuthor << "\n";
	std::cout << "uciok\n";
}

void UCI::isready()
{
	std::cout << "readyok\n";
}

void UCI::quit()
{
	std::cout << "bye\n";
	exit(0);
}

void UCI::ucinewgame()
{

}

