/**
* Provides routines for the main field.
* Movements and actions of the agents are organized here.
* This class also provides detection routines (sight range)
* 
* @author Clemens Lode, clemens at lode.de
*/

#include "grid.hpp"
#include "redagent.hpp"
#include "blueagent.hpp"

/**
 * Initialize an empty grid and an empty agent list, fill basic geometric vectors
 */
Grid::Grid():
	agentList(),
	obstacleList(),
	randomArray(),
	randomExecutionList(),
	currentStep(0)
{
	Geometry::fillSightPoints();
	Geometry::fillSavedDistances();
	Geometry::fillSavedLinePosition();

	grid.resize(Configuration.getMaxX());
	for(unsigned int i = 0; i < grid.size(); i++) {
		grid[i].resize(Configuration.getMaxY());
	}

	clearGrid();

	BaseAgent::resetGlobalID();

	for (unsigned int i = 0; i < Configuration.getMemberCountRed(); i++) {
		addAgent(new RedAgent());
	}
	for(unsigned int i = 0; i < Configuration.getMemberCountBlue(); i++) {
		addAgent(new BlueAgent());
	}
}

Grid::~Grid()
{
	for(std::list<BaseAgent*>::iterator i = agentList.begin(); i != agentList.end(); i++) {
		delete *i;
	}
	agentList.clear();
}


 /**
 * Move an agent on the grid, marking the former position with 'EMPTY' and
 * the new position with its ID.
 * @param a The agent in question
 * @param action The action the agent wants to take
 * @return true if the movement was succesful
 */
bool Grid::moveAgent(BaseAgent* a, const eAction action) {
	assert(action >= 0 && action < MAX_ACTIONS);
	assert(a->isAlive());
	a->setHasMoved(false);
	switch(action) {
		case NORTH:
		case EAST:
		case SOUTH:
		case WEST:
			{
				if (isDirectionInvalid(a->getPosition(), (eDirection)action)) {
					return false;
				}
				int x = Geometry::correctX[128 + a->getX() + DIRECTION_X[(eDirection)action]];
				int y = Geometry::correctY[128 + a->getY() + DIRECTION_Y[(eDirection)action]];
			    if(grid[x][y].isOccupied()) {
					return false;
			    }
				a->setHasMoved();
			    return putAgentTo(a, GridPoint(x, y));
			}break;
		case TURN_LEFT:a->turnLeft();return true;break;
		case TURN_RIGHT:a->turnRight();return true;break;
		default:assert(false);return false;
	}
}


/**
 * @return A random empty field in the grid
 */
const GridPoint Grid::getFreeField() const {
    int x;
    int y;
    int max_x = Configuration.getMaxX();
    int max_y = Configuration.getMaxY();
    do {
        x = rand()%(max_x);
        y = rand()%(max_y);
    } while (grid[x][y].isOccupied());
    return GridPoint(x, y);
}

/**
 * @param p The base point
 * @param action The action to take
 * @return The resulting coordinate when using the action at the given point
 */
const GridPoint Grid::getNeighborField(const GridPoint& p, const eDirection direction) const {
	int x = Geometry::correctX[128 + p.x + DIRECTION_X[direction]];
	int y = Geometry::correctY[128 + p.y + DIRECTION_Y[direction]];
    return GridPoint(x, y);
}

/**
 * Tests if the agent can move in the direction or if it is blocked
 * @param position Position of agent in question
 * @param direction desired movement direction
 * @return true if movement is invalid
 */
bool Grid::isDirectionInvalid(const GridPoint& position, const eDirection direction) const {
	int x = Geometry::correctX[128 + position.x + DIRECTION_X[direction]];
	int y = Geometry::correctY[128 + position.y + DIRECTION_Y[direction]];
    return grid[x][y].isOccupied();
}


/**
 * @param p The position of the agent in question
 * @return An array of directions in which the agent can move
 */
