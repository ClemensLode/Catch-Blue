#ifndef _UI_TITLE_HPP
#define _UI_TITLE_HPP

#include "ui_object.hpp"
#include "ui_statictext.hpp"
#include "windowcolors.hpp"

class UI_Title : public UI_Object {
	public:
		UI_Title(SDL_Object* parent_object, std::string& text, WindowColors* window_colors);
		UI_Title(SDL_Object* parent_object, Text* text, WindowColors* window_colors);
		~UI_Title();
		void draw(DC* dc) const;
	private:
		UI_StaticText* title;
		WindowColors* windowColors;
};

#endif