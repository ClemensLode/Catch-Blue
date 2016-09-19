#ifndef _UI_TOOLTIP_HPP
#define _UI_TOOLTIP_HPP

#include "ui_multilinetext.hpp"
#include "font.hpp"
#include "color.hpp"
#include "objectsize.hpp"

class UI_ToolTip : public UI_MultiLineText
{
	public:
		UI_ToolTip(SDL_Object* parent_object, const std::string& tooltip_text, WindowColors* window_colors, ObjectSize* object_size);
		UI_ToolTip(SDL_Object* parent_object, Text* tooltip_text, WindowColors* window_colors, ObjectSize* object_size);
		~UI_ToolTip();
	
		void reloadOriginalSize();
		void object_info();
		void draw(DC* dc) const;
		void process();
	private:
		Rect boxSize;
		ObjectSize* objectSize;
	
		UI_ToolTip& operator=(const UI_ToolTip& object);
		UI_ToolTip(const UI_ToolTip& object);
};

#endif

