#include "ui_menuentry.hpp"

UI_MenuEntry::UI_MenuEntry(SDL_Object* menuentry_parent, 
						   const Rect& rect, 
						   const ButtonColors* button_colors, 
						   const Text* unit) :
	UI_Button(menuentry_parent, rect, Size(4, 2), button_colors, NULL, unit, PRESS_BUTTON_MODE, DO_NOT_ADJUST, AUTO_HEIGHT_CONST_WIDTH)
{ }

UI_MenuEntry::UI_MenuEntry(SDL_Object* menuentry_parent, 
						   const Rect& rect, 
						   const ButtonColors* button_colors, 
						   const std::string& unit) :
	UI_Button(menuentry_parent, rect, Size(4, 2), button_colors, unit, NULL, PRESS_BUTTON_MODE, DO_NOT_ADJUST, AUTO_HEIGHT_CONST_WIDTH)
{}

//UI_MenuEntry::UI_MenuEntry(SDL_Object* menuentry_parent, const Rect rect, const eButtonColorsType button) :
//	UI_Button(menuentry_parent, rect, Size(4, 2), button, NULL_BITMAP, PRESS_BUTTON_MODE, NULL_STRING"), // TODO
//	color(NULL_COLOR)
//{}

UI_MenuEntry::~UI_MenuEntry()
{ }

void UI_MenuEntry::process()
{
	if(!isShown()) {
		return;
	}

	UI_Button::process();
}

void UI_MenuEntry::draw(DC* dc) const
{
	UI_Button::draw(dc);
}
