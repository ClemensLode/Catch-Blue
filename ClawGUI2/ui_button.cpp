#include "ui_button.hpp"
#include "ui_radio.hpp"

UI_Button::UI_Button(SDL_Object* button_parent, 
		const Rect& button_rect, 
		const Size& distance_bottom_right, 
		const ButtonColors* button_colors, 
		const Text* button_text, 
		const Text* button_tooltip, 
		const eButtonMode button_mode, 
		const ePositionMode button_position_mode, 
		const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right, 
			button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : button_position_mode, button_auto_size),
	radio(NULL), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
//	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	oldGradient(999),
	pressdepth(0),
	buttonColors(button_colors),
	bitmap(NULL),
	statusFlags(0),
	frameNumber(0),
	text(NULL),
	animationPhase(NORMAL_BUTTON_PHASE)
{
	if(buttonColors->bitmap[NORMAL_BUTTON_PHASE]) {
		bitmap = new UI_Bitmap(this, Rect(), Size(), buttonColors->bitmap[NORMAL_BUTTON_PHASE], VERTICALLY_CENTERED);
	}
	updateToolTip(button_tooltip);
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case TOP_TAB_BUTTON_MODE:statusFlags |= BF_IS_TOP_TAB; statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 

	Size bitmap_size;
	if(bitmap)
	{
		bitmap_size = bitmap->getBitmapSize();
		setClipped();
	//	bitmap->setPositionParent(this);
	}

	if(button_text!=NULL)
	{
		if(button_position_mode == SPECIAL_BUTTON_LEFT) {
			text = new UI_StaticText(this, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), button_text, buttonColors->startTextColor[PRESSED_BUTTON_PHASE], buttonColors->font, CENTER_LEFT); // TODO
		} else if(button_auto_size == AUTO_SIZE) {
			text = new UI_StaticText(this, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), button_text, buttonColors->startTextColor[PRESSED_BUTTON_PHASE], buttonColors->font, DO_NOT_ADJUST); // TODO
		} else {
			text = new UI_StaticText(this, Rect(Point(bitmap_size.getWidth(), 1), getSize()), Size(0,0), button_text, buttonColors->startTextColor[PRESSED_BUTTON_PHASE], buttonColors->font, TOTAL_CENTERED); // TODO
		}
	} else if(bitmap) {
		setOriginalSize(bitmap_size);
	}

}

UI_Button::UI_Button(SDL_Object* button_parent, 
					 const Rect& button_rect, 
					 const Size& distance_bottom_right, 
					 const ButtonColors* button_colors,
					 const std::string& button_text, 
					 const Text* button_tooltip,
					 const eButtonMode button_mode, 
					 const ePositionMode button_position_mode, 
					 const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right,
					button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : 
					button_position_mode, 
					button_auto_size),
	radio(NULL), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
//	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	oldGradient(999),
	pressdepth(0),
	buttonColors(button_colors),
	bitmap(NULL),
	statusFlags(0),
	frameNumber(0),
	text(NULL),
	animationPhase(NORMAL_BUTTON_PHASE)
{
	if(buttonColors->bitmap[NORMAL_BUTTON_PHASE]) {
		bitmap = new UI_Bitmap(this, Rect(), Size(), buttonColors->bitmap[NORMAL_BUTTON_PHASE], VERTICALLY_CENTERED);
	}
	updateToolTip(button_tooltip);

	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case TOP_TAB_BUTTON_MODE:statusFlags |= BF_IS_TOP_TAB; statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 
//	adjustPositionAndSize( gui.lookUpFont(font)->getTextExtent(button_text));//text->getTextSize());
//	startRect=getRelativeRect();
//	targetRect=getRelativeRect();
//	filledHeight=getHeight();

	
	Size bitmap_size;
	if(bitmap)
	{
		bitmap_size = bitmap->getBitmapSize();
		setClipped();
//		bitmap->setPositionParent(this);
	}
	if((button_text!="")||(!bitmap))
	{
		if(button_position_mode == SPECIAL_BUTTON_LEFT) {
			text = new UI_StaticText(this, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), button_text, buttonColors->startTextColor[PRESSED_BUTTON_PHASE], buttonColors->font, CENTER_LEFT); // TODO
		} else if(button_auto_size == AUTO_SIZE) {
			text = new UI_StaticText(this, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), button_text, buttonColors->startTextColor[PRESSED_BUTTON_PHASE], buttonColors->font, DO_NOT_ADJUST); // TODO
		} else {
			text = new UI_StaticText(this, Rect(Point(bitmap_size.getWidth(), 1), getSize()), Size(0,0), button_text, buttonColors->startTextColor[PRESSED_BUTTON_PHASE], buttonColors->font, TOTAL_CENTERED); // TODO
		}
	} else if(bitmap) {
		setOriginalSize(bitmap_size);
	}
}

