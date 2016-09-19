#ifndef _UI_BUTTON_HPP
#define _UI_BUTTON_HPP

#include "ui_statictext.hpp"
#include "ui_bitmap.hpp"
#include <sdl_object.hpp>
#include "buttoncolors.hpp"


enum eButtonMode
{
	NO_BUTTON_MODE,
	STATIC_BUTTON_MODE, // stays in its position
	PRESS_BUTTON_MODE,  // returns to unpressed
	PUSH_BUTTON_MODE,   // calls several messages when being pressed
	TAB_BUTTON_MODE,
	TOP_TAB_BUTTON_MODE,
	CHECK_BUTTON_MODE, // no '3d effect', allows button to be set with 'check'
//	MENU_TAB_BUTTON_MODE,
	BOGRAPH_BUTTON_MODE, // just a rectangle, not rounded
	
	MAX_BUTTON_MODES
};

class UI_Radio;

class UI_Button : public UI_Object
{
	public:
		UI_Button(SDL_Object* button_parent, 
				const Rect& button_rect, 
				const Size& distance_bottom_right,
				const ButtonColors* button_colors, 
				const Text* button_text, 
				const Text* button_tooltip,
				const eButtonMode button_mode,				
				const ePositionMode button_position_mode = DO_NOT_ADJUST, 
				const eAutoSize button_auto_size = NO_AUTO_SIZE);

		UI_Button(SDL_Object* button_parent, 
				const Rect& button_rect, 
				const Size& distance_bottom_right, 
				const ButtonColors* button_colors,
				const std::string& button_text, 
				const Text* button_tooltip,
				const eButtonMode button_mode, 
				const ePositionMode button_position_mode = DO_NOT_ADJUST,
				const eAutoSize button_auto_size = NO_AUTO_SIZE);
		~UI_Button();
		
		const bool isLeftClicked();			// has it been selected (ie clicked on)
		const bool isRightClicked();
		const bool isCurrentlyPressed() const; // is the button depressed?
		const bool isCurrentlyHighlighted() const; // is the mouse over this button?
		const bool isCurrentlyActivated() const;

		const unsigned int getGradient() const;

		void reloadOriginalSize();																																							
		void doHighlight(const bool high_light = true);	// force button to be highlighted

		void check(const bool is_checked = true);
		void forcePress(const bool click_message = true);	  // force button to get pressed
		void forceUnpress(const bool Rclick_message = true);

		void updateText(const std::string& utext);
		void updateText(const Text* utext);

		void process(); // process messages, continue animation etc.
		UI_Object* checkToolTip(const Point& mouse) const;
		const SDL_Object* checkHighlight(const Point& mouse);

		void mouseHasMoved();
		void frameReset();
		
		void mouseHasEnteredArea();
		void mouseHasLeftArea();
		void mouseLeftButtonPressed(const Point& mouse);
		void mouseLeftButtonReleased(const Point& mouse);
		void mouseRightButtonPressed(const Point& mouse);
		void mouseRightButtonReleased(const Point& mouse);

		static void resetButton();
		static UI_Button* getCurrentButton();
		static const bool isCurrentButtonPressed();
		static const bool isCurrentButtonHasAlreadyLeft();
		static const Point& getMouseMovePoint();
		static const bool isMoveByMouse();
		static void setMoveByMouse(const bool move_by_mouse = true);
		static void setCurrentButtonPressed(const bool current_button_pressed = true);
		static void setCurrentButtonHasAlreadyLeft(const bool button_has_already_left = true);
		static void setMouseMovePoint(const Point& mouse_move_point);
		static void setCurrentButton(UI_Button* current_button);
		static const bool isWasResetted();
		static void setWasResetted(const bool was_resetted = true);

		const bool addKey(unsigned int key, unsigned int mod);
		
		void draw(DC* dc) const;
		void setButtonColors(ButtonColors* button_colors);
		const ButtonColors* getButtonColors() const;

		void setBitmap(Bitmap* button_bitmap);
		
		void resetGradient();
		UI_Radio* radio;

// relative 'button placement area'
		const Uint16 getTextWidth() const;
		const Uint16 getTextHeight() const;
		const Size getTextSize() const;
		void setAllowMoveByMouse(const bool allow_move_by_mouse = true);


		
		UI_StaticText* getText() const;

		const bool isBitmapButton() const;
	private:
		static bool doClickedSound;
		static bool doClickSound;
		void setPressDepth(const unsigned int press);
		bool allowMoveByMouse;

