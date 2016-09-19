#include "ui_tooltip.hpp"

UI_ToolTip::UI_ToolTip(SDL_Object* parent_object, 
					   const std::string& tooltip_text, 
					   WindowColors* window_colors, 
					   ObjectSize* object_size) :
	UI_MultiLineText(
		parent_object, 
		Rect(3, 3, object_size->getWidth() , 0), 
		tooltip_text,
		window_colors),
		boxSize(),
		objectSize(object_size)
{ 
}

UI_ToolTip::UI_ToolTip(SDL_Object* parent_object, 
					   Text* tooltip_text, 
					   WindowColors* window_colors, 
					   ObjectSize* object_size) :
	UI_MultiLineText(
		parent_object, 
		Rect(3, 3, object_size->getWidth(), 0), 
		// gui.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) 
		tooltip_text,
		window_colors),
		boxSize(),
		objectSize(object_size)
{ 
}
	
UI_ToolTip::~UI_ToolTip()
{ 
}

void UI_ToolTip::reloadOriginalSize()
{
	setOriginalSize(Size(objectSize->getWidth(), 0));
	UI_MultiLineText::reloadOriginalSize();
}

void UI_ToolTip::process()
{
	UI_MultiLineText::process();
}
                                                                                
void UI_ToolTip::draw(DC* dc) const
{
	assert(dc);
	UI_MultiLineText::draw(dc);
}

