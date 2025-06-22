#define M_CE_UCI
#include "engine.h"
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <functional>

class Engine;
class UCI
{
private:
	const char* m_uciName = "M_CE";
	const char* m_uciAuthor = "Marc Maravall Díez";

	Engine engine;

public:
	UCI();

	void SendInfo();

	void uci();
	void isready();
	void setoption(const char* name, const char* value);
	void register_uci();
	void ucinewgame();
	void position(const char* fen);
	// void go();
	void stop();
	void ponderhit();
	void quit();
	void perft(int depth);
	void draw();

	void ManageInput(const char* input);

	// static bool IsSearching;

	bool sendInfo = true;
};