UI_Button::~UI_Button()
{
	if(UI_Button::currentButton == this) {
		UI_Button::resetButton();
	}
	delete bitmap; // ~_~
	delete text;
}

void UI_Button::reloadOriginalSize()
{
	if((text==NULL)&&(bitmap)) {
		setOriginalSize(bitmap->getBitmapSize());
	}
	UI_Object::reloadOriginalSize();
	adjustSize(CHILD_WAS_CHANGED, getTextSize());
}

void UI_Button::setPressDepth(const unsigned int depth)
{
	if((pressdepth == depth)||(statusFlags & BF_NOT_CLICKABLE))
		return;
//	UI_Object::addToProcessArray(this);
	pressdepth = depth;
	wasPressed = true;
	if(statusFlags & BF_IS_CHECKBUTTON)
	{
		if(pressdepth == 0) {
			bitmap->setChecked(false);
		} else {
			bitmap->setChecked(true);
		}
	}
}


// Render button.  How it draws exactly depends on it's current state.
void UI_Button::draw(DC* dc) const
{
	assert(dc);


//		if(!(statusFlags & BF_NOT_CLICKABLE)) // ?? TODO
//		{
//			dc->setBrush(*gui.lookUpBrush(NULL_BRUSH));
//			dc->setPen(*gui.lookUpPen(DARK_BACKGROUND_PEN));
//			dc->DrawEdgedRectangle(Rect(getAbsolutePosition()-Size(1,1), getSize() + Size(2,2)));

//			dc->setBrush(*gui.lookUpBrush(NULL_BRUSH)); //?
//			dc->setPen(*gui.lookUpPen(RECTANGLE_PEN));
//			dc->DrawRoundedRectangle(Rect(getAbsolutePosition(), getSize()), 2);
//		}

	dc->setPen(Pen(buttonColors->startBorderPen[animationPhase]->getColor().mixColor(
		dc->getSurface(), 
		buttonColors->endBorderPen[animationPhase]->getColor(), gradient), 
			buttonColors->startBorderPen[animationPhase]->getWidth(), 
			buttonColors->startBorderPen[animationPhase]->getStyle()));

	if((text!=NULL)||((text==NULL)&&(!bitmap))) // textbutton/bitmaptextbutton or empty button (e.g. bograph)
	{
		dc->setBrush(Brush(buttonColors->startBrush[animationPhase]->getColor().mixColor(dc->getSurface(), 
			buttonColors->endBrush[animationPhase]->getColor(), gradient),
					buttonColors->startBrush[animationPhase]->getStyle()));
//TODO: eigene tab-button klasse


//			dc->setPen(*gui.lookUpPen(NULL_PEN));
//			if(statusFlags & BF_DOWN)
		if(text!=NULL) {
			text->setTemporaryColor(buttonColors->endTextColor[animationPhase]->mixColor(dc->getSurface(), 
				*(buttonColors->startTextColor[animationPhase]), gradient));
		}
// TODO TAB-BUTTON MODE
		if(pressdepth>0) {
			dc->setPressedRectangle(true);
		} else {
			dc->setPressedRectangle(false);
		}

		if(statusFlags & BF_IS_RECTANGLE) {
//			dc->DrawRectangle(getAbsolutePosition()+Size(pressdepth, pressdepth), getSize()/*+Size(1,1)*/);
			dc->DrawEdgedRoundedRectangle(Point(0, 0), getSize(), 2);
		} else if(statusFlags & BF_IS_TOP_TAB) {
			dc->DrawTabRectangle(Point(0,0), getSize());
		} else {
			dc->DrawEdgedRoundedRectangle(Point(0,0), getSize(), 4);
		}
		dc->setPressedRectangle(false);
	}

 // empty rectangle (e.g. bograph)
	
/*		if((animationPhase==PRESSED_BUTTON_PHASE)||(animationPhase==PRESSED_HIGHLIGHTED_BUTTON_PHASE))
		{
			dc->setPen(Pen(*(gui.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->getColor()),1,SOLID_PEN_STYLE));
			dc->DrawLine(getAbsolutePosition() + Point(0, getHeight()-1), getAbsolutePosition() + Point(getWidth(), getHeight()-1));
			dc->DrawLine(getAbsolutePosition() + Point(getWidth()-1, 0), getAbsolutePosition() + Point(getWidth()-1, getHeight()));
		} ??? */

//		dc->DrawRectangle(Rect(originalButtonRect.getTopLeft() + getAbsolutePosition(), originalButtonRect.getSize()));
	UI_Object::draw(dc);
//	dc->setPen(*gui.lookUpPen(RECTANGLE_PEN));
//	dc->DrawRectangle(getAbsoluteRect());
}



