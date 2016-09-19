#include "size.hpp"
#include <sstream>
 void Size::setWidth(const Uint16 width) {
	assert(width <= 10000);
	this->width = width;
}
		
 void Size::setHeight(const Uint16 height) {
	assert(height <= 10000);
	this->height = height;
}

const bool Size::mv(Uint16& x, const Uint16 sx, const Uint16 tx)
{
	assert( (x >= sx && x <= tx) || (x <= sx && x >= tx) );

	if(x == tx) {
		return(true);
	}

	Sint16 z;
	if(sx < tx) {
		if(2*x < tx+sx) {
			z = (Sint16)x - (Sint16)sx;
		} else {
			z = (Sint16)tx - (Sint16)x;
		}
	} else {
		if(2*x > tx+sx) {
			z = (Sint16)x - (Sint16)sx;
		} else {
			z = (Sint16)tx - (Sint16)x;
		}
	}
	if(abs(z) > 1) {
		z /= 2;
	}
	if(z == 0) {
		z = sx<tx?1:-1;
	}

	x = (Uint16)(((Sint16)x) + z);

	return(false);
}	

void Size::move(const Size& startSize, const Size& targetSize)
{
	mv(width, startSize.width, targetSize.width);
	mv(height, startSize.height, targetSize.height);
}	



const std::string Size::toString() const
{
	std::ostringstream os;
	os << getWidth() << "x" << getHeight();
	return(os.str());
}
