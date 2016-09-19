#ifndef _UI_BITMAP_HPP
#define _UI_BITMAP_HPP

#include "ui_object.hpp"
#include <bitmap.hpp>

class UI_Bitmap : public UI_Object
{
	public:
		UI_Bitmap(SDL_Object* bitmap_parent, 
				const Rect& bitmap_rect, 
				const Size distance_bottom_right,
				const Bitmap* my_bitmap,
				const ePositionMode bitmap_position_mode = DO_NOT_ADJUST);
		~UI_Bitmap();
		
		void reloadOriginalSize();
		void process();
		UI_Object* checkToolTip(const Point& mouse) const;
		const SDL_Object* checkHighlight(const Point& mouse);
		void object_info();
		void draw(DC* dc) const;
		const Uint16 getBitmapWidth() const;
		const Uint16 getBitmapHeight() const;
		const Size getBitmapSize() const;

		void setBrightness(const Uint8 bitmap_brightness);
		void setChecked(const bool bitmap_checked = true);  // for buttons... 

		void setBitmap(const Bitmap* new_bitmap);
		
	private:
		const Bitmap* bitmap;
		// TODO weiteres Bitmap* Feld für checked-Bitmaps!
		bool checked;
		Uint8 brightness;
};

inline void UI_Bitmap::setChecked(const bool bitmap_checked) {
	checked = bitmap_checked;
}

inline const Uint16 UI_Bitmap::getBitmapWidth() const {
	return (Uint16)((*bitmap)->w);
}

inline const Uint16 UI_Bitmap::getBitmapHeight() const {
	return (Uint16)((*bitmap)->h);
}

inline const Size UI_Bitmap::getBitmapSize() const {
	return(Size(getBitmapWidth(), getBitmapHeight()));
}

#endif // _UI_BITMAP_HPP

