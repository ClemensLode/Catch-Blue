#ifndef _SDL_RECT_HPP
#define _SDL_RECT_HPP

#include "point.hpp"
#include <list>

enum eTouchType 
{
	TOUCHES_NO_TOUCH,
	TOUCHES_IS_COMPLETELY_INSIDE,
	TOUCHES_CROSSES_RIGHT_BORDER,
	TOUCHES_CROSSES_LOWER_BORDER,
	TOUCHES_CROSSES_LEFT_BORDER,
	TOUCHES_CROSSES_UPPER_BORDER,
	TOUCHES_CROSSES_BOTTOM_RIGHT_CORNER,
	TOUCHES_CROSSES_BOTTOM_LEFT_CORNER,
	TOUCHES_CROSSES_TOP_LEFT_CORNER,
	TOUCHES_CROSSES_TOP_RIGHT_CORNER,
	TOUCHES_CROSSES_LEFT_AND_RIGHT_BORDER,
	TOUCHES_CROSSES_TOP_AND_BOTTOM_BORDER,
	TOUCHES_CROSSES_UPPER_AREA,
	TOUCHES_CROSSES_RIGHT_AREA,
	TOUCHES_CROSSES_LOWER_AREA,
	TOUCHES_CROSSES_LEFT_AREA,

	TOUCHES_RIGHT_BORDER,
	TOUCHES_LOWER_BORDER,
	TOUCHES_LEFT_BORDER,
	TOUCHES_UPPER_BORDER
};



class DC_API Rect
{
	public:
		Rect();
		Rect(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height);
		Rect(const Point& pos, const Size& size);
		Rect(const Point& top_left_pos, const Point& bottom_right_pos);
		Rect(const Rect& rect);
		
		const std::string toString() const;

		const Point& getTopLeft() const;
		const Point getBottomLeft() const;
		void setTopLeft( const Point& point );
		const Point& getBottomRight() const;
		const Point getTopRight() const;
	
		const Size& getSize() const;
		void setSize( const Size& size );
		const Uint16 getWidth() const;
		void setWidth(const Uint16 width);
		const Uint16 getHeight() const;
		void setHeight(const Uint16 height);
		const Sint16 getLeft() const;
		const Sint16 getTop() const;
		const Sint16 getBottom() const;
		const Sint16 getRight()  const;
		void setLeft(const Sint16 left);
		void setTop(const Sint16 top); 
		void setRight(const Sint16 right);
		void setBottom(const Sint16 bottom);

		void calculateBottomRightCorner();
		void correctStartTargetRectPosition(Point& start_rect_pos, Point& target_rect_pos);
		void correctStartTargetRectSize(Size& start_rect_size, Size& target_rect_size);


		const Rect operator+(const Point p) const;
		Rect& operator=(const Rect& rect);
		const bool operator==(const Rect& rect) const;
		const bool operator!=(const Rect& rect) const;
		const bool isPixelInside(const Sint16 x, const Sint16 y) const;
		const bool isPixelInside(const Point& point) const;
		const bool isTopLeftCornerInside(const Sint16 x, const Sint16 y) const;
		const bool isTopRightCornerInside(const Sint16 x, const Sint16 y) const;
		const bool isBottomRightCornerInside(const Sint16 x, const Sint16 y) const;
		const bool isBottomLeftCornerInside(const Sint16 x, const Sint16 y) const;
		const bool isTopLeftCornerInside(const Point& point) const;
		const bool isTopRightCornerInside(const Point& point) const;
		const bool isBottomRightCornerInside(const Point& point) const;
		const bool isBottomLeftCornerInside(const Point& point) const;
		const bool isRectInside(const Rect& rect) const;

		const eTouchType getTouchType(const Rect& rect) const;
		const bool isTouched(const Rect& rect) const;

		const Rect commonRect(const Rect& rect) const; // Schnittmenge
		std::list<Rect> withoutRect(Rect rect) const; // Ohne



		void moveSmooth(Rect& start_rect, Rect& target_rect);

		/*static void testCommonRect();
		static void testConnectRect();
		static void testConnectRects();
		static void testWithoutRect();
		static void testCutFromList();*/
	private:
		Point topLeftCorner;
		Point bottomRightCorner;
		Size rectSize;
};

	
inline Rect::Rect():topLeftCorner(0,0),bottomRightCorner(0,0), rectSize(0,0) 
{ }

inline Rect::Rect(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) :
	topLeftCorner(x, y),
	bottomRightCorner(x + width, y + height),
	rectSize(width, height)
{ }

	
inline Rect::Rect(const Point& pos, const Size& size) :
	topLeftCorner(pos),
	bottomRightCorner(pos + size),
	rectSize(size)
{ }

inline Rect::Rect(const Point& top_left_pos, const Point& bottom_right_pos) :
	topLeftCorner(top_left_pos),
	bottomRightCorner(bottom_right_pos),
	rectSize(bottom_right_pos - top_left_pos)
{ }

inline const Rect Rect::operator+(const Point p) const {
	return(Rect(topLeftCorner + p, rectSize));
}

inline void Rect::calculateBottomRightCorner() {
	bottomRightCorner = topLeftCorner + rectSize;
}


inline const Point& Rect::getTopLeft() const { 
	return topLeftCorner; 
}

