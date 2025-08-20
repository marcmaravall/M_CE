#include "time_control.h"

uint32_t TimeManager::CalculateTime(const uint32_t wTime, const uint32_t bTime, const uint32_t wInc, const uint32_t bInc , const bool engineColor)
{
	this->wTime = wTime;
	this->bTime = bTime;
	this->wIncrement = wInc;
	this->bIncrement = bInc;

	const uint32_t engineTime = engineColor == WHITE_TURN ? wTime : bTime;
	const uint32_t engineInc  = engineColor == WHITE_TURN ? wInc  : bInc;

	uint32_t timeToCalc = wTime/20 + engineInc/2;							// this is very basic stuff

	return timeToCalc;
}

uint32_t TimeManager::CalculateTimeBasic(const uint32_t wTime, const uint32_t bTime, const uint32_t wInc, const uint32_t bInc, const bool engineColor)
{
	this->wTime = wTime;
	this->bTime = bTime;
	this->wIncrement = wInc;
	this->bIncrement = bInc;

	const uint32_t engineTime = engineColor == WHITE_TURN ? wTime : bTime;
	const uint32_t engineInc = engineColor == WHITE_TURN ? wInc : bInc;

	uint32_t total = 0;

	if (total > engineTime) {
		std::cout << "ERROR: calculated time bad.\n";
		total = engineTime / 2;
	}

	// table implementation
	for (size_t i = 0; i < TMT_SIZE; i++) {
		if (engineTime < entries[i].time) {
			total = entries[i].value;
		}
	}
		
	total += engineInc / 2;

	return total;
}

TimeManager::TimeManager()
{
}
