#ifndef _UI_NUMBERFIELD_HPP
#define _UI_NUMBERFIELD_HPP

#include "ui_button.hpp"
#include "ui_group.hpp"

enum eFieldType
{
	NORMAL_NUMBER_TYPE,
	PERCENT_NUMBER_TYPE,
	TIME_NUMBER_TYPE
	//STRING_NUMBER_TYPE TODO
};

class UI_NumberField : public UI_Object
{
	public:
		UI_NumberField(SDL_Object* numberfield_parent, 
				const Rect& field_rect, 
				const Size distance_bottom_right, 
				const ePositionMode position_mode, 
				const unsigned int number_min, 
				const unsigned int number_max, 
				const std::string& txt, 
				const Text* tool_tip, 
				const Color* text_color,
				const Font* font,
				const ButtonColors* add_button_colors,
				const ButtonColors* sub_button_colors,
				const unsigned int number_steps = 1, 
				const unsigned int num = 0, 
				const bool shift_right = false, 
				const eFieldType field_type = NORMAL_NUMBER_TYPE);
		UI_NumberField(SDL_Object* numberfield_parent, 
				const Rect& field_rect, 
				const Size distance_bottom_right, 
				const ePositionMode position_mode, 
				const unsigned int number_min, 
				const unsigned int number_max, 
				const Text* txt, 
				const Text* tool_tip, 
				const Color* text_color,
				const Font* font,
				const ButtonColors* add_button_colors,
				const ButtonColors* sub_button_colors,
				const unsigned int number_steps = 1, 
				const unsigned int num = 0, 
				const bool shift_right = false, 
				const eFieldType field_type = NORMAL_NUMBER_TYPE);
		~UI_NumberField();
		UI_Object* checkToolTip(const Point& mouse) const;
		
		const bool addClicked() const;
		const bool subClicked() const;
		const bool addRightClicked() const;
		const bool subRightClicked() const;

		void reloadOriginalSize();
		
		void updateText(const std::string& txt);
		void updateText(const Text* txt);

		const unsigned int getNumber() const;
		void updateNumber(const unsigned int num);
		void process();
		void draw(DC* dc) const;
		const bool hasNumberChanged() const;

		const SDL_Object* checkHighlight(const Point& mouse);
	private:
		eFieldType fieldType;
		UI_Button* addButton;
		UI_Button* subButton;
		UI_StaticText* text;
		UI_StaticText* numberText;
		unsigned int number;
		unsigned int min;
		unsigned int max;
		unsigned int steps;
		bool numberHasChanged;
		bool shiftRight;

		UI_NumberField& operator=(const UI_NumberField& object);
		UI_NumberField(const UI_NumberField& object);
};

inline const bool UI_NumberField::hasNumberChanged() const {
	return(numberHasChanged);
}

inline const unsigned int UI_NumberField::getNumber() const {
	return(number);
}

inline const bool UI_NumberField::addClicked() const {
	return(addButton->isLeftClicked());
}

inline const bool UI_NumberField::subClicked() const {
	return(subButton->isLeftClicked());
}

inline const bool UI_NumberField::addRightClicked() const {
	return(addButton->isRightClicked());
}

inline const bool UI_NumberField::subRightClicked() const {
	return(subButton->isRightClicked());
}

#endif

