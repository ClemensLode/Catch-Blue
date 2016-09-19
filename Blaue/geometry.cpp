/**
 * Basic geometry class (lines, distances, torus etc.)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <math.h>

#include "geometry.hpp"

/**
 * Precompute the sightPoints
 */
void Geometry::fillSightPoints() 
{
	// sight range blue is the larger sight range
	sightPoints.resize((int)ceil(Configuration.getSightRangeBlue())+1);
// iterate through all possible sight ranges
	for(int max = (int)floor(Configuration.getSightRangeRed()); max <= (int)ceil(Configuration.getSightRangeBlue()); max++) {
// iterate through all circle radii
		for (int i = max; i >= 1; i--) {
		// iterate through all coordinates of the circle radius
			for (int x = -i; x <= i; x++) {
				for (int y = -i; y <= i; y++) {

				// ignore the center of the circle
					if (x == 0 && y == 0) {
						continue;
					}

					// ignore the area outside of the circle
					double dist = sqrt((double) (x * x + y * y));
					if (dist > ((double) i) || (dist <= ((double) (i - 1)))) {
						continue;
					}
					GridPoint p = GridPoint(x, y);
					bool found = false;

					// check whether we already know this point
					for(unsigned int j = 0; j < sightPoints[max].size(); j++) {
						if(sightPoints[max][j] == p) {
							found = true;
							break;
						}
					}
					if (found) {
						continue;
					}

					// it's not yet in sightPoints => add it
					sightPoints[max].push_back(p);
				}
			}
		}
	}
}

/**
 * Precompute the distances on the torus
 */
void Geometry::fillSavedDistances() 
{
    int max_x = Configuration.getMaxX();
    int max_y = Configuration.getMaxY();
	torusDistanceX.resize(max_x);
    torusDistanceY.resize(max_y);
    correctX.resize(128 + max_x + 128);
    correctY.resize(128 + max_y + 128);

    for (int x = 0; x < 256 + max_x; x++) {
        correctX[x] = correctTorusX(x - 128);
    }
    for (int y = 0; y < 256 + max_y; y++) {
        correctY[y] = correctTorusY(y - 128);
    }

    for (int x1 = 0; x1 < max_x; x1++) {
		torusDistanceX[x1].resize(max_x);
        for (int x2 = 0; x2 < max_x; x2++) {
            torusDistanceX[x1][x2] = getTorusDistanceX(x1, x2);
        }
    }
    for (int y1 = 0; y1 < max_y; y1++) {
		torusDistanceY[y1].resize(max_y);
        for (int y2 = 0; y2 < max_y; y2++) {
            torusDistanceY[y1][y2] = getTorusDistanceY(y1, y2);
        }
    }
    torusDistance.resize(max_x);
    for (int x = 0; x < max_x; x++) {
		torusDistance[x].resize(max_y);
        for (int y = 0; y < max_y; y++) {
			torusDistance[x][y].resize(max_x);
            for (int a = 0; a < max_x; a++) {
				torusDistance[x][y][a].resize(max_y);
                for (int b = 0; b < max_y; b++) {
                    int tdx = torusDistanceX[x][a];
                    int tdy = torusDistanceY[y][b];
                    torusDistance[x][y][a][b] = sqrt((double)(tdx * tdx + tdy * tdy));
                }
            }
        }
    }
}

/**
 * Precompute the lines of sight
 */
void Geometry::fillSavedLinePosition() 
{
    int max_x = Configuration.getMaxX();
    int max_y = Configuration.getMaxY();

	// sight range blue is the largest sight range
    const int max = (int)ceil(Configuration.getSightRangeBlue());

	savedLinePosition.resize(max_x);
    for (int x = 0; x < max_x; x++) {
		savedLinePosition[x].resize(max_y);
        for (int y = 0; y < max_y; y++) {
            savedLinePosition[x][y].resize(2*max+1);
            for (int dx = -max; dx <= max; dx++) {
				savedLinePosition[x][y][dx+max].resize(2*max+1);
                for (int dy = -max; dy <= max; dy++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    double dist = sqrt((double) (dx * dx + dy * dy));
                    if (dist > max) {
                        continue;
                    }
                    int tx = correctX[x + dx + 128];
                    int ty = correctY[y + dy + 128];
					savedLinePosition[x][y][dx + max][dy + max] = getTorusLine(GridPoint(x, y), GridPoint(tx, ty));
                }
            }
        }
    }
}

/**
 * Determine direction from position1 looking at position2, depending on
 * the type of field (Torus, grid with borders)
 * @param position1 base point
 * @param position2 point to look at
 * @return Direction (0=north, 1=east, 2=south, 3=west)
 */
eDirection Geometry::getDirection(const GridPoint& position1, const GridPoint& position2)  {
    return getGridDirection(torusDistanceX[position1.x][position2.x],
            torusDistanceY[position1.y][position2.y]);
}

/**
 * corrects a x-coordinate outside of the grid if it is a torus
 * @param x the x-coordinate in question
 * @return the corrected coordinate
 */
int Geometry::correctTorusX(const int x) {
    if (x < 0) {
        return x + (signed int)Configuration.getMaxX();
    } else if (x >= (signed int)Configuration.getMaxX()) {
        return x - (signed int)Configuration.getMaxX();
    }
    return x;
}

/**
 * corrects a y-coordinate outside of the grid if it is a torus
 * @param y the y-coordinate in question
 * @return the corrected coordinate
 */
