#ifndef _UI_RADIO_HPP
#define _UI_RADIO_HPP

#include "ui_group.hpp"
#include "ui_button.hpp"

#define RADIO_MAX_BUTTONS 100

class UI_Radio : public UI_Group
{
	public:
		UI_Radio(SDL_Object* radio_parent,
				WindowColors* window_colors,
				Rect initial_rect = Rect(0,0,0,0), 
				Size distance_bottom_right = Size(0,0), 
				const eGroupType group_type = ONE_COLOUMN_GROUP, 
				const bool draw_background = false, 
				const ePositionMode position_mode = DO_NOT_ADJUST, 
				Text* radio_title = NULL);
		~UI_Radio();
		void addButton(UI_Button* button, const unsigned int button_id);
		void removeButton(const unsigned int button_id);
// TODO statictext für Titel...
// eigenes Titelobjekt?
		const signed int getMarked() const;

		void leftButtonPressed(UI_Button* button);
		void leftButtonReleased(UI_Button* button); // allow release?		
//		void rightButtonPressed(UI_Button* button);
//		void rightButtonReleased(UI_Button* button); // allow release?		

		void process();
		void draw(DC* dc) const;
		
		void forceUnpressAll();
		void updateIds();
	private:
		signed int markedItem;
		bool reorder;
		UI_Button* buttonId[RADIO_MAX_BUTTONS];
		UI_Button* buttonWasPressed;

		UI_Radio& operator=(const UI_Radio& object);
		UI_Radio(const UI_Radio& object);
};

inline const signed int UI_Radio::getMarked() const {
	return(markedItem);
}

inline void UI_Radio::leftButtonPressed(UI_Button* button) { // Unpress all except one
	buttonWasPressed = button;
}

inline void UI_Radio::leftButtonReleased(UI_Button* button) { // allow release?
	buttonWasPressed = button;
}

/*
inline void UI_Radio::rightButtonPressed(UI_Button* button) {
//	leftButtonPressed(button);
}

inline void UI_Radio::rightButtonReleased(UI_Button* button) { // allow release?
//	leftButtonReleased(button);
}*/

#endif

