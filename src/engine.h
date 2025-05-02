#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include "board.h"
#include "constants.h"

class Engine
{
private:
	Board currentBoard;

public:
	Engine();
	~Engine();
	

	void init();
	void start();
	void stop();
	void reset();

	void RunTest();
};

#endif