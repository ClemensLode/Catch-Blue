#include "ui_multilinetext.hpp"

// boxed string!
UI_MultiLineText::UI_MultiLineText(SDL_Object* mlt_parent, 
				const Rect& mlt_pos,
				const std::string& ml_text, 
				WindowColors* window_colors):
	SDL_Object(mlt_parent, mlt_pos),
	text(ml_text),
	eText(NULL),
	windowColors(window_colors)
{
	reloadOriginalSize();
//	setDrawType(TRANSPARENT_OBJECT);
}

UI_MultiLineText::UI_MultiLineText(SDL_Object* mlt_parent, 
				const Rect& mlt_pos,
				Text* ml_text, 
				WindowColors* window_colors):
	SDL_Object(mlt_parent, mlt_pos),
	text(""),
	eText(ml_text),
	windowColors(window_colors)
{
	reloadOriginalSize();
//	setDrawType(TRANSPARENT_OBJECT);
}

UI_MultiLineText::~UI_MultiLineText()
{}

void UI_MultiLineText::process() {
	SDL_Object::process();
}

void UI_MultiLineText::reloadOriginalSize() {
	Point p = Point(0,3);
	
	const std::string& ml_text = eText?eText->toString():text;
	int width = 0;
	int index = 0;
	int old_index = 0;
	linebreaks.clear();
	linebreaks.push_back(std::pair<int, int>(0, 0));
	for(unsigned int i = 0; i < ml_text.length(); i++) {
		int new_index = ml_text.find_first_of(" ", old_index);
		Size text_size = windowColors->getFont()->getTextExtent(ml_text.substr(index, new_index - index));
		if(text_size.getWidth() > getWidth()) {
			p.setY(p.getY() + text_size.getHeight() + 1);
			index = old_index;
			linebreaks.push_back(std::pair<int, int>(index, p.getY()));
		} else {
			old_index = new_index;
		}
	}
	setHeight(p.getY());
}


void UI_MultiLineText::draw(DC* dc) const {
	dc->setPen(*(windowColors->getBorderPen()));
	dc->setBrush(*(windowColors->getBackgroundBrush()));
	dc->DrawRectangle(Rect(Point(0,0), Size(6 + getWidth(), 6 + getHeight())));

	dc->setFont(*(windowColors->getFont()));
	dc->setTextForeground(*(windowColors->getTextColor()));

	Point p = Point(0,3);
	
	const std::string& ml_text = eText?eText->toString():text;

	for(std::vector<std::pair<int, int> >::const_iterator i = linebreaks.begin(); i != linebreaks.end(); ) {
		std::vector<std::pair<int, int> >::const_iterator j = i;
		i++;
		int index_end = i == linebreaks.end()?std::string::npos:i->first;
		p.setY(j->second);
		dc->DrawText(ml_text.substr(j->first, i->first - j->first), p);
	}


	SDL_Object::draw(dc);
}