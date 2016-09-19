#include "ui_numberfield.hpp"

// TODO: ui_objects arrangen! es nur in button zu benutzen bringt wenig...


// std::string&
	UI_NumberField::UI_NumberField(SDL_Object* numberfield_parent, 
		const Rect& rect, 
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
		const unsigned int number_steps, 
		const unsigned int num, 
		const bool shift_right, 
		const eFieldType field_type) :
	UI_Object(numberfield_parent, rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	fieldType(field_type),
	addButton(NULL),
	subButton(NULL),
	text(NULL),
	numberText(NULL),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps),
	numberHasChanged(false),
	shiftRight(shift_right)
{
	addButton = new UI_Button(this, Rect(Point(140, 1), Size(8, 8)), Size(0, 0), add_button_colors, NULL, NULL, PRESS_BUTTON_MODE); // Point(gui.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1) INCREASE_BITMAP, 
	subButton = new UI_Button(this, Rect(Point(150, 1), Size(8, 8)), Size(0, 0), sub_button_colors, NULL, NULL, PRESS_BUTTON_MODE); // SUB_BITMAP, MODIFY_BUTTON
	text = new UI_StaticText(this, Rect(Point(shift_right?10:0, 0), Size(0, 0)), Size(0, 0),  txt, text_color, font, DO_NOT_ADJUST);
	numberText = new UI_StaticText(this, Rect(Point(110, 0), Size(0, 0)), Size(0, 0), "0", text_color, font, DO_NOT_ADJUST);

	if(tool_tip != NULL) {
		this->updateToolTip(tool_tip);
	}
	updateNumber(num);
	reloadOriginalSize();
}

UI_NumberField::UI_NumberField(SDL_Object* numberfield_parent, 
							   const Rect& rect, 
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
							   const unsigned int number_steps, 
							   const unsigned int num, 
							   const bool shift_right, 
							   const eFieldType field_type) :
	UI_Object(numberfield_parent, rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	fieldType(field_type),
	addButton(NULL),
	subButton(NULL),
	text(NULL),
	numberText(NULL),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps),
	numberHasChanged(false),
	shiftRight(shift_right)
{
	addButton = new UI_Button(this, Rect(Point(140, 1), Size(8, 8)), Size(0, 0), add_button_colors, NULL, NULL, PRESS_BUTTON_MODE); // Point(gui.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1) INCREASE_BITMAP, 
	subButton = new UI_Button(this, Rect(Point(150, 1), Size(8, 8)), Size(0, 0), sub_button_colors, NULL, NULL, PRESS_BUTTON_MODE); // SUB_BITMAP, MODIFY_BUTTON
	text = new UI_StaticText(this, Rect(Point(shift_right?10:0, 0), Size(0, 0)), Size(0, 0), txt, text_color, font, DO_NOT_ADJUST);
	numberText = new UI_StaticText(this, Rect(Point(110, 0), Size(0, 0)), Size(0, 0), "0", text_color, font, DO_NOT_ADJUST);

	if(tool_tip != NULL) {
		this->updateToolTip(tool_tip);
	}
	updateNumber(num);
	reloadOriginalSize();
}

UI_NumberField::~UI_NumberField()
{
	delete addButton;
	delete subButton;
	delete text;
	delete numberText;
}

UI_Object* UI_NumberField::checkToolTip(const Point& mouse) const
{
	if( (!isShown()) || 
		( (!addButton->getAbsoluteRect().isTopLeftCornerInside(mouse)) && 
		  (!subButton->getAbsoluteRect().isTopLeftCornerInside(mouse)) && 
		  ((!text)||(!text->getAbsoluteRect().isTopLeftCornerInside(mouse))) && 
		  (!numberText->getAbsoluteRect().isTopLeftCornerInside(mouse)))) {
		return(NULL);
	}
	return((UI_Object*)this);
}

void UI_NumberField::updateText(const std::string& txt) {
	text->updateText(txt);
}

void UI_NumberField::updateText(const Text* txt) {
	text->updateText(txt);
}

void UI_NumberField::reloadOriginalSize()
{
	/*setOriginalSize(Size(gui.lookUpButtonWidth(LARGE_BUTTON_WIDTH), text->getTextSize().getHeight()));
		
	addButton->setOriginalPosition(Point(gui.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1));
	subButton->setOriginalPosition(Point(gui.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1));
	numberText->setOriginalPosition(Point(addButton->getRelativeLeftBound() - numberText->getTextSize().getWidth() - 5, 0));*/
	//TODO
	UI_Object::reloadOriginalSize();
}


void UI_NumberField::updateNumber(const unsigned int num)
{
	if(number == num) {
		return;
	}
	number = num;
	numberHasChanged = true;
}

const SDL_Object* UI_NumberField::checkHighlight(const Point& mouse) {
	if(text) {
		text->doHighlight(getAbsoluteRect().isTopLeftCornerInside(mouse));
	}
	numberText->doHighlight(getAbsoluteRect().isTopLeftCornerInside(mouse));

	return UI_Object::checkHighlight(mouse);
}

void UI_NumberField::process()
{
	if(!isShown()) {
		return;
	}

	UI_Object::process();

	if(numberHasChanged)
	{
		numberHasChanged=false;
		std::ostringstream os;
		os.str("");
		switch(fieldType)
		{
			case NORMAL_NUMBER_TYPE:os << number; break;
			case PERCENT_NUMBER_TYPE:os << number << "%"; break;
			//case TIME_NUMBER_TYPE:os << formatTime(number, efConfiguration.getGameSpeed());break;
			//case STRING_NUMBER_TYPE: os << gui.lookUpResource((eString)(number)); TODO
			default:break;
		}
		numberText->updateText(os.str());
		numberText->setOriginalPosition(Point(addButton->getRelativeLeftBound() - numberText->getTextSize().getWidth() - 5, 0));
	//	setNeedRedrawMoved();
	//	adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, numberText->getTextSize()); // TODO
	}
	
	if(subClicked())
	{
		if(number > min+(signed int)steps) {
			number-=steps;
		} else {
			number = min;
		}
		updateNumber(number);
	}

	if(addClicked())
	{
		if(number < max-(signed int)steps) {
			number += steps;
		} else {
			number = max;
		}
		updateNumber(number);
	}

	if(subRightClicked())
	{
		if(number > min + 10*(signed int)steps) {
			number -= 10*steps;
		} else {
			number = min;
		}
		updateNumber(number);
	}

	if(addRightClicked())
	{
		if((number < max - 10 * (signed int)steps) && (max > 10 * steps)) {
			number += 10*steps;
		} else {
			number = max;
		}
		updateNumber(number);
	}


//	if(/*(numberText->checkForNeedRedraw())||((text)&&(text->checkForNeedRedraw())))||*/(addButton->checkForNeedRedraw())||(subButton->checkForNeedRedraw()))
//	if(checkForNeedRedraw())
//		setNeedRedrawMoved(); TODO
}

void UI_NumberField::draw(DC* dc) const
{
	UI_Object::draw(dc);
	
	//if(shiftRight) {
	//	dc->DrawBitmap(gui.lookUpBitmap(LIST_BITMAP), Point());
	//} TODO
	
/*	dc->SetBrush(*gui.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(*gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	Rect edge;
	edge.SetTopLeft(getAbsolutePosition()+Size(115,0));
	edge.SetWidth(58);
	edge.SetHeight(12);
	dc->DrawRoundedRectangle(edge, 2);*/
}

