#ifndef _SDL_FONT_HPP
#define _SDL_FONT_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include "size.hpp"

#include <string>
#include <misc.hpp>
#include <resource.hpp>

class DC_API Font : public Resource
{
	public:
		Font(const std::string& font_name, const Uint16 font_size, const bool is_under_lined = false, const bool is_shadow = false);
		Font(const Font& object);
		Font();
		~Font();
		void setFont(const std::string& font_name, const Uint16 font_size);

		Font& operator=(const Font& object);
		operator TTF_Font*();

		const Size getTextExtent(const std::string& font_text) const; 
		void DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& text, const Sint16 x, const Sint16 y) const;
		void DrawText(SDL_Surface* surface, const SDL_Color& color, const SDL_Color& back_color, const std::string& font_text, const Sint16 x, const Sint16 y) const;

		TTF_Font* getTTF_Font() const;
		const bool isUnderlined() const;
		const bool isShadow() const;
		void setUnderlined(const bool underlined);
		void setShadow(const bool shadow);
		const Uint16 getSize() const;

	private:

		TTF_Font* font;
		Uint16 size;
		bool underlined;
		bool shadow;
};

inline Font::operator TTF_Font*() {
	return(font);
}

inline TTF_Font* Font::getTTF_Font() const {
	return(font);
}

inline const bool Font::isUnderlined() const {
	return(underlined);
}

inline const bool Font::isShadow() const {
	return(shadow);
}

inline void Font::setUnderlined(const bool underlined) {
	this->underlined = underlined;
}

inline void Font::setShadow(const bool shadow) {
	this->shadow = shadow;
}

inline const Uint16 Font::getSize() const {
	return(size);
}

#endif // _SDL_FONT_HPP
