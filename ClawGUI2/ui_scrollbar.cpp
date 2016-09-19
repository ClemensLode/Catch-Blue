#include "ui_scrollbar.hpp"

#define MAX_INTERNAL_SCROLLY 10000

UI_ScrollBar::UI_ScrollBar(SDL_Object* scroll_parent, 
						   const Sint16 start_y, 
						   const Uint16 max_height, 
						   const ButtonColors* up_button_colors,
						   const ButtonColors* down_button_colors,
						   const WindowColors* window_colors,
						   const bool scroll_hideable) :
	UI_Object(scroll_parent, Rect()),//scroll_parent->getWidth(), scroll_parent->getHeight())), // TODO
	firstItemY(0),
	lastItemY(0),
	startY(start_y),
	add(NULL),
	sub(NULL),
	hideable(scroll_hideable),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	targetScrollY(0),
	barHeight(0),
	clientHeight(0),
	clientTargetHeight(0),
	totalHeight(0),
	maxHeight(max_height),
	windowColors(window_colors)
{
	add = new UI_Button(this, Rect(getParent()->getWidth()-10, 0, 8, 8), Size(0,0), down_button_colors, NULL, NULL, PRESS_BUTTON_MODE);// SMALL_ARROW_DOWN_BITMAP, PRESS_BUTTON_MODE, NULL);
	sub = new UI_Button(this, Rect(getParent()->getWidth()-10, 0, 8, 8), Size(0,0), up_button_colors, NULL, NULL, PRESS_BUTTON_MODE);//ARROW_BUTTON, SMALL_ARROW_UP_BITMAP, PRESS_BUTTON_MODE, NULL);
}

UI_ScrollBar::~UI_ScrollBar()
{
	delete add;
	delete sub;
}



void UI_ScrollBar::checkBoundsOfChildren(const Sint16 upper_bound, const Sint16 lower_bound)
{
	UI_Object* tmp = (UI_Object*)getChildren();
	if (tmp) {
		do {
			if((tmp != add) && (tmp != sub))
			{
				if(((tmp->getAbsoluteUpperBound() < upper_bound) || (tmp->getAbsoluteLowerBound() > lower_bound)))
				{
					if(tmp->isClipped())
					{
						if((tmp->getAbsoluteLowerBound() > upper_bound) && (tmp->getAbsoluteUpperBound() < lower_bound))
						{
							tmp->Show();
							tmp->setClipRect(Rect(tmp->getAbsoluteLeftBound(), upper_bound, tmp->getWidth(), lower_bound - upper_bound));
						} else 
						{
							tmp->Hide();
							tmp->setClipRect(Rect());
						}
					}	
					else
						tmp->Hide();
				}
				else tmp->Show();
			}
			tmp = (UI_Object*)(tmp->getNextBrother());
		} while (tmp != (UI_Object*)getChildren());
	}
}

const SDL_Object* UI_ScrollBar::checkHighlight(const Point& mouse) {
	if(Rect(getParent()->getAbsolutePosition(), getParent()->getSize() + Size(8, 0)).isTopLeftCornerInside(mouse)) {
		add->Show();
		sub->Show();
	} else {
		add->Hide();
		sub->Hide();
	}
	return SDL_Object::checkHighlight(mouse);
}

// TODO: Klarer gliedern zwischen virtueller Hoehe und realer Hoehe!
void UI_ScrollBar::process() // process messages, continue animation etc.
{

	if(!isShown()) {
		return;
	}

	
	totalHeight = lastItemY - firstItemY + clientHeight; //? :-/

// totalHeight wird 0... ?

//	unsigned int oldBarHeight = barHeight;

	if(lastItemY == firstItemY) {
		barHeight = clientHeight;
	} else {
		barHeight = clientHeight * clientHeight / totalHeight;
	}
	
	if(barHeight > clientHeight) {
		barHeight = clientHeight;
	}
	
//	signed int oldScrollY = currentScrollY;
	currentScrollY = targetScrollY;
	if(currentScrollY < 0) {
		currentScrollY = 0;
	}

	if(currentScrollY > (signed int)clientHeight - (signed int)barHeight) {
		currentScrollY = (signed int)clientHeight - (signed int)barHeight;
	}

	Sint16 position = 0;
	if(clientHeight <= 0) {
		position = 0;
	} else {
		position = currentScrollY * (signed int)(totalHeight / clientHeight);
	}
	setPosition(Point(0, startY-position));
	add->setPosition(Point(getParent()->getWidth() -12, clientHeight + position + 6));
	sub->setPosition(Point(getParent()->getWidth() -12, position - 4));


	targetScrollY = currentScrollY;
	
	UI_Object::process();

//	if((currentScrollY != oldScrollY)||(barHeight != oldBarHeight)||(add->checkForNeedRedraw())||(sub->checkForNeedRedraw()))
//		setNeedRedrawMoved(); TODO

	if(add->isLeftClicked()) {
		moveDown();
	}
	if(sub->isLeftClicked()) {
		moveUp();
	}

	// TODO: nicht zeichnen falls Hoehe zu klein
}


#include <sstream>
void UI_ScrollBar::draw(DC* dc) const
{
	assert(dc);

	// TODO!
	dc->setBrush(*(windowColors->getBackgroundBrush()));
//	if(gui.isMouseInside(Rect(getParent()->getAbsolutePosition() + Point(getWidth(), startY+5) - Size(14, 0), Size(12, getHeight()+12)))) {
//		dc->setPen(*(windowsColors->getHighlightedBorderPen())));//*gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
//	} else { TODO
		dc->setPen(*(windowColors->getBorderPen()));
//	}
		
	dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Size(getParent()->getWidth()-13, startY+5) - Size(getAbsolutePosition().getX(), getAbsolutePosition().getY()), Size(10, clientHeight)));
	//dc->setBrush(*gui.lookUpBrush(CONTINUE_BUTTON_BRUSH)); TODO
	dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Point(getParent()->getWidth()-12, startY+currentScrollY+5) - Size(getAbsolutePosition().getX(), getAbsolutePosition().getY()), Size(8, barHeight)));
	UI_Object::draw(dc);
		
	/*Point p = Point(100, 300);
	dc->setTextForeground(DC::toSDL_Color(255, 20, 20));
	dc->setFont(gui.lookUpFont(LARGE_BOLD_FONT));
	dc->setBrush(Brush(Color(dc->getSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
	dc->DrawRectangle(Rect(p, Size(200, 20)));

	std::ostringstream os;
	os.str("");
	os << "cH : " << clientHeight << " / T : " << totalHeight << " / cSY : " << currentScrollY << "  [" << firstItemY << ":" << lastItemY << "]";
	dc->DrawText(os.str(), p + Size(20, 2));	*/
}

void UI_ScrollBar::setClientHeight(const Uint16 height) {
	assert(height >= 12);

	clientHeight = height - 12;
}

void UI_ScrollBar::setClientTargetHeight(const Uint16 height) {
	clientTargetHeight = height;
}

void UI_ScrollBar::moveUp() {
	targetScrollY -= clientHeight / 40;
}

void UI_ScrollBar::moveDown() {
	targetScrollY += clientHeight / 40;
}

// TODO: Wenn Scrollbereich ausserhalb des Fensters rutscht moechte das Programm verstaendlicherweise neumalen, da es nicht weiss, dass die entsprechenden Eintraege versteckt werden... :-/
void UI_ScrollBar::moveToTop() {
	if(currentScrollY==0)
		return;
	targetScrollY = 0;
}

void UI_ScrollBar::moveToBottom() {
	targetScrollY = 9999;
}
