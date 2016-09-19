#ifndef _UI_STATICTEXT_HPP
#define _UI_STATICTEXT_HPP

#include "ui_object.hpp"
#include <font.hpp>

class UI_StaticText : public UI_Object
{
	public:
		UI_StaticText(SDL_Object* st_parent, 
				const Rect& st_pos, 
				const Size& distance_bottom_right, 
				const Color* st_color, 
				const Font* st_font, 
				const ePositionMode position_mode = HORIZONTALLY_CENTERED);
		UI_StaticText(SDL_Object* st_parent, 
				const Rect& st_pos, 
				const Size& distance_bottom_right, 
				const Text* st_text, 
				const Color* st_color, 
				const Font* st_font, 
				const ePositionMode position_mode = HORIZONTALLY_CENTERED);
		UI_StaticText(SDL_Object* st_parent, 
				const Rect& st_pos, 
				const Size& distance_bottom_right, 
				const std::string& st_text, 
				const Color* st_color, 
				const Font* st_font, 
				const ePositionMode position_mode = HORIZONTALLY_CENTERED);
		~UI_StaticText();

		void setTemporaryColor(const Color& st_color);
		void setColor(Color* st_color);
		void setFont(Font* st_font);
		const std::string& getString() const;
		void updateText(const std::string& st_text, const bool etext_change = false);
		void reloadText(const std::string& st_text);
		void reloadOriginalSize();

		void addChar(const unsigned int position, const char key);
		void removeCharBackspace(const unsigned int position);
		void removeCharDelete(const unsigned int position);

		UI_Object* checkToolTip(const Point& mouse) const;
		const SDL_Object* checkHighlight(const Point& mouse);
	
		void updateText(const Text* st_text);
		void reloadText(const Text* st_text);
		void setTextWasChanged();
		
		void draw(DC* dc) const;
		void process();

		const Size getTextSize() const;
		const Rect& getTextBox() const;
		const Size getTextPosSize(const unsigned int pos) const;

		void doHighlight(const bool high_light = true);
	private:
		const Font* font;
		const Color* color;
		const Text* eText;
		std::string text;

		bool highlight;
		bool tempColorIsSet;
		bool textWasChanged;
		Color tempColor;

		UI_StaticText& operator=(const UI_StaticText& object);
		UI_StaticText(const UI_StaticText& object);
};

inline const SDL_Object* UI_StaticText::checkHighlight(const Point& mouse) {
	return SDL_Object::checkHighlight(mouse);
}

inline const Size UI_StaticText::getTextSize() const {
	return(font->getTextExtent(text));
}

inline const Size UI_StaticText::getTextPosSize(const unsigned pos) const {
	return(font->getTextExtent(text.substr(0, pos)));
}

inline const std::string& UI_StaticText::getString() const {
	return(text);
}

inline void UI_StaticText::setColor(Color* st_color) {
	color = st_color;
}

inline void UI_StaticText::setTemporaryColor(const Color& st_color) {
	tempColorIsSet = true;
	tempColor = st_color;
}

inline void UI_StaticText::setFont(Font* st_font) {
	font = st_font;
}

inline void UI_StaticText::setTextWasChanged() {
	textWasChanged = true; 
}


#endif // _UI_STATICTEXT_HPP