//void UI_Button::set_hotkey_if_focus(int key)
//{
//	hotkey_if_focus = key;
//}
// reset anything that needs to be at the start of a new frame before processing
void UI_Button::frameReset()
{
	statusFlags &= ~BF_HIGHLIGHTED;
	statusFlags &= ~BF_LEFT_CLICKED;
	statusFlags &= ~BF_DOUBLE_CLICKED;
// TODO bei -O3 bleiben die buttons gehighlighted... statusFlags = 0 behebt das, aber das wollen wir ja nicht :o
//	  hideToolTip(); ~~
}

void UI_Button::mouseHasEnteredArea()
{
//	UI_Object::addToProcessArray(this);
	resetGradient();
	if(!(statusFlags & BF_HIGHLIGHTED))
	{
		statusFlags |= BF_JUST_HIGHLIGHTED;
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
		gui.getSound()->playSound(MOUSEOVER_SOUND, /*(getAbsolutePosition() + getSize()/2)*/gui.getMouse().getX());
#endif
	}
	statusFlags |= BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(isOriginalPosition) {
			setPressDepth(0);
		} else {
			setPressDepth(1);
		}
		statusFlags |= BF_DOWN;
	}
/*	if((buttonColorsType == TEXT_BUTTON) || (buttonColorsType == VISITED_TEXT_BUTTON)) {
		SDL_SetCursor(gui.lookUpCursor(HAND_CURSOR, 0));
	}*/ // TODO
}

void UI_Button::mouseHasLeftArea()
{
//	UI_Object::addToProcessArray(this);
	statusFlags &= ~BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(isOriginalPosition) {
			setPressDepth(1);
		} else {
			setPressDepth(0);
		}
		statusFlags &= ~BF_DOWN;
	}
	/*if(buttonColorsType == TEXT_BUTTON) {
		SDL_SetCursor(gui.lookUpCursor(ARROW_CURSOR, 0));
	}*/ // TODO
}

void UI_Button::mouseLeftButtonPressed(const Point& mouse)
{
	if((statusFlags & BF_NOT_CLICKABLE)||((statusFlags & BF_IS_TAB)&&(isOriginalPosition==true))) {
		return;
	}
//	UI_Object::addToProcessArray(this);
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	if(isOriginalPosition) {
		setPressDepth(0);
	} else {
		setPressDepth(1);
	}
	if(allowMoveByMouse)
	{
		UI_Button::mouseMovePoint = mouse;
		UI_Button::moveByMouse = true;
	}
}