std::vector<eDirection> Grid::getAvailableDirections(const GridPoint& p) const {
	std::vector<eDirection> list;
	if(!isDirectionInvalid(p, NORTH)) list.push_back(NORTH);
	if(!isDirectionInvalid(p, EAST)) list.push_back(EAST);
	if(!isDirectionInvalid(p, SOUTH)) list.push_back(SOUTH);
	if(!isDirectionInvalid(p, WEST)) list.push_back(WEST);
    return list;
}



/**
 * Puts the agent to a specific point
 * @param a the agent
 * @param p the target point
 * @return true if the target was empty and the move succesful
 */
bool Grid::putAgentTo(BaseAgent* a, const GridPoint& p) {
    if (grid[p.x][p.y].isEmpty()) {
		// clear up old field
		grid[a->getX()][a->getY()].addPath(GridPoint(p.x, p.y), a->getColor());
		grid[a->getX()][a->getY()].setAgent(NULL);

		// initialize new field
		grid[p.x][p.y].setAgent(a);
        a->setPosition(p);
		return true;
    } else {
        return false;
    }
}

/**
 * Clears the grid and places a new configuration of obstacles
 */
void Grid::clearGrid() {
        
	int max_x = Configuration.getMaxX();
    int max_y = Configuration.getMaxY();
    for (int i = 0; i < max_x; i++) {
        for (int j = 0; j < max_y; j++) {
            grid[i][j].clear();
        }
    }

    obstacleList.clear();
    switch (Configuration.getScenarioType()) 
	{
        case RANDOM_SCENARIO:
             {
                unsigned int obstacle_count = Configuration.getObstaclePercentage() * Configuration.getMaxX() * Configuration.getMaxY() / 100;
                while (obstacle_count > 0) {
                    GridPoint p = getFreeField();
                    obstacle_count = createConnectedObstacle(p, obstacle_count, Configuration.getObstacleConnectionFactor());
                }
            }
            break;

        case NON_TORUS_SCENARIO:
             {
                for (unsigned int x = 0; x < Configuration.getMaxX(); x++) {
                    createObstacle(GridPoint(x, 0));
                }
                for (unsigned int y = 1; y < Configuration.getMaxY(); y++) {
                    createObstacle(GridPoint(0, y));
                }
            }
            break;

        case PILLAR_SCENARIO:
             {
                unsigned int nx = Configuration.getMaxX() / 8;
                unsigned int ny = Configuration.getMaxY() / 8;
                for (unsigned int x = 0; x < nx; x++) {
                    for (unsigned int y = 0; y < ny; y++) {
                        createObstacle(GridPoint(x * 8 + 4, y * 8 + 4));
                    }
                }
            }
            break;

        case CROSS_SCENARIO:
             {
                unsigned int x1 = Configuration.getMaxX() / 4;
                unsigned int x2 = Configuration.getMaxX() - x1;
                unsigned int y1 = Configuration.getMaxY() / 4;
                unsigned int y2 = Configuration.getMaxY() - y1;
                unsigned int cx = Configuration.getMaxX() / 2;
                unsigned int cy = Configuration.getMaxY() / 2;
                for (unsigned int x = x1; x <= x2; x++) {
                    createObstacle(GridPoint(x, cy));
                }
                for (unsigned int y = y1; y < cy; y++) {
                    createObstacle(GridPoint(cx, y));
                }
                for (unsigned int y = cy + 1; y <= y2; y++) {
                    createObstacle(GridPoint(cx, y));
                }
            }
            break;

        case ROOM_SCENARIO:
             {
                unsigned int x1 = Configuration.getMaxX() / 4;
                unsigned int x2 = Configuration.getMaxX() - x1;
                unsigned int y1 = Configuration.getMaxY() / 4;
                unsigned int y2 = Configuration.getMaxY() - y1;
                unsigned int c1 = Configuration.getMaxX() / 2 - 1;
                unsigned int c2 = c1 + 3;
                for (unsigned int x = x1; x <= x2; x++) {
                    createObstacle(GridPoint(x, y2));
                    if (x >= c1 && x <= c2) {
                        continue;
                    }
                    createObstacle(GridPoint(x, y1));
                }
                for (unsigned int y = y1 + 1; y < y2; y++) {
                    createObstacle(GridPoint(x1, y));
                    createObstacle(GridPoint(x2, y));
                }
            }
            break;

        case DIFFICULT_SCENARIO:
             {
                unsigned int y1 = Configuration.getMaxY() / 4 - 1;
                unsigned int y2 = Configuration.getMaxY() / 4 + 1;
                unsigned int y3 = (3 * Configuration.getMaxY()) / 4 - 1;
                unsigned int y4 = (3 * Configuration.getMaxY()) / 4 + 1;
                unsigned int n = Configuration.getMaxX() / 4;
                for (unsigned int i = 0; i < Configuration.getMaxY(); i++) {
                    createObstacle(GridPoint(Configuration.getMaxX() - 1, i));
                }
                for (unsigned int i = 1; i < n; i++) {
                    if (i % 2 == 0) {
                        for (unsigned int j = 0; j < y1; j++) {
                            createObstacle(GridPoint(4 * i, j));
                        }
                        for (unsigned int j = y2; j < Configuration.getMaxY(); j++) {
                            createObstacle(GridPoint(4 * i, j));
                        }
                    } else {
                        for (unsigned int j = 0; j < y3; j++) {
                            createObstacle(GridPoint(4 * i, j));
                        }
                        for (unsigned int j = y4; j < Configuration.getMaxY(); j++) {
                            createObstacle(GridPoint(4 * i, j));
                        }
                    }
                }
            }
            break;
    }
}


