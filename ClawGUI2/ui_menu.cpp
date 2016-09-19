#include "ui_menu.hpp"

UI_Menu::UI_Menu(SDL_Object* menu_parent, 
				const Rect& rect, 
				const Size distance_bottom_right, 
				const ePositionMode position_mode, 
				const bool choose_menu, 
				ObjectSize* object_size, 
				ButtonColors* button_colors,
				WindowColors* window_colors,
				std::vector<Text*> strings,
				eMenuType menu_type) :
	UI_Object(menu_parent, rect, distance_bottom_right, position_mode),
	menuEntries(),
	menuLevel(0),
	pressedItem(-1),
	markedItem(-1),
	height(0),
	chooseMenu(choose_menu),
	p1(),
	p2(),
	menuChanged(false),
	objectSize(object_size),
	buttonColors(button_colors),
	windowColors(window_colors),
	menuType(menu_type)
{
	if(strings.size() > 0)
	{
		for(std::vector<Text*>::const_iterator i = strings.begin(); i != strings.end(); i++) {
			UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), button_colors, *i);
			
			menuEntries.push_back(entry);
		}
		updateItemSizes(objectSize->getWidth());
		updateItemPositions();
	}
	Hide();
}

UI_Menu::~UI_Menu()
{
	for(std::list<UI_MenuEntry*>::const_iterator m = menuEntries.begin(); m != menuEntries.end(); ++m) {
		delete *m;	
	}
}

void UI_Menu::reloadOriginalSize() {

	updateItemSizes(objectSize->getWidth());

	updateItemPositions();
	UI_Object::reloadOriginalSize();	 //?
}

void UI_Menu::updateItemSizes(const Uint16 width)
{
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m) {
		(*m)->setOriginalSize(Size(width, 0));
	}
}

void UI_Menu::updateItemPositions()
{
	unsigned int i = 0;
	height = 0;
//	Size s;
//	if(positionMode == TOTAL_CENTERED) // TODO
//		s = 
		
	switch(menuType)
	{
		case CUSTOM_MENU:break;
		case ONE_COLOUMN_MENU:
		for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m)
			if((*m)->isShown())
			{
				Uint16 item_height;
				if((*m)->isBitmapButton()) {
					item_height = (*m)->getHeight();
				} else {
					item_height = (*m)->getTextHeight();
				}

				(*m)->setOriginalPosition(Point(0, i*(item_height+(*m)->getDistanceBottomRight().getHeight())) + Size(3,3));
				i++;
				height++;
			}
		break;			 
		case TWO_COLOUMNS_MENU:
		for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m)
			if((*m)->isShown())
			{
				Uint16 item_height;
				if((*m)->isBitmapButton()) {
					item_height = (*m)->getHeight();
				} else {
					item_height = (*m)->getTextHeight();
				}
				(*m)->setOriginalPosition(Point((i%2) * ((*m)->getWidth() + (*m)->getDistanceBottomRight().getWidth()), (i/2)*(item_height+(*m)->getDistanceBottomRight().getHeight())) + Size(3,3));
				i++;
				if(i%2==0) {
					height++;
				}
			}
		break;
		case HORIZONTAL_MENU:
		for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m)
			if((*m)->isShown())
			{
				(*m)->setOriginalPosition(Point(i * ((*m)->getWidth() + (*m)->getDistanceBottomRight().getWidth()), 0)  + Size(3,3));
				i++;
			}
		height = 1;
		break;	
		default:break;
	}

	Uint16 max_width = 0;
	Uint16 max_height = 0;
	UI_Object* tmp = (UI_Object*)getChildren();
	if(tmp == NULL) {
		return;
	}
	do
	{
		if(tmp->isShown())
		{
			Rect r = tmp->getTargetRect();
			if((r.getRight() > 0) && ((Sint16)max_width < r.getRight())) {
				max_width = r.getRight();
			}
			if((r.getBottom() > 0) && ((Sint16)max_height < r.getBottom())) {
				max_height = r.getBottom();
			}
		}
		tmp = (UI_Object*)(tmp->getNextBrother());
	}

	while(tmp!=(UI_Object*)getChildren());
	
	Size s = Size(max_width+7, max_height+7);
