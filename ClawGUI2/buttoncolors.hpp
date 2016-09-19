#ifndef _BUTTONCOLORS_HPP
#define _BUTTONCOLORS_HPP

#include "resourcehandler.hpp"
#include <brush.hpp>
#include <color.hpp>
#include <pen.hpp>
#include <bitmap.hpp>
#include <font.hpp>

// TODO evtl Pointerpointer...

enum eButtonAnimationPhase
{
	DISABLED_BUTTON_PHASE,
	NORMAL_BUTTON_PHASE,
	PRESSED_BUTTON_PHASE,
	HIGHLIGHT_BUTTON_PHASE,
	PRESSED_HIGHLIGHTED_BUTTON_PHASE, // primarily for static buttons
	
	MAX_BUTTON_ANIMATION_PHASES
};


enum eButtonAnimationType
{
	NO_ANIMATION,
	JUMPY_COLORS_ANIMATION, // gets bright and jumps back
	GLOWING_ANIMATION, // back and forth
	BLINKING_ANIMATION // jumps between start and end colors
};

class ButtonColors
{
public:
	ButtonColors();
	~ButtonColors();

	Brush* startBrush[MAX_BUTTON_ANIMATION_PHASES];
	Brush* endBrush[MAX_BUTTON_ANIMATION_PHASES];

	Color* startTextColor[MAX_BUTTON_ANIMATION_PHASES];
	Color* endTextColor[MAX_BUTTON_ANIMATION_PHASES];
	
	Pen* startBorderPen[MAX_BUTTON_ANIMATION_PHASES];
	Pen* endBorderPen[MAX_BUTTON_ANIMATION_PHASES];

	Font* font;
	
	Bitmap* bitmap[MAX_BUTTON_ANIMATION_PHASES];// bitmap animation is fixed... for now

	unsigned int speed; // 100 = 100 steps for full animation
	eButtonAnimationType animationType;
};


#endif