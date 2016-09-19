/**
 * This class provides a container for formatted sensor information
 * New instances are created by the grid class
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include "sensors.hpp"

/**
 * Creates a sensor information object out of the given information from the grid
 * @param blue_agents_positions The positions of nearby blue agents
 * @param red_agents_positions The positions of nearby red agents
 * @param distance_obstacle_in_sight The smallest distance to an obstacle nearby
 * @see Grid#getAbsoluteSensorInformation
 */
Sensors::Sensors(const std::list<GridPoint>& blue_agents_positions,
			     const std::list<GridPoint>& red_agents_positions,
				 const double distance_obstacle_in_sight) 
{
	distanceObstacleInSight = distance_obstacle_in_sight;
	blueAgentsPositions.insert(blueAgentsPositions.begin(), blue_agents_positions.begin(), blue_agents_positions.end());
	redAgentsPositions.insert(redAgentsPositions.begin(), red_agents_positions.begin(), red_agents_positions.end());
}

Sensors::Sensors() {
}

Sensors& Sensors::operator=(const Sensors& other) {
	distanceObstacleInSight = other.distanceObstacleInSight;
	blueAgentsPositions.clear();
	redAgentsPositions.clear();
	blueAgentsPositions.insert(blueAgentsPositions.begin(), other.blueAgentsPositions.begin(), other.blueAgentsPositions.end());
	redAgentsPositions.insert(redAgentsPositions.begin(), other.redAgentsPositions.begin(), other.redAgentsPositions.end());

	return *this;
}