//	setOriginalSize(s);
	adjustRelativeRect(Rect(getTargetPosition(), s));

}

const bool UI_Menu::isOpen() const
{
	if(menuLevel == 0) {
		return(false);
	} else {
		return(true);
	}
}

const signed int UI_Menu::getPressedItem()
{
	signed int p = pressedItem;
	pressedItem = -1;
	return(p);
}

const Uint16 UI_Menu::getHeight() const
{
	if(!isOpen()) {
		return(0);
	} else {
		return(height);
	}
}


void UI_Menu::open()
{
	if(menuLevel == 0) {
		menuLevel = 1;
	} else if(menuLevel == 1) {
		menuLevel = 0;
	} else {
		menuLevel = 1;
	}

	setMenuHasChanged();


	if(menuLevel) {
		Show();
	} else {
		Hide();
	}

	if(isShown())
	{
		UI_Object* tmp = (UI_Object*)getChildren();  
		if (tmp) {
			do {
				((UI_Button*)tmp)->resetGradient();
				tmp = (UI_Object*)(tmp->getNextBrother());
			} while (tmp != (UI_Object*)getChildren());
		}
	}
}

void UI_Menu::close()
{
	if(menuLevel!=0) {
		setMenuHasChanged();
	}
	menuLevel=0;
	Hide();
}

void UI_Menu::setMenuLevel(const unsigned int menu_level)
{
	if(menuLevel == menu_level) {
		return;
	}
	menuLevel = menu_level;
	setMenuHasChanged();
}

void UI_Menu::setMenuHasChanged(const bool has_changed)
{
	if(has_changed == menuChanged) {
		return;
	}

	menuChanged = has_changed;
	makePufferInvalid();
}

void UI_Menu::process()
{
	markedItem = -1;
	if(!isShown()) {
		return;
	}

	setMenuHasChanged(false);
	
	UI_Object::process();
	
	p1 = Point(9999, 9999);
	p2 = Point(0, 0);

	unsigned int i = 0;

	for(std::list<UI_MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(!(*m)->isShown())
		{
			(*m)->setPosition(Point(0, 0));
			(*m)->frameReset();
		}
		else
		{
			if((*m)->getAbsoluteLeftBound() < p1.getX()) {
				p1.setX((*m)->getAbsoluteLeftBound());
			}

			if((*m)->getAbsoluteUpperBound() < p1.getY()) {
				p1.setY((*m)->getAbsoluteUpperBound());
			}

			if((*m)->getAbsoluteRightBound() > p2.getX()) {
				p2.setX((*m)->getAbsoluteRightBound());
			}

			if((*m)->getAbsoluteLowerBound() > p2.getY()) {
				p2.setY((*m)->getAbsoluteLowerBound());
			}

			if ((*m)->isLeftClicked()) {
				pressedItem = i;
			}

			if ((*m)->isCurrentlyHighlighted()) {
				markedItem = i;
			}
		}
		++i;
	}

	if((pressedItem >= 0) && (chooseMenu)) {
		close();
	}
}

void UI_Menu::draw(DC* dc) const
{
	assert(dc);

	if(p1 > p2) {
		return;
	}
	Rect edge = Rect(p1 - Size(3,3), Size(p2.getX() - p1.getX(), 
		p2.getY() - p1.getY()) + Size(6,6) );

	dc->setBrush(*(windowColors->getBackgroundBrush())); //	*gui.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//	dc->setBrush(*gui.lookUpBrush(TRANSPARENT_BRUSH));
	dc->setPen(*(windowColors->getHighlightedBorderPen()));//*gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->DrawRoundedRectangle(edge, 4);

	UI_Object::draw(dc);
}


void UI_Menu::forcePressItem(const unsigned int number)
{
	assert(number < menuEntries.size());

	pressedItem = number;
	if(chooseMenu) {
		close();
	}
/*	unsigned int n = 0;
	for(std::list<UI_MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		if((*m)->isShown())
		{
			if(n == number)
			{
				(*m)->forcePress();
				return;
			}
			n++;
		}*/
}


