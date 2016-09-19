#include "ui_radio.hpp"

UI_Radio::UI_Radio(SDL_Object* radio_parent, 
				WindowColors* window_colors,
		Rect initial_rect, 
		Size bottom_right_distance, 
		const eGroupType group_type, 
		const bool draw_background, 
		const ePositionMode position_mode, 
		Text* radio_title) :
	UI_Group(radio_parent, initial_rect, bottom_right_distance, group_type, window_colors, radio_title, draw_background, position_mode),
	markedItem(-1),
	reorder(false),
	buttonWasPressed(NULL)
{ 

	for(unsigned int i = 0; i < RADIO_MAX_BUTTONS; i++) 
		buttonId[i]=NULL;
}

UI_Radio::~UI_Radio()
{ }

void UI_Radio::addButton(UI_Button* button, const unsigned int id)
{
	assert(button);
	assert(id >= 0 && id < RADIO_MAX_BUTTONS);
	assert(buttonId[id]);

	button->setParent(this);
	button->radio = this;
	buttonId[id] = button;

	calculateBoxSize();
	reorder=true;
}

void UI_Radio::removeButton(const unsigned int button_id)
{
	assert(button_id >= 0 && button_id < RADIO_MAX_BUTTONS);
	assert(buttonId[button_id]);

	buttonId[button_id]->setParent(NULL);
	buttonId[button_id]->radio = NULL;
	buttonId[button_id] = NULL;

	calculateBoxSize();
	reorder=true;
}

void UI_Radio::updateIds()
{
	for(unsigned int i = 0; i < RADIO_MAX_BUTTONS;i++)
	{
		while((i<RADIO_MAX_BUTTONS)&&(buttonId[i]!=NULL)) 
			i++;
		if(i<RADIO_MAX_BUTTONS)
		{
			for(int j = i;j<9;j++)
				buttonId[j] = buttonId[j+1];
			buttonId[9] = NULL;
			return;
		}
	}
}

void UI_Radio::draw(DC* dc) const
{
	assert(dc);

	UI_Group::draw(dc);
}

void UI_Radio::forceUnpressAll()
{
	if(!getChildren())
		return;

	for(unsigned int i = 0; i < RADIO_MAX_BUTTONS; i++)
		if(buttonId[i]) {
			buttonId[i]->forceUnpress();
		}
}


void UI_Radio::process()
{
// TODO
	UI_Button* lastPressed = NULL;
//	UI_Button* tmp;
	markedItem = -1;
	if(buttonWasPressed != NULL)
	{
		for(unsigned int i = 0; i < RADIO_MAX_BUTTONS; i++)
			if(buttonId[i])
			{
				if(buttonWasPressed!=buttonId[i])
					buttonId[i]->forceUnpress();
				else markedItem = i;
			}
		buttonWasPressed = NULL;
	}

	for(unsigned int i = 0; i < RADIO_MAX_BUTTONS; i++)
		if((buttonId[i])&&(buttonId[i]->isCurrentlyActivated()))
		{
			lastPressed = buttonId[i];
			break;
		}
	UI_Group::process();
		
	if(lastPressed!=NULL)
	{
		bool allUnpressed=true;
		for(unsigned int i = 0; i < RADIO_MAX_BUTTONS; i++)
			if((buttonId[i])&&(buttonId[i]->isCurrentlyActivated()))
			{
				allUnpressed = false;
				break;
			}
		if(allUnpressed)
			lastPressed->forcePress();
	}
	if(reorder)
	{
		reorder=false;
//		calculateBoxSize(); // TODO
/*		resetMinXY();
		for(unsigned int i = 0; i < RADIO_MAX_BUTTONS; i++)
			if(buttonId[i])
				buttonId[i]->adjustPositionAndSize(ADJUST_ONLY_POSITION);*/
	}
}

