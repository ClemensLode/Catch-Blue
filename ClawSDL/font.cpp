#include "font.hpp"
#include "point.hpp"


Font::Font(const std::string& font_name, const Uint16 font_size, const bool is_under_lined, const bool is_shadow):
	font(NULL),
	size(font_size),
	underlined(is_under_lined),
	shadow(is_shadow)
{
	setFont(font_name, font_size);
}

void Font::setFont(const std::string& font_name, const Uint16 font_size) {
	font = TTF_OpenFont(font_name.c_str(), font_size);

#ifdef _SCC_DEBUG
	if(font == NULL) {
		throw MyException("ERROR (Font::setFont()): Could not initialize font " + font_name + " [TTF ERROR: \"" + std::string(TTF_GetError()) + "\"].");
	}
#endif
}

Font::Font():
	font(NULL),
	size(0),
	underlined(false),
	shadow(false)
{ }

Font& Font::operator=(const Font& object)
{
	font = object.font;
	size = object.size;
	underlined = object.underlined;
	shadow = object.shadow;
	return(*this);
}

Font::Font(const Font& object) :
	font(object.font),
	size(object.size),
	underlined(object.underlined),
	shadow(object.shadow)
{ }



Font::~Font()
{
	if(font) 
	{	
		TTF_CloseFont(font);
		font = NULL;
	}
}

const Size Font::getTextExtent(const std::string& font_text) const
{
#ifdef _SCC_DEBUG
	if(font == NULL) {
		throw MyException("ERROR (Font::getTextExtent()): No font was initialized.");
	}
#endif

	int width;
	int height;
	int result = TTF_SizeUTF8(font, font_text.c_str(), &width, &height);

#ifdef _SCC_DEBUG
	if(result != 0) {
		throw MyException("ERROR (Font::getTextExtent()): Could not determine text dimensions with font [TTF ERROR: \"" + std::string(TTF_GetError()) + "\"].");
	}
#endif

	if(height < 2) {
		height = 0;
	} else {
		height -= 2;
	}
	return(Size((Uint16)width, (Uint16)height));
}


void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& font_text, const Sint16 x, const Sint16 y) const
{
	assert(surface);
	assert(Point(x, y) == Point(x, y));
	SDL_Surface *sText = TTF_RenderUTF8_Blended( font, font_text.c_str(), color);
#if _SCC_DEBUG
	if(sText == NULL) {
		throw MyException("ERROR (Font::DrawText()): Could not render blended text [TTF ERROR: \"" + std::string(TTF_GetError()) + "\"].");
	}
#endif
	if(sText) {
		SDL_Rect rcDest = {x, y-4, 0, 0};
		SDL_BlitSurface( sText, NULL, surface, &rcDest );
		SDL_FreeSurface( sText );
	}
}

void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const SDL_Color& back_color, const std::string& font_text, const Sint16 x, const Sint16 y) const // TODO
{
	assert(surface);
	assert(Point(x, y) == Point(x, y));
	SDL_Surface *sText = TTF_RenderUTF8_Shaded( font, font_text.c_str(), color, back_color);
#if _SCC_DEBUG
	if(sText == NULL) {
		throw MyException("ERROR (Font::DrawText()): Could not render shaded text [TTF ERROR: \"" + std::string(TTF_GetError()) + "\"].");
	}
#endif
	SDL_Rect rcDest = {x, y-4, 0, 0};
	SDL_BlitSurface( sText, NULL, surface, &rcDest );
	SDL_FreeSurface( sText );
}

