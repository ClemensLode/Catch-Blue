#ifndef _SDL_PEN_HPP
#define _SDL_PEN_HPP

#include <SDL.h>
#include <resource.hpp>
#include "color.hpp"

enum ePenStyle
{
	TRANSPARENT_PEN_STYLE,
	SOLID_PEN_STYLE,
	DOT_PEN_STYLE,
	LONG_DASH_PEN_STYLE,
	SHORT_DASH_PEN_STYLE,
	DOT_DASH_PEN_STYLE,
	MAX_PEN_STYLE
};

// #define PEN_MAX_WIDTH 1000 TODO

class DC_API Pen : public Resource
{
	public:
		Pen(const Color& pen_color, const Uint16 pen_width = 1, const ePenStyle pen_style = SOLID_PEN_STYLE);
		Pen();
		~Pen();
		const ePenStyle getStyle() const;
		const Uint16 getWidth() const;
		const Color& getColor() const;
		void setColor(const Color& pen_color);
//		void updateColor(SDL_Surface* surface);
	private:
		Uint16 width;
		Color color;
		ePenStyle style;
};

inline void Pen::setColor(const Color& pen_color) {
	color = pen_color;
}

inline const ePenStyle Pen::getStyle() const {
	return style;
}

inline const Uint16 Pen::getWidth() const {
	return width;
}

inline const Color& Pen::getColor() const {
	return color;
}

#endif // _SDL_PEN_HPP