void UI_Button::mouseLeftButtonReleased(const Point& mouse)
{
	if((statusFlags & BF_NOT_CLICKABLE)) {
		return;
	}
//	UI_Object::addToProcessArray(this);
	statusFlags &= ~BF_WAS_PRESSED;
	if(statusFlags & BF_DOWN)
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_LEFT_CLICKED;
		if(statusFlags & BF_STATIC)
		{
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
			gui.getSound()->playSound(CLICKED_SOUND, gui.getMouse().getX());
#endif
			if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB))) {
				isOriginalPosition = false;
			}
			else if(!isOriginalPosition) {
				isOriginalPosition = true;
			}
			wasPressed = true;
			SDL_Object::setFocus(this);
		}
		else
		{
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
			gui.getSound()->playSound(CLICK_SOUND, gui.getMouse().getX());
#endif
			setPressDepth(0);
		}
		if(radio)
		{
			if(isOriginalPosition)
				radio->leftButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->leftButtonReleased(this);
		}		
	}
	if(allowMoveByMouse)
	{
		UI_Button::mouseMovePoint = Point(0,0);
		UI_Button::moveByMouse = false;
	}
}

void UI_Button::mouseRightButtonPressed(const Point& mouse)
{
	if((statusFlags & BF_NOT_CLICKABLE)||(statusFlags & BF_STATIC)||((statusFlags & BF_IS_TAB)&&(isOriginalPosition==true))) {
		return;
	}
//	UI_Object::addToProcessArray(this);
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	
	if(isOriginalPosition) {
		setPressDepth(0);
	} else {
		setPressDepth(1);
	}
}

void UI_Button::mouseRightButtonReleased(const Point& mouse)
{
	if((statusFlags & BF_NOT_CLICKABLE) || (statusFlags & BF_STATIC)) {
		return;
	}
//	UI_Object::addToProcessArray(this);
	statusFlags &= ~BF_WAS_PRESSED;
	if(statusFlags & BF_DOWN)
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_RIGHT_CLICKED;
	
		if(statusFlags & BF_STATIC)
		{
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
			gui.getSound()->playSound(CLICK_SOUND, gui.getMouse().getX());
//			gui.getSound()->playSound(CLICKED_SOUND, (getAbsolutePosition() + getSize()/2).x);
#endif
	/*		if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;*/
		}
		else
		{
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
			gui.getSound()->playSound(CLICK_SOUND, gui.getMouse().getX());
#endif
			setPressDepth(0);
		}
	/*	if(radio)
		{
			if(isOriginalPosition)
				radio->rightButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->rightButtonReleased();
		}		 // TODO?*/
	}
}



UI_Object* UI_Button::checkToolTip(const Point& mouse) const 
{
	if( !isShown() || !getAbsoluteRect().isTopLeftCornerInside(mouse)) {
		return(NULL);
	}
	return (UI_Object*)this;
}

const SDL_Object* UI_Button::checkHighlight(const Point& mouse)
{
	if(!isShown()) {
		return NULL;
	}

	if(!getAbsoluteRect().isTopLeftCornerInside(mouse)) {
		return UI_Object::checkHighlight(mouse);
	}
	return((SDL_Object*)this);
}

