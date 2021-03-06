#include "ui_editfield.hpp"

// ISO-LEVEL 2 

UI_EditField::UI_EditField(SDL_Object* edit_parent, 
						   const Rect& edit_rect, 
						   const Size& edit_size, 
						   const Font* st_font, 
						   const Color* text_color,
						   const ButtonColors* button_colors,
						   const ePositionMode position_mode, 
						   const std::string& description_text, 
						   const std::string& name_proposal) :
	UI_Object(edit_parent, edit_rect, edit_size, position_mode),
	position(name_proposal.size()),
	ani(5),
	pressedEnter(false),
	pressedEscape(false),
	editField(NULL),
	descriptionText(NULL)
{
	Uint16 height = st_font->getTextExtent("01gfFA").getHeight();
	descriptionText = new UI_StaticText(this, Rect(Point(0, 4), Size(100, height+8)), Size(0,0), description_text, text_color, st_font, DO_NOT_ADJUST);
	editField = new UI_Button(this, Rect(Point(120, 0), Size(200, height+8)), Size(0,0), button_colors, name_proposal, NULL, TAB_BUTTON_MODE, SPECIAL_BUTTON_LEFT);
}

UI_EditField::UI_EditField(SDL_Object* edit_parent, 
						   const Rect& edit_rect, 
						   const Size& edit_size, 
						   const Font* st_font, 
						   const Color* text_color,
						   const ButtonColors* button_colors,
						   const ePositionMode position_mode, 
						   const Text* description_text, 
						   const std::string& name_proposal) :
	UI_Object(edit_parent, edit_rect, edit_size, position_mode),
	position(name_proposal.size()),
	ani(5),
	pressedEnter(false),
	pressedEscape(false),
	editField(NULL),
	descriptionText(NULL)
{
	Uint16 height = st_font->getTextExtent("01gfFA").getHeight();
	descriptionText = new UI_StaticText(this, Rect(Point(0, 4), Size(100, height+8)), Size(0,0), description_text, text_color, st_font, DO_NOT_ADJUST);
	editField = new UI_Button(this, Rect(Point(120, 0), Size(200, height+8)), Size(0,0), button_colors, name_proposal, NULL, TAB_BUTTON_MODE, SPECIAL_BUTTON_LEFT);
}

void UI_EditField::resetData()
{
	editField->getText()->updateText("");
	pressedEnter = false;
	pressedEscape = false;
}

UI_EditField::~UI_EditField()
{
	delete editField;
	delete descriptionText;
}



const bool UI_EditField::addKey(unsigned int key, unsigned int mod)
{
	switch(key)
	{
		case SDLK_KP_ENTER:
		case SDLK_RETURN:
			if((editField->getText()->getString().length()>0) && (!(mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT)))) {
				enterWasPressed();break;
			}
		case SDLK_ESCAPE:escapeWasPressed();break;
		case SDLK_SPACE:addChar(' ');break;
		case SDLK_UNDERSCORE:addChar('_');break;
		case SDLK_DELETE:removeCharDelete();break;
		case SDLK_BACKSPACE:removeCharBackspace();break;
		case SDLK_KP0:
		case SDLK_KP1:
		case SDLK_KP2:
		case SDLK_KP3:
		case SDLK_KP4:
		case SDLK_KP5:
		case SDLK_KP6:
		case SDLK_KP7:
		case SDLK_KP8:
		case SDLK_KP9:addChar('0'+key-SDLK_0);break;
		case SDLK_MINUS:if(mod & (KMOD_LSHIFT | KMOD_RSHIFT)) addChar('_'); else return(false);break;
		case SDLK_a:
		case SDLK_b:
		case SDLK_c:
		case SDLK_d:
		case SDLK_e:
		case SDLK_f:
		case SDLK_g:
		case SDLK_h:
		case SDLK_i:
		case SDLK_j:
		case SDLK_k:
		case SDLK_l:
		case SDLK_m:
		case SDLK_n:
		case SDLK_o:
		case SDLK_p:
		case SDLK_q:
		case SDLK_r:
		case SDLK_s:
		case SDLK_t:
		case SDLK_u:
		case SDLK_v:
		case SDLK_w:
		case SDLK_x:
		case SDLK_y:
		case SDLK_z:if(mod & (KMOD_LSHIFT | KMOD_RSHIFT)) addChar('A'+ key - SDLK_a);else addChar('a'+ key - SDLK_a);break;
		case SDLK_0:
		case SDLK_1:
		case SDLK_2:
		case SDLK_3:
		case SDLK_4:
		case SDLK_5:
		case SDLK_6:
		case SDLK_7:
		case SDLK_8:
		case SDLK_9:
			if(!(mod & (KMOD_LSHIFT | KMOD_RSHIFT))) {
				addChar('0' + key - SDLK_0);
			} else {
				return(false);
			}break;
		case SDLK_LEFT:moveLeft();break;
		case SDLK_RIGHT:moveRight();break;
		default:return(false);break;
	}
	return(true);
}


void UI_EditField::addChar(char a)
{
	if(position>=28) {
		return;
	}
	editField->getText()->addChar(position, a);
	++position;
	ani=5;
}

void UI_EditField::moveLeft()
{
	if(position>0) {
		--position;
	}
	ani=5;
}

void UI_EditField::moveRight()
{
	if(position<28) {
		++position;
	}
	ani=5;
}

void UI_EditField::removeCharBackspace()
{
	editField->getText()->removeCharBackspace(position);
	if(position>0) {
		--position;
	}
	ani=5;
}

void UI_EditField::removeCharDelete()
{
	editField->getText()->removeCharDelete(position);
	if(position>0) {
		--position;
	}
	ani=5;
}

void UI_EditField::draw(DC* dc) const
{
	assert(dc);

	UI_Object::draw(dc);
	if(SDL_Object::getFocus() != this) {
		return;
	}
	
	Rect entry_rect = Rect(Point(0,0), getOriginalRect().getSize());
//	if(entry_rect.Inside(mouse))
//		dc->setPen(*gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
//	else
//		dc->setPen(*gui.lookUpPen(INNER_BORDER_PEN));
//	dc->setBrush(*gui.lookUpBrush(TRANSPARENT_BRUSH));
//	dc->DrawEdgedRoundedRectangle(Rect(100, 100, 200, 200), 4);

//  TODO	
//	dc->setPen(Pen(gui.lookUpColor(FORCE_TEXT_COLOR)->mixColor(dc->getSurface(), *gui.lookUpColor(BRIGHT_TEXT_COLOR), (unsigned int)(50*(sin(3.141*ani/10)+1))), 1, SOLID_PEN_STYLE));
//	dc->DrawVerticalLine(editField->getRelativePosition() + Point(5 + editField->getText()->getTextPosSize(position).getWidth(), 2), editField->getHeight()-3);
}

const SDL_Object* UI_EditField::checkHighlight(const Point& mouse) {
	if(!isShown()) {
		return NULL;
	}

	editField->doHighlight(getAbsoluteRect().isTopLeftCornerInside(mouse));
	descriptionText->doHighlight(getAbsoluteRect().isTopLeftCornerInside(mouse));

	return UI_Object::checkHighlight(mouse);
}

void UI_EditField::process()
{
	if(!isShown()) {
		return;
	}

	UI_Object::process();

	if(SDL_Object::getFocus() == editField) {
		SDL_Object::resetFocus();
	}
	if(SDL_Object::getFocus() != this) {
		editField->forceUnpress();
	}
	else {
		editField->forcePress();
	}
	++ani;
	editField->makePufferInvalid();
}