/**
 * Creates an obstacle at the given point and registers it globally
 * @param p The coordinate for the new obstacle
 */
void Grid::createObstacle(const GridPoint& p) {
    grid[p.x][p.y].setAgent(NULL);
	grid[p.x][p.y].setContent(OBSTACLE);
    obstacleList.push_back(p);
}

/**
 * Taken from http://en.wikipedia.org/wiki/Knuth_shuffle
 * @return A random permutation array
 */
std::vector<int> getRandomArray(int length) {
	std::vector<int> random_array(length);
    for (int i = 0; i < length; i++) {
        random_array[i] = i;
    }

    int n = length;        // The number of items left to shuffle (loop invariant).
    while (n > 1) {
        int k = rand()%n;  // 0 <= k < n.
        n--;                     // n is now the last pertinent index;
        int temp = random_array[n];     // swap array[n] with array[k] (does nothing if k == n).
        random_array[n] = random_array[k];
        random_array[k] = temp;
    }

    return random_array;
}

/**
 * Creates a number of connected obstacles
 * @param p The current position
 * @param obstacle_count The numbeer of remaining obstacles to put in the grid
 * @param factor The probability of creating an obstacle near another obstacle
 * @return The new number of remaining obstacles to put in the grid
 */
unsigned int Grid::createConnectedObstacle(const GridPoint& p, unsigned int& obstacle_count, const unsigned int factor) {
	std::vector<eDirection> list = getAvailableDirections(p);
    if (list.size() > 0 && rand()%100 >= Configuration.getObstacleConnectionFactor()) {
        return obstacle_count;
    }
    createObstacle(p);

    obstacle_count--;

    if (list.empty()) {
        return obstacle_count;
    }

	std::vector<int> rand_array = getRandomArray(list.size());
    for (unsigned int i = 0; i < list.size(); i++) {
        if (obstacle_count == 0) {
            return 0;
        }
        GridPoint t = getNeighborField(p, (eDirection)(rand_array[i]));
        if (grid[t.x][t.y].isOccupied()) {
            continue;
        }

        if (rand()%100 <= factor) {
            obstacle_count = createConnectedObstacle(t, obstacle_count, factor / 2);
        }
    }
    return obstacle_count;
}

/**
 * Resets the grid and the agents, reassigns obstacles, agents and the goal object
 */
