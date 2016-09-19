#ifndef _WINDOWCOLORS_HPP
#define _WINDOWCOLORS_HPP

#include <brush.hpp>
#include <color.hpp>
#include <pen.hpp>
#include <font.hpp>

class WindowColors
{
public:
	WindowColors(Font* font, Color* text_color, Brush* background_brush, Pen* border_pen, Pen* outer_border_pen, Pen* highlighted_border_pen);
	~WindowColors();

	const Font* getFont() const {return font;}
	const Color* getTextColor() const {return textColor;}
	const Brush* getBackgroundBrush() const {return backgroundBrush;}
	const Pen* getBorderPen() const {return borderPen;}
	const Pen* getOuterBorderPen() const {return outerBorderPen;}
	const Pen* getHighlightedBorderPen() const {return highlightedBorderPen;}

private:
	Font* font;
	Color* textColor;
	Brush* backgroundBrush;
	Pen* borderPen;
	Pen* highlightedBorderPen;
	Pen* outerBorderPen;
};

#endif