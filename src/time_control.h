#ifndef TIME_CONTROL_H
#define TIME_CONTROL_H

#include <iostream>
#include <assert.h>
#include <vector>
#include "constants.h"

#define TMT_SIZE 8
#define TME(x, y) TMEntry { x, y },

class TimeManager
{
public:
	const uint16_t midMovesInGame = 60;

public:
	typedef struct TMEntry {
		uint32_t value = 0;
		uint32_t time  = 0;
	};

public:
	TimeManager();

	// This table contains all the data to return how many time to search, for example, an entry who has a
	// (1000, 30000) indicates that if your time is under 30000ms (30s) the max time to search is 1000ms (1s).
	TMEntry entries[TMT_SIZE] = {
		TME(500  , 3000   )
		TME(1000 , 10000  )
		TME(2000 , 30000  )
		TME(2500 , 60000  )
		TME(5000 , 300000 )
		TME(6000 , 600000 )
		TME(9500 , 900000 )
		TME(12000, 1800000)
	};

	const float waitingOffset = 10;

public:
	typedef enum {
		ULTRABULLET,
		BULLET, 
		BLITZ,
		RAPID,
		CLASSICAL,
	} Mode;				// i don't think i'm going to use this for caluclating time.

	uint32_t CalculateTime(const uint32_t wTime = 1000, const uint32_t bTime = 1000, const uint32_t wInc = 0, const uint32_t bInc = 0, const bool engineColor = WHITE_TURN);
	uint32_t CalculateTimeBasic(const uint32_t wTime = 1000, const uint32_t bTime = 1000, const uint32_t wInc = 0, const uint32_t bInc = 0, const bool engineColor = WHITE_TURN);

private:
	uint32_t wIncrement = 0;
	uint32_t bIncrement = 0;

	uint32_t wTime = 0;
	uint32_t bTime = 0;
};


#endif