		static bool wasResetted;
		static Point mouseMovePoint;
		static bool moveByMouse;
		static bool currentButtonPressed;
		static bool currentButtonHasAlreadyLeft;
		static UI_Button* currentButton;
		static bool doMouseEnterSound;

		bool moved; // did this item move one pixel down (pressed)
		bool isOriginalPosition; // always false (not pressed) for non-static buttons
		bool wasPressed;

		unsigned int gradient;
		unsigned int oldGradient;
		unsigned int pressdepth;
		
		const ButtonColors* buttonColors;	
		UI_Bitmap* bitmap;
		
		unsigned int statusFlags;
		unsigned int frameNumber;
// Button flags

		static const unsigned int BF_DOWN = 1;
		static const unsigned int BF_JUST_PRESSED = 2;
		static const unsigned int BF_JUST_RELEASED = 4;
		static const unsigned int BF_LEFT_CLICKED = 8;
		static const unsigned int BF_RIGHT_CLICKED = 16;
		static const unsigned int BF_DOUBLE_CLICKED = 32;
		static const unsigned int BF_HIGHLIGHTED = 64;  // button is not highlighted (ie mouse is not above the item)
		static const unsigned int BF_JUST_HIGHLIGHTED = 128;  // button has just been highlighted, true for 1 frame
		static const unsigned int BF_REPEATS = 256;
		static const unsigned int BF_STATIC = 512;
		static const unsigned int BF_STATIC_PRESSED = 1024;
		static const unsigned int BF_NOT_CLICKABLE = 2048;
		static const unsigned int BF_IS_TAB = 4096;
		static const unsigned int BF_WAS_PRESSED = 8192; // button will be DOWN again, wenn mouse gets over button, without pressing the button again
		static const unsigned int BF_IS_RECTANGLE = 16384;
		static const unsigned int BF_IS_CHECKBUTTON = 32768; // no '3d effect', allows to be 'checked'
		static const unsigned int BF_IS_TOP_TAB = 65536; 
		// TODO irgendwie mit Zeit machen, dass z.B. nach 100ms die Taste automatisch losgelassen wird
	protected:
		UI_StaticText* text;
	private:
		UI_Button& operator=(const UI_Button& object);		
		UI_Button(const UI_Button& object);
		eButtonAnimationPhase animationPhase;
};

inline const bool UI_Button::isWasResetted() {
	return wasResetted;
}

inline void UI_Button::setWasResetted(const bool was_resetted) {
	wasResetted = was_resetted;
}

inline void UI_Button::setBitmap(Bitmap* button_bitmap) {
	bitmap->setBitmap(button_bitmap);
}

inline const bool UI_Button::isBitmapButton() const {
	return(bitmap != NULL);
}

inline UI_StaticText* UI_Button::getText() const {
	return(text);
}


inline void UI_Button::setAllowMoveByMouse(const bool allow_move_by_mouse) {
	allowMoveByMouse = allow_move_by_mouse;
}

inline const unsigned int UI_Button::getGradient() const {
	return(gradient);
}

inline const ButtonColors* UI_Button::getButtonColors() const {
	return(buttonColors);
}

inline void UI_Button::setButtonColors(ButtonColors* button_colors)
{
	assert(button_colors);

	buttonColors = button_colors;
}

inline UI_Button* UI_Button::getCurrentButton() {
	return currentButton;
}

inline const bool UI_Button::isCurrentButtonPressed() {
	return currentButtonPressed;
}

inline const bool UI_Button::isCurrentButtonHasAlreadyLeft() {
	return currentButtonHasAlreadyLeft;
}

inline void UI_Button::setCurrentButtonPressed(const bool current_button_pressed) {
	currentButtonPressed = current_button_pressed;
}

inline void UI_Button::setCurrentButtonHasAlreadyLeft(const bool button_has_already_left) {
	currentButtonHasAlreadyLeft = button_has_already_left;
}

inline const Point& UI_Button::getMouseMovePoint() {
	return mouseMovePoint;
}

inline void UI_Button::setMouseMovePoint(const Point& mouse_move_point) {
	mouseMovePoint = mouse_move_point;
}

inline const bool UI_Button::isMoveByMouse() {
	return moveByMouse;
}

inline void UI_Button::setMoveByMouse(const bool move_by_mouse) {
	moveByMouse = move_by_mouse;
}

inline void UI_Button::setCurrentButton(UI_Button* current_button) {
	currentButton = current_button;
}
	
#endif // _UI_BUTTON_HPP

