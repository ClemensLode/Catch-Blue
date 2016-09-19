/**
 * Blue agent class (the prey)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include "blueagent.hpp"

BlueAgent::BlueAgent():
	BaseAgent(Configuration.getHearRangeBlue(), 
			  Configuration.getSightRangeBlue(), 
			  Configuration.getSpeedBlue(),
			  BLUE_AGENT,
			  BLUE_AGENT_SEARCH_HIDING_PLACE)
{}

BlueAgent::~BlueAgent()
{}

void BlueAgent::calculateNextMove() {
// blue agents in sight! Try to flee
	if(!lastState.getRedAgentsPositions().empty()) {
		currentState = BLUE_AGENT_FLEE_FROM_RED_AGENT;
	} else 
// We see an obstacle and we're not already hiding => Go to the obstacle
	if(currentState != BLUE_AGENT_STAY_IN_HIDING && lastState.getDistanceObstacleInSight() > 0) {
		currentState = BLUE_AGENT_HIDE_IN_HIDING_PLACE;
	} else 
// An obstacle is very near, hide there
	if(lastState.getDistanceObstacleInSight() > 0 && lastState.getDistanceObstacleInSight() < 2.0) {
		currentState = BLUE_AGENT_STAY_IN_HIDING;
	} else 
// Wander around	
	{
		currentState = BLUE_AGENT_SEARCH_HIDING_PLACE;
	}

	switch(currentState) {
		case BLUE_AGENT_FLEE_FROM_RED_AGENT: 
			{
				// Look for free directions. If there are none, pick a random direction.
				std::vector<bool> free_directions; 
				free_directions.resize(MAX_DIRECTIONS);
				for(int i = 0; i < MAX_DIRECTIONS; i++) {
					free_directions[i] = true;
				}
				for(std::list<GridPoint>::const_iterator i = lastState.getRedAgentsPositions().begin(); i != lastState.getRedAgentsPositions().end(); i++) {
					free_directions[Geometry::getDirection(getPosition(), *i)] = false;
				}
				unsigned int directions_count = 0;
				for(int i = 0; i < MAX_DIRECTIONS; i++) {
					if(free_directions[i]) {
						directions_count++;
					}
				}
				if(directions_count == 0) {
					calculatedAction = (eAction)(rand()%MAX_DIRECTIONS);
				} else {
					int rand_dir = rand()%directions_count;
					for(int i = 0; i < MAX_DIRECTIONS; i++) {
						if(free_directions[i]) {
							if(rand_dir == 0) {
								calculatedAction = (eAction)i;
								break;
							} else {
								rand_dir--;
							}
						}
					}
				}
			}
	// wait here, turn around to look for incoming red agents
		case BLUE_AGENT_STAY_IN_HIDING:calculatedAction = TURN_RIGHT;break;
	// move towards sight direction
		case BLUE_AGENT_HIDE_IN_HIDING_PLACE:calculatedAction = (eAction)getSightDirection();break;
	// move randomly
		case BLUE_AGENT_SEARCH_HIDING_PLACE:calculatedAction = (eAction)(rand()%(MAX_ACTIONS-1));break;
		default:assert(false);break;
	}

}