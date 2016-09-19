#ifndef _SDL_BRUSH_HPP
#define _SDL_BRUSH_HPP

#include "color.hpp"

enum eBrushStyle
{
	TRANSPARENT_BRUSH_STYLE,
	SOLID_BRUSH_STYLE,
	BDIAGONAL_HATCH_BRUSH_STYLE,
	CROSSDIAG_HATCH_BRUSH_STYLE,
	FDIAGONAL_HATCH_BRUSH_STYLE,
	CROSS_HATCH_BRUSH_STYLE,
	HORIZONTAL_HATCH_BRUSH_STYLE,
	VERTICAL_HATCH_BRUSH_STYLE,
	STIPPLE_BRUSH_STYLE,
	STIPPLE_MASK_OPAQUE_BRUSH_STYLE,

	MAX_BRUSH_STYLES
};

class DC_API Brush : public Resource
{
	public:
		Brush(const Color brush_color, const eBrushStyle brush_style = SOLID_BRUSH_STYLE);
		Brush();
		~Brush();
	
		const eBrushStyle& getStyle() const;
		const Color& getColor() const;
//		void updateColor(SDL_Surface* surface);
	private:
		Color color;
		eBrushStyle style;
};

inline const eBrushStyle& Brush::getStyle() const {
	return style;
}

inline const Color& Brush::getColor() const {
	return color;
}

#endif // _SDL_BRUSH_HPP
