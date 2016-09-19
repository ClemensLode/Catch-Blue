#include "ui_object.hpp"

UI_Object::UI_Object(SDL_Object* parent_object, const Rect& relative_rect, const Size& distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	SDL_Object(parent_object, relative_rect, distance_bottom_right, position_mode, auto_size),
	toolTipEString(NULL),
	toolTipString()
{
}

UI_Object::UI_Object(SDL_Object* parent_object, UI_Object* position_parent_object, const Rect& relative_rect, const Size& distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	SDL_Object(parent_object, position_parent_object, relative_rect, distance_bottom_right, position_mode, auto_size),
	toolTipEString(NULL),
	toolTipString()
{
}

UI_Object::~UI_Object()
{
}


const bool UI_Object::addKey(unsigned int key, unsigned int mod)
{
	return(false);
}


void UI_Object::process()
{
	SDL_Object::process();
}

// returns the object the mouse cursor is currently over and owns a tooltip
UI_Object* UI_Object::checkToolTip(const Point& mouse) const
{
	if(!isShown()) {
		return(NULL);
	}

	UI_Object* tmp = (UI_Object*)getChildren();  // process all children of gadget
	
	if(!tmp) {
		return(NULL); // return 0 as this is an object and no button!
	}
	UI_Object* result = NULL;
	do 
	{
		result = tmp->checkToolTip(mouse);
		if(result != NULL && !result->hasToolTip()) {
			result = NULL;
		}
		tmp = (UI_Object*)(tmp->getNextBrother());
	} while((tmp != (UI_Object*)getChildren())&&(result == NULL));
	
	return(result);
}




	
void UI_Object::draw(DC* dc) const {
	assert(dc);

	SDL_Object::draw(dc);
}
