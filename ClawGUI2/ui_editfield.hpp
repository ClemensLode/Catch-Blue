#ifndef _UI_EDITFIELD_HPP
#define _UI_EDITFIELD_HPP

#include "ui_button.hpp"

// TODO: Editfield aus Button und Text bestehen lassen... wegen UI_Group (die Box) und wegen besserer Uebersicht (damits net immer ein extra STaticText braucht)

class UI_EditField : public UI_Object
{
	public:
		UI_EditField(SDL_Object* edit_parent, 
				const Rect& edit_rect, 
				const Size& edit_size, 
				const Font* st_font, 
			    const Color* text_color,
			    const ButtonColors* button_colors,
				const ePositionMode position_mode, 
				const Text* description_text, 
				const std::string& name_proposal);
		UI_EditField(SDL_Object* edit_parent, 
				const Rect& edit_rect, 
				const Size& edit_size, 
				const Font* st_font, 
			    const Color* text_color,
			    const ButtonColors* button_colors,
				const ePositionMode position_mode, 
				const std::string& description_text, 
				const std::string& name_proposal);
		~UI_EditField();

		void draw(DC* dc) const;
		void process();
		
		UI_Object* checkToolTip(const Point& mouse) const;
		const SDL_Object* checkHighlight(const Point& mouse);
	
		const bool addKey(unsigned int key, unsigned int mod);

		const UI_StaticText* getText() const;
		
		void addChar(char a);
		void removeCharBackspace();
		void removeCharDelete();
		void enterWasPressed();
		void escapeWasPressed();
		const bool done() const;
		const bool canceled() const;

		void resetData();

		void moveLeft();
		void moveRight();

	private:
		unsigned int position;
		unsigned int ani;
		bool pressedEnter;
		bool pressedEscape;

		UI_Button* editField;
		UI_StaticText* descriptionText;

		UI_EditField(const UI_EditField& object);
		UI_EditField& operator=(const UI_EditField& object);
};

inline const UI_StaticText* UI_EditField::getText() const {
	return(editField->getText());
}

inline void UI_EditField::enterWasPressed() {
	pressedEnter = true;
}

inline void UI_EditField::escapeWasPressed() {
	pressedEscape = true;
}

inline const bool UI_EditField::done() const {
	return(pressedEnter);
}

inline const bool UI_EditField::canceled() const {
	return(pressedEscape);
}

inline UI_Object* UI_EditField::checkToolTip(const Point& mouse) const {
	return(UI_Object::checkToolTip(mouse));
}


#endif

