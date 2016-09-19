#ifndef _UI_TITLE_GROUP_HPP
#define _UI_TITLE_GROUP_HPP

#include "ui_title.hpp"

enum eGroupType
{
	CUSTOM_GROUP,
	ONE_COLOUMN_GROUP,
	TWO_COLOUMNS_GROUP,
	HORIZONTAL_GROUP,
	TWO_LINES_GROUP
};


// TODO eString erlauben!

// simplified window class, maybe inheritate UI_Window from UI_Group...
class UI_Group : public SDL_Object
{
	public:
		UI_Group(SDL_Object* group_parent, 
				const Rect& initial_rect, 
				const Size bottom_right_distance, 
				const eGroupType group_type, 
				WindowColors* window_colors,
				Text* title_text = NULL,
				const bool draw_background = false, 
				const ePositionMode position_mode = DO_NOT_ADJUST);
		~UI_Group();

		void draw(DC* dc) const;
		void process();
		void reloadOriginalSize();

		void alignWidth(const Uint16 width);
		void calculateBoxSize();
	private:
		UI_Title* title;

		unsigned int number;
		bool highlighted;
		eGroupType groupType;
		bool drawBackground;

		Pen* pen;
		Pen* pen_highlighted;
		Brush* brush;
	
		UI_Group(const UI_Group& object);
		UI_Group& operator=(const UI_Group& object);
};

#endif