void Grid::resetState() {
    clearGrid();
    for each (GridPoint p in obstacleList) {
		grid[p.x][p.y].setAgent(NULL);
		grid[p.x][p.y].setContent(OBSTACLE);
    }

	for(std::list<BaseAgent*>::iterator a = agentList.begin(); a != agentList.end(); a++) {
		if(!(*a)->isAlive()) {
			continue;
		}
		// don't let two different colors start near each other
        (*a)->setPosition(getColorFreeField((*a)->getColor()));
        grid[(*a)->getX()][(*a)->getY()].setAgent(*a);
    }
    updateSight();

    // updating sensor information, initializing data, reanimate agents etc.
    for(std::list<BaseAgent*>::iterator a = agentList.begin(); a != agentList.end(); a++) {
        (*a)->resetBeforeNewProblem();
    }
}

/**
 * Checks all points whether they are visible by an agent
 */
void Grid::updateSight() {
    unsigned int max_x = Configuration.getMaxX();
    unsigned int max_y = Configuration.getMaxY();
    unsigned int max = (unsigned int)ceil(Configuration.getSightRangeBlue());
    for (unsigned int i = 0; i < max_x; i++) {
        for (unsigned int j = 0; j < max_y; j++) {
            grid[i][j].clearSight();
        }
    }

    for(std::list<BaseAgent*>::iterator a = agentList.begin(); a != agentList.end(); a++) {
		if(!(*a)->isAlive()) {
			continue;
		}

        int ax = (*a)->getX();
        int ay = (*a)->getY();

		// iterate through all points of the maximum possible sight area
		for each(GridPoint sp in Geometry::sightPoints[(*a)->getSightRange()]) {

			int tx = Geometry::correctX[128 + ax + sp.x];
			int ty = Geometry::correctY[128 + ay + sp.y];

            // already checked? (it's possible in a torus and with large sight/hear ranges
            if (grid[tx][ty].isSeenBy((*a)->getID()) ||
				grid[tx][ty].isHeardBy((*a)->getID())) {
                continue;
            }
			
			bool line_of_sight_blocked = false;
			// get the line of sight from the agent to the point in question
			SavedLine& line_points = Geometry::savedLinePosition[ax][ay][sp.x + max][sp.y + max];
            for each(GridPoint p in line_points) {
				double dist = Geometry::getTorusDistance(GridPoint(ax, ay), p);
				if((!line_of_sight_blocked) &&
					dist <= (double)(*a)->getSightRange() &&
				    Geometry::getDirection((*a)->getPosition(), p) == (*a)->getSightDirection()) {
					grid[p.x][p.y].addSeen(*a);
				}

				if(dist <= (double)(*a)->getHearRange()) {
					grid[p.x][p.y].addHear(*a);
				}
 
				// check whether the line of sight is blocked
                if (grid[p.x][p.y].isOccupied()) {
                    line_of_sight_blocked = true;
                }
            }
        }
    }
}


/**
 * Determine the positions of all agents sharing a certain color 
 * @param self_id The ID of the agent in question
 * @param color The color of the agents in question
 * @return List of positions of agents of different color in view
 */
const std::list<GridPoint> Grid::getSightedAgentsPositionsList(const unsigned int self_id, const eAgentColor color) const {
	std::list<GridPoint> sighted_agents_positions_list;

	for(std::list<BaseAgent*>::const_iterator a = agentList.begin(); a != agentList.end(); a++) {
		if(!(*a)->isAlive() || (*a)->getColor() != color) {
			continue;
		}

		bool found = false;
		if(grid[(*a)->getX()][(*a)->getY()].isSeenBy(self_id)) {
			sighted_agents_positions_list.push_back((*a)->getPosition());
			found = true;
		}
		if(!found && grid[(*a)->getX()][(*a)->getY()].isHeardBy(self_id)) {
			sighted_agents_positions_list.push_back((*a)->getPosition());
		}
	}

	return sighted_agents_positions_list;
}

/**
 * Determines the nearest obstacle in sight
 * @param self_id The ID of the agent in question
 * @return -1.0 if no obstacle in sight, shortest distance to the obstacle otherwise
 */
double Grid::getDistanceObstacleInSight(const unsigned int self_id) const {
	double min_distance = -1.0;
	for(std::list<GridPoint>::const_iterator o = obstacleList.begin(); o != obstacleList.end(); o++) {
		const BaseAgent* a;
		if(grid[o->x][o->y].isSeenBy(self_id, a)) {
			if(min_distance < 0.0 || Geometry::getTorusDistance(*o, a->getPosition()) < min_distance) {
				min_distance = Geometry::getTorusDistance(*o, a->getPosition());
			}
		}
	}

	return min_distance;
}


