#include "brush.hpp"

Brush::Brush(const Color brush_color, const eBrushStyle brush_style):
	color(brush_color),
	style(brush_style)
{}

Brush::Brush():
	color(Color()),
	style(TRANSPARENT_BRUSH_STYLE)
{}

Brush::~Brush()
{}

/*void Brush::updateColor(SDL_Surface* surface)
{
	color.updateColor(surface);
}*/

