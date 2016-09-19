#ifndef _UI_MENUENTRY_HPP
#define _UI_MENUENTRY_HPP

#include "ui_button.hpp"

class UI_MenuEntry : public UI_Button
{
	public:
		UI_MenuEntry(SDL_Object* menuentry_parent, const Rect& rect, const ButtonColors* button_colors, const Text* unit);
		UI_MenuEntry(SDL_Object* menuentry_parent, const Rect& rect, const ButtonColors* button_colors, const std::string& unit);
//		UI_MenuEntry(SDL_Object* menuentry_parent, const Rect& rect, const ButtonColors* button_colors); //TODO bitmaps...
		~UI_MenuEntry();
		void process();
		void draw(DC* dc) const;
	private:
		UI_MenuEntry& operator=(const UI_MenuEntry& object);
		UI_MenuEntry(const UI_MenuEntry& object);
};

#endif // _UI_MENUENTRY_HPP