void UI_Button::process()
{
	// TODO evtl Animation fuer jede Phase in die config datei
	// dann waere sowas moeglich, dass ich maus reinfahr und das langsam verblasst
	// evtl auch einfach brightencolor ueberlegen...

	if(wasPressed)
	{
		wasPressed = false;
//		setNeedRedrawMoved();
	} // TODO

//		gradient = 100;
//	else
	if(!(statusFlags & BF_HIGHLIGHTED)) {
		gradient += (100 - gradient) / 6 + 1;
	} else  
/*	if((!gui.isGlowingButtons()) || (buttonColors->animationType == NO_ANIMATION))
	{
		if(gradient > gradient / 10 + 1) {
			gradient -= gradient / 10 + 1;
		} else {
			gradient = 0;
		}
	}  
	else*/ // TODO
	{
//		UI_Object::addToNextProcessArray(this);
		switch(buttonColors->animationType)
		{	
//			case NO_ANIMATION:if(gradient < 100) ++gradient;else gradient = 100;break;
			case JUMPY_COLORS_ANIMATION:gradient=(frameNumber%buttonColors->speed)*100/buttonColors->speed;break;
			case GLOWING_ANIMATION:gradient=(unsigned int)(50*(sin(3.141*frameNumber/buttonColors->speed)+1));break;
			case BLINKING_ANIMATION:if(frameNumber<buttonColors->speed/2) gradient=0;else gradient=100;break;
			default:break;
		}
	}

	if(gradient > 100) {
		gradient = 100;
	}
	
	if(gradient != oldGradient)
	{
		redrawWholeObject();
		oldGradient = gradient;
	}

	Point absoluteCoord = getRelativePosition();
	Size absoluteSize = getSize();
	UI_Object::process();
	
	if(statusFlags & BF_WAS_PRESSED) {
		statusFlags |= BF_HIGHLIGHTED;
	}

	if(statusFlags & BF_HIGHLIGHTED)
	{
		if(frameNumber<buttonColors->speed) {
			frameNumber += 2;
		}
		else {
			frameNumber = 0;
		}
	}


	if((statusFlags & BF_STATIC)&&(isOriginalPosition))
	{
		if((statusFlags & BF_HIGHLIGHTED) || (gradient < 100 )) {
			animationPhase = PRESSED_HIGHLIGHTED_BUTTON_PHASE;
		} else {
			animationPhase = PRESSED_BUTTON_PHASE;
		}
	}
	else
	if(statusFlags & BF_DOWN)
	{
		if((statusFlags & BF_HIGHLIGHTED) || (gradient < 100 )) {
			animationPhase = PRESSED_HIGHLIGHTED_BUTTON_PHASE;
		} else {
			animationPhase = PRESSED_BUTTON_PHASE;
		}
	}
	else if((statusFlags & BF_HIGHLIGHTED) || (gradient < 100)) {
		animationPhase = HIGHLIGHT_BUTTON_PHASE;
	} else {
		animationPhase = NORMAL_BUTTON_PHASE;
	}

	if(bitmap)
	{
		if(buttonColors->bitmap[animationPhase]) {// != bitmap->getBitmapHeight()) {
			bitmap->setBitmap(buttonColors->bitmap[animationPhase]);
		}
		bitmap->setClipRect(this->getClipRect());
		bitmap->setBrightness((Uint8)(100-gradient/2));
	}
}

const Size UI_Button::getTextSize() const 
{
	if(text == NULL) {
		return Size();
	}
	else {
		return text->getTextSize();
	}
}

const Uint16 UI_Button::getTextWidth() const 
{
	if(text == NULL) {
		return 0;
	}
	else {
		return text->getTextSize().getWidth();
	}
}

const Uint16 UI_Button::getTextHeight() const {
	if(text==NULL) {
		return 0;
	}
	else {
		return text->getTextSize().getHeight();
	}
}

void UI_Button::updateText(const std::string& utext) 
{
	if(text==NULL) {
		return;
	}
	text->updateText(utext);
}

void UI_Button::updateText(const Text* utext) 
{
	if(text == NULL) {
		return;
	}
	text->updateText(utext);
}


void UI_Button::doHighlight(const bool high_light) 
{
	if(high_light) {
		statusFlags |= BF_HIGHLIGHTED;
	} else {
		statusFlags &= ~BF_HIGHLIGHTED;
	}
}


const bool UI_Button::isLeftClicked()
{
	if(!isShown()) {
		return false;
	}
// currently pressed
	if (statusFlags & BF_LEFT_CLICKED)
	{
		statusFlags &= ~BF_LEFT_CLICKED;
		return true;
	}
	else {
		return false;
	}
}
const bool UI_Button::isRightClicked()
{
	if(!isShown()) {
		return false;
	}
// currently pressed
	if (statusFlags & BF_RIGHT_CLICKED)
	{
		statusFlags &= ~BF_RIGHT_CLICKED;
		return true;
	}
	else {
		return false;
	}
}


