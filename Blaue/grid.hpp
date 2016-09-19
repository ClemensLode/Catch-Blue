#ifndef _BLAUE_GRID_HPP
#define _BLAUE_GRID_HPP

/**
 * Provides routines for the main field
 * All movements and collisions of the agents will be registered here
 * This class also provides detection routines (sight range)
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include <vector>
#include <list>

#include "baseagent.hpp"
#include "field.hpp"
#include "configuration.hpp"


class Grid {

	public:
	   /**
	    * Initialize an empty grid and an empty agent list
	    */
		Grid();
		~Grid();

	   /**
	    * Resets the grid and the agents, reassigns obstacles, agents and the goal object
	    */
		void resetState();

	   /**
	    * Put an agent on the grid
	    * @param a the BaseAgent
	    */
		void addAgent(BaseAgent* a);

		/**
		 * Process one step of the grid (i.e. just one agent moves up to one field)
		 */
		void doStep();

		// -------- Simple GET functions --------
		const std::vector< std::vector<Field> > getFieldGrid() const;
		const std::list<BaseAgent*>& getAgentList() const;
		const std::list<GridPoint>& getObstacleList() const;

	private:

    /** 
     * pointer to actual agents and obstacles, 
     * optimization in order to not have to search through the whole grid each time
     */
		std::list<BaseAgent*> agentList;
		std::list<GridPoint> obstacleList;

		/**
		 * The actual grid
		 */
		std::vector< std::vector<Field> > grid;


		// each "doStep()" function will advance the counter
		unsigned int currentStep;
		// the counter selects an entry from random Array...
		std::vector<int> randomArray;
		// and will access the corresponding number in the randomExecutionList
		std::vector<BaseAgent*> randomExecutionList;

		/**
		 * Clears the grid and places a new configuration of obstacles
		 */
		void clearGrid();

	    /**
	     * Creates an obstacle at the given point and registers it globally
	     * @param p The coordinate for the new obstacle
	     */
	    void createObstacle(const GridPoint& p);

	   /**
	    * Checks all points whether they are visible by an agent
	    */
	   void updateSight();

	   /**
	    * @param a Agent in question
	    * @return true if this agent is in sight of the goal agent
	    */
	  bool isGoalAgentInRewardRange(const BaseAgent& a) const;

	   /**
	    * @return true if any agent sees the goal agent
	    */
	   bool isGoalAgentInRewardRangeByAnyAgent() const;

	   /**
	    * Determines the sensor bit field of an agent at 'position' with 
	    * id 'self_id' depending on all other agents
	    * The directions are absolute, i.e. [0] corresponds to NORTH
	    * @param position the position of the agent
	    * @param self_id the id of the agent
	    * @return sensor bit field that the agent is seeing
	    * @see Sensors#Sensors
	    */
		Sensors getAbsoluteSensorInformation(const unsigned int self_id) const;

		/**
		 * Determines a random free field with no agents of different color nearby
		 * @param color The color in question
		 * @return The coordinate of the free field
		 */
		const GridPoint getColorFreeField(const eAgentColor color) const;

		/**
		 * Check whether there is an agent of different color within kill distance
		 * @param p The position to examine
		 * @param color The own color
		 * @return The possible adversary, NULL if there is no enemy agent nearby
		 */
		BaseAgent* getCloseOpponentAgent(const GridPoint& p, const eAgentColor color) const;

		/**
		 * Age the agent paths on the grid (they get darker and smaller in the display)
		 */
		void doAgePaths();

		/**
		 * @param p The position of the agent in question
		 * @return An array of directions in which the agent can move
		 */
		std::vector<eDirection> getAvailableDirections(const GridPoint& p) const;

	   /**
	    * Creates a number of connected obstacles
	    * @param p The current position
	    * @param obstacle_count The numbeer of remaining obstacles to put in the grid
	    * @param factor The probability of creating an obstacle near another obstacle
	    * @return The new number of remaining obstacles to put in the grid
	    */
	    unsigned int createConnectedObstacle(const GridPoint& p, unsigned int& obstacle_count, const unsigned int factor);
  
		/**
		 * Determine the positions of all agents sharing a certain color 
		 * @param self_id The ID of the agent in question
		 * @param color The color of the agents in question
		 * @return List of positions of agents of different color in view
		 */
		const std::list<GridPoint> getSightedAgentsPositionsList(const unsigned int self_id, const eAgentColor color) const;

		/**
		 * Determines the nearest obstacle in sight
		 * @param self_id The ID of the agent in question
		 * @return -1.0 if no obstacle in sight, shortest distance to the obstacle otherwise
		 */
		double getDistanceObstacleInSight(const unsigned int self_id) const;

		/**
		 * Puts the agent to a specific point
		 * @param a the agent
		 * @param p the target point
		 * @return true if the target was empty and the move succesful
		 */
		bool putAgentTo(BaseAgent* a, const GridPoint& p);

			
		/**
		 * @return A random empty field in the grid
		 */
		const GridPoint getFreeField() const;

		/**
		 * @param p The base point
		 * @param action The action to take
		 * @return The resulting coordinate when using the action at the given point
		 */
		const GridPoint getNeighborField(const GridPoint& p, const eDirection direction) const;

		/**
		 * Tests if the agent can move in the direction or if it is blocked
		 * @param position Position of agent in question
		 * @param direction desired movement direction
		 * @return true if movement is invalid
		 */
		bool isDirectionInvalid(const GridPoint& position, const eDirection direction) const;

		/**
		 * Move an agent on the grid, marking the former position with 'EMPTY' and
		 * the new position with its ID.
		 * @param a The agent in question
		 * @param action The action the agent wants to take
		 * @return true if the movement was succesful
		 */
		bool moveAgent(BaseAgent* a, const eAction action);
};


// -------- Simple GET functions --------

inline const std::list<BaseAgent*>& Grid::getAgentList() const {
	return agentList;
}

inline 	const std::list<GridPoint>& Grid::getObstacleList() const {
	return obstacleList;
}

inline const std::vector< std::vector<Field> > Grid::getFieldGrid() const {
	return grid;
}

#endif