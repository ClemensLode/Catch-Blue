#include "ui_title.hpp"


UI_Title::UI_Title(SDL_Object* parent_object, std::string& text, WindowColors* window_colors):
	UI_Object(parent_object),
	windowColors(window_colors)
{
	title = new UI_StaticText(parent_object, Rect(0,0,0,0), Size(0,0), text, window_colors->getTextColor(), window_colors->getFont());
}

UI_Title::UI_Title(SDL_Object* parent_object, Text* text, WindowColors* window_colors):
	UI_Object(parent_object),
	windowColors(window_colors)
{
	title = new UI_StaticText(parent_object, Rect(0,0,0,0), Size(0,0), text, window_colors->getTextColor(), window_colors->getFont());
}

UI_Title::~UI_Title()
{
	delete title;
}

void UI_Title::draw(DC* dc) const {
	assert(dc);

//		dc->setBrush(*gui.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//		dc->DrawRoundedRectangle(getAbsolutePosition() - Size(4, 5), getSize(), 4);

	// Point(2, 3)  ?
	Size s = title->getTextSize();
	Rect titleRect = Rect(Point(0,0), s + Size(5,2));
	dc->setPen(*(windowColors->getBorderPen()));
		//gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->setBrush(*(windowColors->getBackgroundBrush()));
		//*gui.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	dc->DrawEdgedRoundedRectangle(titleRect, 2);

	UI_Object::draw(dc);
}
/*
void UI_Window::drawTitle(DC* dc) const
{
	assert(dc);
	if(titleString == NULL) {
		return;
	}
	//TODO
	dc->setBrush(windowColors->getBackgroundBrush());//(WINDOW_FOREGROUND_BRUSH));
	dc->setPen(windowColors->getBorderPen()); //(INNER_BORDER_HIGHLIGHT_PEN));
	dc->setFont(windowColors->getFont());//(SMALL_BOLD_FONT));
	dc->setTextForeground(windowColors->getTextColor());//(TITLE_COLOR));

	std::string text;

// TODO UI_StaticText einfuegen!
	if(titleParameter != "") {
		text = gui.lookUpFormattedString(titleString, titleParameter);
	} else {
		text = gui.lookUpResource(titleString);
	}
	
	Size s = dc->getTextExtent(text);
	Rect titleRect = Rect(Point(), s + Size(5, 0));
	
	dc->DrawRectangle(titleRect);
	titleRect.setTopLeft(titleRect.getTopLeft() + Point(2, 3));
	dc->DrawText(text, titleRect.getTopLeft());
}
*/