void UI_Button::resetGradient()
{
	if(gradient != 0)
	{
		gradient=0;
		frameNumber=0;
		makePufferInvalid();
	}
}

const bool UI_Button::isCurrentlyActivated() const
{
//	if(!isShown())
//			return false;
	if ( (statusFlags & BF_STATIC) && (isOriginalPosition==true) ) {
		return true;
	} else {
		return false;
	}
}

const bool UI_Button::isCurrentlyPressed() const
{
	if(!isShown()) {
		return false;
	}
	if (statusFlags & BF_DOWN) {
		return true;
	} else {
		return false;
	}
}

// Is the mouse over this button?
const bool UI_Button::isCurrentlyHighlighted() const
{
	if(!isShown())
		return false;
	return (statusFlags & BF_HIGHLIGHTED) == BF_HIGHLIGHTED;
}

// changes static buttons
void UI_Button::check(const bool is_checked)
{
	if(isCurrentlyActivated() != is_checked)
    {
		if(!is_checked) {
			forceUnpress();
		} else {
			forcePress();
		}
		makePufferInvalid();
	}
}

// Force button to get pressed, doesn't work for non-static buttons yet
void UI_Button::forcePress(const bool click_message)
{
	if (!(statusFlags & BF_NOT_CLICKABLE))
    {
		if((statusFlags & BF_STATIC) && (!isOriginalPosition))
        {
			if(click_message) {
				statusFlags |= BF_LEFT_CLICKED;
			}
            statusFlags &= ~BF_WAS_PRESSED;
            isOriginalPosition = true;
//          if (statusFlags & BF_DOWN)
//			{
            statusFlags &= ~BF_DOWN;
            setPressDepth(1);
		} else if((!(statusFlags & BF_STATIC)) && (!(statusFlags & BF_DOWN))) {
			if(click_message)
			{
				statusFlags |= BF_LEFT_CLICKED;
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
				gui.getSound()->playSound(CLICK_SOUND, (getAbsolutePosition() + getSize()/2).getX());
#endif
			}
			statusFlags |= BF_DOWN;                        //~?
            setPressDepth(1); //?
        }
    }
}

void UI_Button::forceUnpress(const bool click_message)
{
	if((statusFlags & BF_STATIC) && (isOriginalPosition))
    {
		isOriginalPosition = false;
        statusFlags &= ~BF_WAS_PRESSED;
//		if(click_message);
//      statusFlags |= BF_LEFT_CLICKED;
        statusFlags &= ~BF_DOWN; // ~~
        setPressDepth(0);
	}
}


const bool UI_Button::addKey(unsigned int key, unsigned int mod)
{
	if(((key == SDLK_KP_ENTER)||(key == SDLK_RETURN))&& (!(mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))))
	{
		forcePress();
		return(true);
	}
	return(false);	
}

void UI_Button::resetButton()
{
	if(UI_Button::currentButton!=NULL) {
		UI_Button::currentButton->frameReset();
	}
	UI_Button::currentButton = NULL;
	UI_Button::currentButtonHasAlreadyLeft = false;
	UI_Button::currentButtonPressed = false;
	UI_Button::mouseMovePoint = Point(0, 0);
	UI_Button::moveByMouse = false;
	UI_Button::setWasResetted();
//	SDL_SetCursor(gui.lookUpCursor(ARROW_CURSOR, 0)); // TODO
}


bool UI_Button::currentButtonPressed = false;
bool UI_Button::currentButtonHasAlreadyLeft = false;
UI_Button* UI_Button::currentButton = NULL;
bool UI_Button::moveByMouse = false;
Point UI_Button::mouseMovePoint = Point();
bool UI_Button::doClickedSound = false;
bool UI_Button::wasResetted = false;
