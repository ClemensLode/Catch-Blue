#ifndef _BLAUE_GRIDPOINT_HPP
#define _BLAUE_GRIDPOINT_HPP

/**
 * Base geometry class
 *
 * @author Clemens Lode, clemens at lode.de
 */

class GridPoint {
	public:
		int x;
		int y;

		GridPoint(const int x, const int y);
		GridPoint();
		~GridPoint();
		const bool operator==(const GridPoint& point) const;
		double distance(const GridPoint& p) const;
};

// -------- Simple GET functions --------

inline const bool GridPoint::operator==(const GridPoint& point) const {
	return ((x == point.x) && (y == point.y));
}

#endif