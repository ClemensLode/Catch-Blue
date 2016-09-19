#ifndef _SDL_SIZE_HPP
#define _SDL_SIZE_HPP

#include <string>
#include <iostream>
#include <SDL.h>
#include <assert.h>

#define DC_API
/*#ifdef DC_EXPORTS
    #define DC_API __declspec(dllexport)
#else
    #define DC_API __declspec(dllimport)
#endif*/

class DC_API Size
{
	public:

		Size();
		Size(const Size& size);
		Size(const Uint16 width, const Uint16 height);
		Size& operator=(const Size& size);
		const bool operator==(const Size& size) const;
		const bool operator<(const Size& size) const;
		const bool operator!=(const Size& size) const;
		const Size operator+(const Size& size) const;
		const Size operator-(const Size& size) const;
		const Size operator/(const Uint16 d) const;
		void setWidth(const Uint16 width);
		void setHeight(const Uint16 height);
		const Uint16 getWidth() const;
		const Uint16 getHeight() const;
		void move(const Size& startSize, const Size& targetSize);
		static const bool mv(Uint16& x, const Uint16 sx, const Uint16 tx);

		const std::string toString() const;
	private:
		Uint16 width;
		Uint16 height;

};

inline Size::Size() : width(0), height(0)
{}

inline Size::Size(const Size& size) {
	setWidth(size.width);
	setHeight(size.height);
}

inline Size::Size(const Uint16 width, const Uint16 height) {
	setWidth(width);
	setHeight(height);
}

inline const Size Size::operator-(const Size& size) const {
	return Size(width - size.width, height - size.height);
}

inline Size& Size::operator=(const Size& size)
{
	setWidth(size.width);
	setHeight(size.height);
	return(*this);
}

inline const bool Size::operator==(const Size& size) const {
	return ( (width == size.width) && (height == size.height) );
}

inline const bool Size::operator<(const Size& size) const {
	return ( (width < size.width) && (height < size.height) );
}	

inline const bool Size::operator!=(const Size& size) const {
	return ( !(*this == size));
}

inline const Size Size::operator+(const Size& size) const {
	return Size(width + size.width, height + size.height);
}
		
inline const Size Size::operator/(const Uint16 d) const {
	return(Size(width/d, height/d));
}


inline const Uint16 Size::getWidth() const {
	return width;
}
																				
inline const Uint16 Size::getHeight() const {
	return height;
}

#endif // _SDL_SIZE_HPP