inline const Size& Rect::getSize() const { 
	return rectSize; 
}

inline const Point& Rect::getBottomRight() const {
	return bottomRightCorner;
}

inline Rect::Rect(const Rect& rect) : 
	topLeftCorner(rect.getTopLeft()), 
	bottomRightCorner(rect.getBottomRight()),
	rectSize(rect.getSize()) 
{}	

inline const Point Rect::getTopRight() const {
	return(Point(bottomRightCorner.getX(), topLeftCorner.getY()));
}

inline const Point Rect::getBottomLeft() const {
	return(Point(topLeftCorner.getX(), bottomRightCorner.getY()));
}

inline void Rect::setTopLeft( const Point& point ) { 
	topLeftCorner = point; 
	calculateBottomRightCorner();
}
	
inline void Rect::setSize( const Size& size ) {
	rectSize = size;
	calculateBottomRightCorner();
}
	
inline const Uint16 Rect::getWidth() const { 
	return rectSize.getWidth();
}

inline const Uint16 Rect::getHeight() const { 
	return rectSize.getHeight(); 
}

inline const Sint16 Rect::getLeft() const {
	return topLeftCorner.getX();
}
		
inline const Sint16 Rect::getTop() const {
	return topLeftCorner.getY();
}
		
inline const Sint16 Rect::getBottom() const { 
	return bottomRightCorner.getY();
}
		
inline const Sint16 Rect::getRight()  const { 
	return bottomRightCorner.getX();
}

inline void Rect::setWidth(const Uint16 width) {
	rectSize.setWidth(width);
	bottomRightCorner.setX(topLeftCorner.getX() + (Sint16)width);
}
		
inline void Rect::setHeight(const Uint16 height) { 
	rectSize.setHeight(height);
	bottomRightCorner.setY(topLeftCorner.getY() + (Sint16)height);
}

inline void Rect::setLeft(const Sint16 left) { 
	topLeftCorner.setX(left); 
	bottomRightCorner.setX(left + (Sint16)(rectSize.getWidth()));
}

inline void Rect::setRight(const Sint16 right) {
	rectSize.setWidth(right - getLeft());
	bottomRightCorner.setX(right);
}

inline void Rect::setTop(const Sint16 top) { 
	topLeftCorner.setY(top); 
	bottomRightCorner.setY(top + (Sint16)(rectSize.getHeight()));

}
		
inline void Rect::setBottom(const Sint16 bottom) {
	rectSize.setHeight(bottom - getTop());
	bottomRightCorner.setY(bottom);
}
				
inline Rect& Rect::operator=(const Rect& rect) 
{	
	topLeftCorner = rect.getTopLeft();
	bottomRightCorner = rect.getBottomRight();
	rectSize = rect.getSize();
	return(*this);
}

inline const bool Rect::operator==(const Rect& rect) const 
{ 
	if( ( getTopLeft() != rect.getTopLeft() ) || ( getSize() != rect.getSize() ) ) {
		return false;
	}
	else {
		return true; 
	}
}

inline const bool Rect::operator!=(const Rect& rect) const {
	return (!(*this == rect));
}

inline const bool Rect::isRectInside(const Rect& rect) const {
	return( ( rect.getTopLeft() >= topLeftCorner ) && ( rect.getBottomRight() <= bottomRightCorner ) );
}

inline const bool Rect::isPixelInside(const Point& point) const {
	if( ( point >= topLeftCorner ) && ( point < bottomRightCorner ) )
		return(true);
	else return(false);
}
inline const bool Rect::isTopLeftCornerInside(const Point& point) const {
	return ( ( point >= topLeftCorner ) && (point < bottomRightCorner));
}

inline const bool Rect::isTopRightCornerInside(const Point& point) const {
	return( point >= (topLeftCorner + Point(1, 0)) && point < (bottomRightCorner + Point(1, 0)));
}

inline const bool Rect::isBottomRightCornerInside(const Point& point) const {
	return( point >= (topLeftCorner + Point(1, 1)) && point < (bottomRightCorner + Point(1, 1)));
}

inline const bool Rect::isBottomLeftCornerInside(const Point& point) const {
	return( point >= (topLeftCorner + Point(0, 1)) && point < (bottomRightCorner + Point(0, 1)));
}

inline const bool Rect::isTopLeftCornerInside(const Sint16 x, const Sint16 y) const {
	return isTopLeftCornerInside(Point(x, y));
}
inline const bool Rect::isTopRightCornerInside(const Sint16 x, const Sint16 y) const {
	return isTopRightCornerInside(Point(x, y));
}
inline const bool Rect::isBottomRightCornerInside(const Sint16 x, const Sint16 y) const {
	return isBottomRightCornerInside(Point(x, y));
}
inline const bool Rect::isBottomLeftCornerInside(const Sint16 x, const Sint16 y) const {
	return isBottomLeftCornerInside(Point(x, y));
}
inline const bool Rect::isPixelInside(const Sint16 x, const Sint16 y) const {
	return(isPixelInside(Point(x,y)));
}

inline const bool Rect::isTouched(const Rect& rect) const {
	if(rect.getLeft() >= getRight() || getLeft() >= rect.getRight() || rect.getTop() >= getBottom() || getTop() >= rect.getBottom()) {
		return false;
	} else {
		return true;
	}
}

#endif // _SDL_RECT_HPP

