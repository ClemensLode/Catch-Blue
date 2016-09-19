#ifndef _BLAUE_BASEAGENT_HPP
#define _BLAUE_BASEAGENT_HPP

/**
 * This class provides basic functionality for any moving agents and the goal agent
 * This class cannot be instanciated
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include <assert.h>

#include "sensors.hpp"
#include "geometry.hpp"

enum eAction {
	MOVE_NORTH,
	MOVE_EAST,
	MOVE_SOUTH,
	MOVE_WEST,

	TURN_LEFT,
	TURN_RIGHT,

	MAX_ACTIONS
};


enum eAgentState
{
	// no blue agent was seen recently, randomly move around
	RED_AGENT_SEARCH_BLUE_AGENT,
	// blue agent was seen recently, use the memory to look towards last position
	RED_AGENT_LOOK_BLUE_AGENT,
	// blue agent was seen recently, use the memory to go to last position (if we already have looked for him)
	RED_AGENT_HUNT_BLUE_AGENT,
	// blue agent is in sight / earshot! get him!
	RED_AGENT_ELIMINATE_BLUE_AGENT,
	// red agent is in sight / earshot! run!
	BLUE_AGENT_FLEE_FROM_RED_AGENT,
	// obstacle is very close, stay put.
	BLUE_AGENT_STAY_IN_HIDING,
	// obstacle is near, go there
	BLUE_AGENT_HIDE_IN_HIDING_PLACE,
	// no obstacle in sight? wander randomly around, move towards obstacles
	BLUE_AGENT_SEARCH_HIDING_PLACE
};

enum eAgentColor
{
	RED_AGENT,
	BLUE_AGENT
};


class BaseAgent
{
	public:

		/**
		 * Retrieve sensor data
		 * @param absolute_sensor_information sensor information delievered by the grid
		 */
		void acquireNewSensorData(const Sensors& absolute_sensor_information);

		/**
		 * determines the next action depending on the sensors and the agent type
		 */
		 virtual void calculateNextMove();

		/**
		 * Called before each problem
		 */
		void resetBeforeNewProblem();

		/**
		 * resets the global id so that we can generate new agents
		 * call only before experiments!
		 */
		static void resetGlobalID();

		/**
		 * @return a new unique index number for the agents
		 */
		static unsigned int getNewID();

		/**
		 * Turn sight direction to the left
		 */
		void turnLeft();

		/**
		 * Turn sight direction to the right
		 */
		void turnRight();

		BaseAgent& operator=(const BaseAgent& other);
		~BaseAgent();

// -------- Simple GET/SET functions --------
		eAction getCalculatedAction() const;
		double getHearRange() const;
		double getSightRange() const;
		double getSpeed() const;
		eDirection getSightDirection() const;
		eAgentColor getColor() const;
		eAgentState getState() const;
		bool isAlive() const;
		bool hasMoved() const;
		const GridPoint& getPosition() const;
		void setPosition(const GridPoint& p);
		int getX() const;
		int getY() const;
		unsigned int getID() const;
		void setHasMoved(const bool moved = true);
		void kill();

	protected:

		/**
		 * Create new agent and register the agent in the grid
		 */
		BaseAgent(const double hear_range, 
				  const double sight_range, 
				  const double speed,
				  const eAgentColor color,
				  const eAgentState state);


		/**
		 * The action that was determined by calculateNextMove
		 */
		eAction calculatedAction;
		Sensors lastState;
		eDirection sightDirection;

		/**
		 * current position on the grid
		 */
		GridPoint p;

		/**
		 * unique id of the agent
		 */
		unsigned int id;

		/**
		 * color of the agent (blue, red)
		 */
		eAgentColor color;

		/**
		 * Current state of the finite-state machine
		 */
		eAgentState currentState;

	private:
		const double hearRange;
		const double sightRange;
		const double speed;

		/**
		 * Did this agent move in the last step?
		 */
		bool moved;

		/**
		 * reset global id whenever a new grid with new agents is created
		 */
		static unsigned int global_id;

		bool alive;
};


// -------- Simple GET/SET functions --------

/**
 * @param p the desired position, may only be called by Grid!
 */
inline void BaseAgent::setPosition(const GridPoint& p) {
    this->p = GridPoint(p.x, p.y);
}

inline eAgentState BaseAgent::getState() const {
	return currentState;
}

inline bool BaseAgent::hasMoved() const {
	return moved;
}

inline void BaseAgent::setHasMoved(const bool moved) {
	this->moved = moved;
}


inline bool BaseAgent::isAlive() const {
	return alive;
}

inline void BaseAgent::kill() {
	alive = false;
}

inline double BaseAgent::getHearRange() const {
	return hearRange;
}

inline double BaseAgent::getSightRange() const {
	return sightRange;
}

inline double BaseAgent::getSpeed() const {
	return speed;
}

inline eAgentColor BaseAgent::getColor() const {
	return color;
}

inline eDirection BaseAgent::getSightDirection() const {
	return sightDirection;
}

/**
 * @return the calculatedAction
 */
inline eAction BaseAgent::getCalculatedAction() const {
    return calculatedAction;
}

inline int BaseAgent::getX() const {
    return p.x;
}

inline int BaseAgent::getY() const {
    return p.y;
}

/**
 * @return The unique ID of this agent
 */
inline unsigned int BaseAgent::getID() const {
    return id;
}


/**
 * @return the current position on the grid
 */
inline const GridPoint& BaseAgent::getPosition() const {
    return p;
}

#endif