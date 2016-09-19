/**
 * Base geometry class
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <math.h>
#include <assert.h>

#include "gridpoint.hpp"

GridPoint::GridPoint(const int x, const int y):
	x(x),
	y(y)
{
	assert(x < 1000);
	assert(y < 1000);
}

GridPoint::GridPoint():
	x(0),
	y(0)
{}

GridPoint::~GridPoint()
{}

double GridPoint::distance(const GridPoint& p) const {
	int dx = x > p.x? x - p.x : p.x - x;
	int dy = y > p.y? y - p.y : p.y - y;
	return sqrt((double)(dx * dx + dy * dy));
}
