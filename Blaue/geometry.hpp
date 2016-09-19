#ifndef _BLAUE_GEOMETRY_HPP
#define _BLAUE_GEOMETRY_HPP

/**
 * Basic geometry class (lines, distances, torus etc.)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <vector>
#include <list>
#include <math.h>

#include "configuration.hpp"
#include "gridpoint.hpp"

enum eDirection {
    NORTH,
    EAST,
    SOUTH,
    WEST,

	MAX_DIRECTIONS
};


// mapping of directions to x/y deltas
extern const int DIRECTION_X[MAX_DIRECTIONS];
extern const int DIRECTION_Y[MAX_DIRECTIONS];

typedef std::vector<GridPoint> SavedLine;
typedef std::vector< std::vector<SavedLine> > SavedLinePosition;


class Geometry {
	public:

		/**
		 * Precalculated Bresenham lines
		 */
		static std::vector<std::vector<GridPoint> > sightPoints;		
		static std::vector< std::vector<SavedLinePosition> > savedLinePosition;
		
		/**
		 * Translate any coordinate to a coordinate within the torus
		 */
		static std::vector<int> correctX;
		static std::vector<int> correctY;

		/**
		 * @param a Base point
		 * @param b Target point
		 * @return The distance between a and b on a torus
		 */
		static double getTorusDistance(const GridPoint& a, const GridPoint& b);

		/**
		 * Precompute the sightPoints
		 */
		static void fillSightPoints();

		/**
		 * Precompute the distances on the torus
		 */
		static void fillSavedDistances();

		/**
		 * Precompute the lines
		 */
		static void fillSavedLinePosition();

		/**
		 * Determine direction from position1 looking at position2, depending on
		 * the type of field (Torus, grid with borders)
		 * @param position1 base point
		 * @param position2 point to look at
		 * @return Direction (0=north, 1=east, 2=south, 3=west)
		 */
		static eDirection getDirection(const GridPoint& position1, const GridPoint& position2);

		/**
		 * corrects a x-coordinate outside of the grid if it is a torus
		 * @param x the x-coordinate in question
		 * @return the corrected coordinate
		 */
		static int correctTorusX(const int x);

		/**
		 * corrects a y-coordinate outside of the grid if it is a torus
		 * @param y the y-coordinate in question
		 * @return the corrected coordinate
		 */
		static int correctTorusY(const int y);

		/**
		 * @param x1 base point
		 * @param x2 goal point
		 * @return Absolute (minimal) X-Distance between the two points on a torus
		 */
		static int getTorusDistanceX(const int x1, const int x2);

		/**
		 * @param y1 base point
		 * @param y2 goal point
		 * @return Absolute (minimal) Y-Distance between the two points on a torus
		 */
		static int getTorusDistanceY(const int y1, const int y2);

	protected:
		/**
		 * Calculates a line that correctly traverses over the torus boundaries
		 * @param a starting point
		 * @param b goal point
		 * @return the array of points of a line in the torus
		 */
		static std::vector<GridPoint> getTorusLine(const GridPoint& a, const GridPoint& b);

	private:
		/**
		 * @param a base point
		 * @param b point to look at
		 * @return The relative direction of point b to point a
		 */
		static eDirection getGridDirection(const int dx, const int dy);

		/**
		 * corrects a x-coordinate outside of the grid if it is a torus
		 * @param x the x-coordinate in question
		 * @return the corrected coordinate
		 */
		static float correctTorusFloatX(const float x);

		/**
		 * corrects a y-coordinate outside of the grid if it is a torus
		 * @param y the y-coordinate in question
		 * @return the corrected coordinate
		 */
		static float correctTorusFloatY(const float y);

		static std::vector< std::vector< std::vector< std::vector<double> > > > torusDistance;

		/**
		 * X and Y distances on a torus
		 */
		static std::vector<std::vector<int> > torusDistanceX;
		static std::vector<std::vector<int> > torusDistanceY;
};


// -------- Simple GET functions --------

inline double Geometry::getTorusDistance(const GridPoint& a, const GridPoint& b) {
	return torusDistance[a.x][a.y][b.x][b.y];
}

#endif