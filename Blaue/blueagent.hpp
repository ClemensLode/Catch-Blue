#ifndef _BLAUE_BLUEAGENT_HPP
#define _BLAUE_BLUEAGENT_HPP

/**
 * Blue agent class (the prey)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include "baseagent.hpp"

class BlueAgent : public BaseAgent
{
	public:
		BlueAgent();
		~BlueAgent();

		void calculateNextMove();
};

#endif