#ifndef _SDL_POINT_HPP
#define _SDL_POINT_HPP

#include "size.hpp"

class DC_API Point
{
	public:
		Point();
		Point(const Sint16 x, const Sint16 y);
		Point& operator=(const Point& point);
		const bool operator==(const Point& point) const;
		const bool operator!=(const Point& point) const;
		const Point operator+(const Point& point) const;
		const Point operator*(const Sint16 scalar) const;
		const Point operator*(const Size& size) const;
		const Point operator+(const Size& size) const;
		const Point operator-(const Size& size) const;
		const Size operator-(const Point& point) const;
		Point& operator+=(const Size& size);
		Point& operator-=(const Size& size);
		const bool operator>=(const Point& point) const;
		const bool operator<=(const Point& point) const;
		const bool operator>(const Point& point) const;
		const bool operator<(const Point& point) const;
		void move(const Point& startPoint, const Point& targetPoint);
		static const bool mv(Sint16& x, const Sint16 sx, const Sint16 tx);

		void setX(const Sint16 x);
		void setY(const Sint16 y);

		const Sint16 getX() const;
		const Sint16 getY() const;
	
	private:
		Sint16 x;
		Sint16 y;
};


inline Point::Point():	x(0), y(0)
{ }
	
inline Point::Point(const Sint16 x, const Sint16 y)
{
	setX(x);
	setY(y);
}



inline const Sint16 Point::getX() const {
	return x;
}

inline const Sint16 Point::getY() const {
	return y;
}

inline Point& Point::operator=(const Point& point)
{
	setX(point.x);
	setY(point.y);
	return(*this);
}

inline const bool Point::operator==(const Point& point) const {
	return ((x == point.x) && (y == point.y));
}

inline const bool Point::operator!=(const Point& point) const {
	return !(*this == point);
}

inline const Point Point::operator*(const Sint16 scalar) const	{
	return Point(x * scalar, y * scalar );
}

inline const Point Point::operator*(const Size& size) const {
	return Point(x * size.getWidth(), y * size.getHeight());
}


inline const Point Point::operator+(const Point& point) const	{
	return Point(x + point.x, y + point.y );
}

inline const Point Point::operator+(const Size& size) const	{
	return Point(x + size.getWidth(), y + size.getHeight() );
}

inline const Point Point::operator-(const Size& size) const {
	return Point(x - size.getWidth(), y - size.getHeight() );
}
	
inline const Size Point::operator-(const Point& point) const {
	return Size((Uint16)abs(x - point.x), (Uint16)abs(y - point.y));
}

inline Point& Point::operator+=(const Size& size) 
{
	setX(x + size.getWidth());
	setY(y + size.getHeight());
	return *this;
}
		
inline Point& Point::operator-=(const Size& size)
{
	setX(x - size.getWidth());
	setY(y - size.getHeight());
	return *this;
}

inline const bool Point::operator>=(const Point& point) const
{
	if((x >= point.x) && (y >= point.y))
		return(true);
	else return(false);
}

inline const bool Point::operator<=(const Point& point) const
{
	if((x <= point.x) && (y <= point.y))
		return(true);
	else return(false);
}
	
inline const bool Point::operator>(const Point& point) const
{
	if((x > point.x) && (y > point.y))
		return(true);
	else return(false);
}
	
inline const bool Point::operator<(const Point& point) const
{
	if((x < point.x) && (y < point.y))
		return(true);
	else return(false);
}	


#endif // _SDL_POINT_HPP
