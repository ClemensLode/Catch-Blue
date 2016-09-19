#include "ui_bitmap.hpp"

UI_Bitmap::UI_Bitmap(SDL_Object* bitmap_parent, 
				const Rect& bitmap_rect,
				const Size distance_bottom_right, 
				const Bitmap* my_bitmap, 
				const ePositionMode bitmap_position_mode) :
	UI_Object(bitmap_parent, Rect(bitmap_rect.getTopLeft(), Size((*my_bitmap)->w, (*my_bitmap)->h)),  distance_bottom_right, bitmap_position_mode, NO_AUTO_SIZE),
	bitmap(my_bitmap),
	checked(false),
	brightness(100)
{
	setClipped();
	setDrawType(TRANSPARENT_OBJECT);
}

UI_Bitmap::~UI_Bitmap()
{}

void UI_Bitmap::reloadOriginalSize()
{
	setOriginalSize(getBitmapSize());
	UI_Object::reloadOriginalSize();
	adjustSize(CHILD_WAS_CHANGED);
}

void UI_Bitmap::setBrightness(const Uint8 bitmap_brightness) 
{
	assert(bitmap_brightness <= 255);

	if(brightness != bitmap_brightness)
	{
		brightness = bitmap_brightness;
		makePufferInvalid();
//		setNeedRedrawAllThatOverlaps(getRelativeRect());
	}
}


// Render button.  How it draws exactly depends on it's current state.
void UI_Bitmap::draw(DC* dc) const
{
	assert(dc);

	if(getClipRect() != Rect()) {
		//gui.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap
		dc->DrawBrightenedBitmap(*bitmap, Point(), getClipRect(), brightness);
	}
	else {
		dc->DrawBrightenedBitmap(*bitmap, Point(), brightness);
	}
	UI_Object::draw(dc);
}

void UI_Bitmap::setBitmap(const Bitmap* new_bitmap)
{
	if(new_bitmap != bitmap) {
		bitmap = new_bitmap;
		reloadOriginalSize();
		//redraw?
		makePufferInvalid();
	}
}

UI_Object* UI_Bitmap::checkToolTip(const Point& mouse) const {
	if( (!isShown()) || (!getAbsoluteRect().isTopLeftCornerInside(mouse))) {
 		return(NULL);
	}
	return((UI_Object*)this);
}

const SDL_Object* UI_Bitmap::checkHighlight(const Point& mouse)
{
	if(!isShown()) {
		return NULL;
	}
	return UI_Object::checkHighlight(mouse);
//	return((UI_Object*)this); lol nein!
	
}

void UI_Bitmap::process()
{
	UI_Object::process();
}

