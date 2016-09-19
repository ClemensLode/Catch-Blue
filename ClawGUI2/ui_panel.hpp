#ifndef _PANEL_HPP
#define _PANEL_HPP

#include <sdl_object.hpp>

class UI_Panel : public SDL_Object {
	public:
		UI_Panel(SDL_Object* radio_parent, 
				Rect initial_rect, 
				Size bottom_right_distance, 
				WindowColors* window_colors,
				Text* title,
				const ePositionMode position_mode, 
				);
		~UI_Panel();
};

#endif