/**
 * This class provides basic functionality for any moving agents and the goal agent
 * This class cannot be instanciated
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include "baseagent.hpp"

BaseAgent::BaseAgent(const double hear_range, 
					 const double sight_range, 
					 const double speed,
					 const eAgentColor color,
					 const eAgentState state):
	hearRange(hear_range),
	sightRange(sight_range),
	speed(speed),
	id(getNewID()),
	calculatedAction(TURN_LEFT),
	color(color),
	sightDirection((eDirection)(rand()%4)),
	alive(true),
	moved(false),
	currentState(state)
{}

BaseAgent::~BaseAgent() {
}

BaseAgent& BaseAgent::operator=(const BaseAgent& other) {
	assert(false);
	return *this;
}

/**
 * Retrieve sensor data
 * @param absolute_sensor_information sensor information delievered by the grid
 */
void BaseAgent::acquireNewSensorData(const Sensors& absolute_sensor_information) {
    lastState = absolute_sensor_information;
}

/**
 * Turn sight direction to the left
 */
void BaseAgent::turnLeft() {
	switch(sightDirection) {
		case NORTH:sightDirection = WEST;break;
		case EAST:sightDirection = NORTH;break;
		case SOUTH:sightDirection = EAST;break;
		case WEST:sightDirection = SOUTH;break;
		default:assert(false);break;
	}
}

/**
 * Turn sight direction to the right
 */
void BaseAgent::turnRight() {
	switch(sightDirection) {
		case NORTH:sightDirection = EAST;break;
		case EAST:sightDirection = SOUTH;break;
		case SOUTH:sightDirection = WEST;break;
		case WEST:sightDirection = NORTH;break;
		default:assert(false);break;
	}
}
/**
 * Called before each problem
 */
void BaseAgent::resetBeforeNewProblem() {
	alive = true;
}

/**
 * resets the global id so that we can generate new agents
 * call only before experiments!
 */
void BaseAgent::resetGlobalID() {
    global_id = 0;
}

/**
 * @return a new unique index number for the agents
 */
unsigned int BaseAgent::getNewID() {
    unsigned int new_id = global_id;
    global_id++;
    return new_id;
}

void BaseAgent::calculateNextMove() {
}

unsigned int BaseAgent::global_id = 0;