int Geometry::correctTorusY(const int y) {
    if (y < 0) {
        return y + (signed int)Configuration.getMaxY();
    } else if (y >= (signed int)Configuration.getMaxY()) {
        return y - (signed int)Configuration.getMaxY();
    }
    return y;
}

/**
 * @param a starting point
 * @param b goal point
 * @return the array of points of a line in the torus
 */
std::vector<GridPoint> Geometry::getTorusLine(const GridPoint& a, const GridPoint& b) {
	std::vector<GridPoint> list;

    if (a.x == b.x && a.y == b.y) {
        return list;
    }

    float dx = (float)torusDistanceX[a.x][b.x];
    float dy = (float)torusDistanceY[a.y][b.y];



    if (abs(dx) > abs(dy)) {
        dy /= abs(dx);
        int tx = dx > 0.0 ? 1 : -1;

        int x = a.x;
        float y = (float)a.y;

        do {
            x += tx;
            y += dy;
            x = correctTorusX(x);
            y = correctTorusFloatY(y);

			assert(x>=0);
			assert(my_round(y)>=0);
            list.push_back(GridPoint(x, my_round(y)));
        } while (x != b.x);


    } else {
        dx /= abs(dy);
        int ty = dy > 0.0 ? 1 : -1;

        float x = (float)a.x;
        int y = a.y;

        do {
            x += dx;
            y += ty;
		//	printf("%f -> ", x);
            x = correctTorusFloatX(x);
            y = correctTorusY(y);
		//	printf("%f -> %i\n", x, my_round(x));
			assert(my_round(x)>=0);
			assert(y>=0);
            list.push_back(GridPoint(my_round(x), y));
        } while (y != b.y);
    }
    return list;

}

/**
 * @param x1 base point
 * @param x2 goal point
 * @return Absolute (minimal) X-Distance between the two points on a torus
 */
int Geometry::getTorusDistanceX(const int x1, const int x2) {
    int tdx = x2 - x1;
    if (tdx < -(signed int)Configuration.getHalfMaxX()) {
        tdx += (signed int)Configuration.getMaxX();
    } else if (tdx >= (signed int)Configuration.getHalfMaxX()) {
        tdx -= (signed int)Configuration.getMaxX();
    }
    return tdx;
}

/**
 * @param y1 base point
 * @param y2 goal point
 * @return Absolute (minimal) Y-Distance between the two points on a torus
 */
int Geometry::getTorusDistanceY(const int y1, const int y2) {
    int tdy = y2 - y1;
    if (tdy < -(signed int)Configuration.getHalfMaxY()) {
        tdy += (signed int)Configuration.getMaxY();
    } else if (tdy >= (signed int)Configuration.getHalfMaxY()) {
        tdy -= (signed int)Configuration.getMaxY();
    }
    return tdy;
}


/**
 * @param dx vector 
 * @param dy vector
 * @return The direction of vector dx/dy
 */
eDirection Geometry::getGridDirection(const int dx, const int dy) {
	//assert(dx != 0 || dy != 0);
    // east or south (2/0), (2/1), (1/2), (1/1), (0/2)
    if (dx >= 0 && dy >= 0) {
        if (dx > dy) {
			return EAST;
        } else {
            return SOUTH;
        }
    } // west or south
    else if (dx <= 0 && dy >= 0) {
        if (abs(dx) >= abs(dy)) {
            return WEST;
        } else {
            return SOUTH;
        }
    } // east or north
    else if (dx >= 0 && dy <= 0) {
        if (abs(dx) < abs(dy)) {
            return NORTH;
        } else {
            return EAST;
        }
    } // west or north
    else //if(dx < 0 && dy < 0)
    {
        if (abs(dx) > abs(dy)) {
            return WEST;
        } else {
            return NORTH;
        }
    }
}

/**
 * corrects a x-coordinate outside of the grid if it is a torus
 * @param x the x-coordinate in question
 * @return the corrected coordinate
 */
float Geometry::correctTorusFloatX(const float x) {
    if (my_round(x) < 0) {
        return x + (float) Configuration.getMaxX();
    } else if (my_round(x) >= (signed int)Configuration.getMaxX()) {
        return x - (float) Configuration.getMaxX();
    }
    return x;
}

/**
 * corrects a y-coordinate outside of the grid if it is a torus
 * @param y the y-coordinate in question
 * @return the corrected coordinate
 */
float Geometry::correctTorusFloatY(const float y) {
    if (my_round(y) < 0) {
        return y + (float) Configuration.getMaxY();
    } else if (my_round(y) >= (signed int)Configuration.getMaxY()) {
        return y - (float) Configuration.getMaxY();
    }
    return y;
}





const int DIRECTION_X[MAX_DIRECTIONS] = {0, 1, 0, -1};
const int DIRECTION_Y[MAX_DIRECTIONS] = {-1, 0, 1, 0};

std::vector<std::vector<GridPoint> > Geometry::sightPoints;
std::vector<std::vector<SavedLinePosition> > Geometry::savedLinePosition;
std::vector<std::vector< std::vector< std::vector<double> > > > Geometry::torusDistance;
std::vector<std::vector<int> > Geometry::torusDistanceX;
std::vector<std::vector<int> > Geometry::torusDistanceY;
std::vector<int> Geometry::correctX;
std::vector<int> Geometry::correctY;