/**
 * Gathers the relevant sensor information of an agent
 * @param self_id the ID of the agent in question
 * @return sensor field that the agent is seeing
 * @see Sensors#Sensors
 */
Sensors Grid::getAbsoluteSensorInformation(const unsigned int self_id) const {
    return Sensors(getSightedAgentsPositionsList(self_id, BLUE_AGENT),
				   getSightedAgentsPositionsList(self_id, RED_AGENT),
				   getDistanceObstacleInSight(self_id));
}

/**
 * Put an agent on the grid
 * @param a the BaseAgent
 */
void Grid::addAgent(BaseAgent* a) {
	if(!a->isAlive()) {
		return;
	}

    agentList.push_back(a);
}

/**
 * Determines a random free field with no agents of different color nearby
 * @param color The color in question
 * @return The coordinate of the free field
 */
const GridPoint Grid::getColorFreeField(const eAgentColor color) const {
	do {
		GridPoint p = getFreeField();
		if(!getCloseOpponentAgent(p, color)) {
			return p;
		}
	} while(true);
}

/**
 * Check whether there is an agent of different color within kill distance
 * @param p The position to examine
 * @param color The own color
 * @return The possible adversary, NULL if there is no enemy agent nearby
 */
BaseAgent* Grid::getCloseOpponentAgent(const GridPoint& p, const eAgentColor color) const {
	for(std::list<BaseAgent*>::const_iterator a = agentList.begin(); a != agentList.end(); a++) {
		if(!(*a)->isAlive()) {
			continue;
		}
		if((*a)->getColor() != color && 
			Geometry::getTorusDistance((*a)->getPosition(), p) <= Configuration.getUnsafeDistance()) {
			return *a;
		}
	}
	return NULL;
}

/**
 * Process one step of the grid (i.e. just one agent moves up to one field)
 */
void Grid::doStep() {
	updateSight();
	doAgePaths();
	// we will let the agents move in a random order
	// agents with higher speeds will be executed several times, i.e.
	// put several times into the random_list

	if(currentStep+1 >= randomArray.size()) {

		for(std::list<BaseAgent*>::const_iterator i = agentList.begin(); i != agentList.end(); i++) {
			if(!(*i)->isAlive()) {
				continue;
			}

			// handle fractions
			int agent_speed = (int)(*i)->getSpeed();
			if(rand()%100 <= (*i)->getSpeed() - (double)agent_speed) {
				agent_speed++;
			}
			for(int j = 0; j < agent_speed; j++) {
				randomExecutionList.push_back(*i);
			}
		}
		randomArray = getRandomArray(randomExecutionList.size());
		currentStep = 0;
	} else {
		currentStep++;
	}

	// retrieve the random entry and process it
	BaseAgent* a = randomExecutionList[randomArray[currentStep]];
	if(a->isAlive()) {
		a->acquireNewSensorData(getAbsoluteSensorInformation(a->getID()));
        a->calculateNextMove();
		
		moveAgent(a, a->getCalculatedAction());

		if(a->getColor() == BLUE_AGENT) {
			if(getCloseOpponentAgent(a->getPosition(), BLUE_AGENT)) {
				a->kill();
				grid[a->getX()][a->getY()].setAgent(NULL);
			}
		} else {
			if(BaseAgent* b = getCloseOpponentAgent(a->getPosition(), RED_AGENT)) {
				b->kill();
				grid[b->getX()][b->getY()].setAgent(NULL);
			}
		}
	}
	updateSight();
}

/**
 * Age the agent paths on the grid (they get darker and smaller in the display)
 */
void Grid::doAgePaths() {
    for (unsigned int i = 0; i < Configuration.getMaxX(); i++) {
        for (unsigned int j = 0; j < Configuration.getMaxY(); j++) {
            grid[i][j].doAgePath();
        }
    }
}