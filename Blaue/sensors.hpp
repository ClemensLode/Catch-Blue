#ifndef _BLAUE_SENSORS_HPP
#define _BLAUE_SENSORS_HPP

/**
 * This class provides a container for formatted sensor information
 * New instances are created by the grid class
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include <vector>
#include <list>

#include "gridpoint.hpp"


class Sensors {
	public:

		/**
		 * Creates a sensor information object out of the given information from the grid
		 * @param goal_agent_direction The absolute direction of the goal agent (-1 if no goal agent is in sight)
		 * @param absolute_direction_agent_distance The absolute distances to the nearest agent for each direction in an array 
		 * @param absolute_direction_obstacle_distance The absolute distances to the nearest ovstacle for each direction in an array 
		 * @see Grid#getAbsoluteSensorInformation
		 */
		Sensors(const std::list<GridPoint>& blue_agents_positions,
			    const std::list<GridPoint>& red_agents_positions,
				const double distance_obstacle_in_sight);
		Sensors();
		Sensors& operator=(const Sensors& other);

// -------- Simple GET/SET functions --------
		double getDistanceObstacleInSight() const;
		const std::list<GridPoint>& getBlueAgentsPositions() const;
		const std::list<GridPoint>& getRedAgentsPositions() const;

	private:
		double distanceObstacleInSight;
		std::list<GridPoint> blueAgentsPositions;
		std::list<GridPoint> redAgentsPositions;
};


// -------- Simple GET/SET functions --------
inline double Sensors::getDistanceObstacleInSight() const {
	return distanceObstacleInSight;
}

inline const std::list<GridPoint>& Sensors::getBlueAgentsPositions() const {
	return blueAgentsPositions;
}

inline const std::list<GridPoint>& Sensors::getRedAgentsPositions() const {
	return redAgentsPositions;
}

#endif