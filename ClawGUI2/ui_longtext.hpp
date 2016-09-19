#ifndef _UI_LONGTEXT_HPP
#define _UI_LONGTEXT_HPP


#include "ui_button.hpp"
#include "fonttype.hpp"

class UI_LongText : public UI_Object
{
	public:
		UI_LongText(SDL_Object* lt_parent, 
				const Rect lt_pos,
				const Size distance_bottom_right, 
				const std::string& lt_text, 
				Color* lt_color1, 
				Color* lt_color2, 
				FontType* lt_font, 
				FontType* big_lt_font,
				ButtonColors* lt_button, 
				ButtonColors* lt_visited_button,// = TEXT_BUTTON, 
				UI_Object* scroll_bar = NULL, 
				const bool use_help_chapters = false, 
				const ePositionMode lt_mode = DO_NOT_ADJUST, 
				const eAutoSize auto_size = NO_AUTO_SIZE);

		~UI_LongText();
		void updateText(const std::string& lt_text);

		void reloadOriginalSize();
		
		void draw(DC* dc) const;
		void process();

		const signed int getPressed() const;
		const Uint16 getTextHeight() const;
		const Uint16 getTextWidth() const;
		
	private:
		UI_Object* scrollBar;
		std::list<UI_StaticText*> longText; // parsed text
		std::list<UI_Button*> longButton; // parsed buttons
		std::list<signed int> longNumber; // numbers of parsed buttons
		std::list<UI_Bitmap*> longBitmap; // parsed Bitmaps
		std::string text;
		Color* color1;
		Color* color2;
		FontType* fontType;
		FontType* bigFontType;
		ButtonColors* button;
		ButtonColors* visitedButton;
		bool textWasChanged;
		bool useHelpChapters;

		UI_LongText& operator=(const UI_LongText& object);
		UI_LongText(const UI_LongText& object);
};

#endif // _UI_LONGTEXT_HPP

