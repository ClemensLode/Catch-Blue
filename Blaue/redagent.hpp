#ifndef _BLAUE_REDAGENT_HPP
#define _BLAUE_REDAGENT_HPP

/**
 * Red agent class (the predator)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include "baseagent.hpp"

class RedAgent : public BaseAgent
{
	public:
		RedAgent();
		~RedAgent();

		void calculateNextMove();
	private:
		GridPoint lastBluePosition;
		unsigned int memoryAge;
};

#endif