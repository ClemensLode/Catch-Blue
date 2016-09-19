/**
 * Red agent class (the predator)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include "redagent.hpp"


RedAgent::RedAgent():
	BaseAgent(Configuration.getHearRangeRed(), 
			  Configuration.getSightRangeRed(), 
			  Configuration.getSpeedRed(),
			  RED_AGENT,
			  RED_AGENT_SEARCH_BLUE_AGENT),
    lastBluePosition(),
	memoryAge(0)
{}

RedAgent::~RedAgent()
{}

void RedAgent::calculateNextMove() {
// blue agents are near => eliminate them
	if(!lastState.getBlueAgentsPositions().empty()) {
		currentState = RED_AGENT_ELIMINATE_BLUE_AGENT;
// no blue agents are near but we remember an old position
	} else if(memoryAge > 0) {
		// hunt them
		if(Geometry::getDirection(getPosition(), lastBluePosition) == getSightDirection()) {
			currentState = RED_AGENT_HUNT_BLUE_AGENT;
		} else {
		// first turn around
			currentState = RED_AGENT_LOOK_BLUE_AGENT;
		}
	} else {
		// wander around
		currentState = RED_AGENT_SEARCH_BLUE_AGENT;
	}

	switch(currentState) {
		case RED_AGENT_ELIMINATE_BLUE_AGENT: 
		{
			memoryAge = Configuration.getMemoryRed();
			// pick the nearest blue agent
			double min = 0.0;
			GridPoint minPoint;
			for(std::list<GridPoint>::const_iterator i = lastState.getBlueAgentsPositions().begin(); i != lastState.getBlueAgentsPositions().end(); i++) {
				if(min == 0.0 || Geometry::getTorusDistance(getPosition(), *i) < min) {
					min = Geometry::getTorusDistance(getPosition(), *i);
					minPoint = *i;
				}
			}
			lastBluePosition = minPoint;
			eDirection direction = Geometry::getDirection(getPosition(), minPoint);
			calculatedAction = (eAction)direction;
		}break;
// simple strategy: turn and go to the last known position
		case RED_AGENT_LOOK_BLUE_AGENT: 
		{
			eDirection direction = Geometry::getDirection(getPosition(), lastBluePosition);
			assert(direction != getSightDirection());

			switch(direction) {
				case NORTH:calculatedAction = getSightDirection()==EAST ?TURN_LEFT:TURN_RIGHT;break;
				case EAST: calculatedAction = getSightDirection()==SOUTH?TURN_LEFT:TURN_RIGHT;break;
				case SOUTH:calculatedAction = getSightDirection()==WEST ?TURN_LEFT:TURN_RIGHT;break;
				case WEST: calculatedAction = getSightDirection()==NORTH?TURN_LEFT:TURN_RIGHT;break;
				default:assert(false);break;
			}
		}
		case RED_AGENT_HUNT_BLUE_AGENT:calculatedAction = (eAction)getSightDirection();break;
	// move randomly
		case RED_AGENT_SEARCH_BLUE_AGENT:calculatedAction = (eAction)(rand()%(MAX_ACTIONS-1));break;
		default:assert(false);break;
	}
// slowly forget the old position
	if(memoryAge > 0) {
		memoryAge--;
	}
}