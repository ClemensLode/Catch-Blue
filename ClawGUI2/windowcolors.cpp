#include "windowcolors.hpp"

WindowColors::WindowColors(Font* font, Color* text_color, Brush* background_brush, Pen* border_pen, Pen* outer_border_pen, Pen* highlighted_border_pen):
	font(font),
	textColor(text_color),
	backgroundBrush(background_brush),
	borderPen(border_pen),
	outerBorderPen(outer_border_pen),
	highlightedBorderPen(highlighted_border_pen)
{
	// TODO register, unregister
}

WindowColors::~WindowColors()
{}

