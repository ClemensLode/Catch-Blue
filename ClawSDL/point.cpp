#include "point.hpp"
void Point::setX(const Sint16 x) {
	assert(x >= -10000 && x <= 10000);
	this->x = x;
}

void Point::setY(const Sint16 y) {
	assert(y >= -10000 && y <= 10000);
	this->y = y;
}
const bool Point::mv(Sint16& x, const Sint16 sx, const Sint16 tx)
{
	assert( (x >= sx && x <= tx) || (x <= sx && x >= tx) );

	if(x == tx)
		return(true);

	Sint16 z;
	
	if(sx < tx) {
		if(2*x < tx+sx) {
			z = x - sx;
		} else {
			z = tx - x;
		}
	} else {
		if(2*x > tx+sx) {
			z = x - sx;
		} else {
			z = tx - x;
		}
	}
	if(z == 0) {
		z = sx<tx?1:-1;
	}

	x += z;

	return(false);
}	


void Point::move(const Point& startPoint, const Point& targetPoint)
{
	mv(x, startPoint.x, targetPoint.x);
	mv(y, startPoint.y, targetPoint.y);
}	
