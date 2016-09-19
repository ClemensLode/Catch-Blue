#include "pen.hpp"

Pen::Pen():
	width(0),
	color(Color()),
	style(TRANSPARENT_PEN_STYLE)
{ }

Pen::Pen(const Color& pen_color, const Uint16 pen_width, const ePenStyle pen_style):
	width(pen_width),
	color(pen_color),
	style(pen_style)
{ }

Pen::~Pen() {
}

/*void Pen::updateColor(SDL_Surface* surface)
{
	assert(surface);
	color.updateColor(surface);
}*/

