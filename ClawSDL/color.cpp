#include "color.hpp"

#include <misc.hpp>
#include <assert.h>

Color::Color(const Uint32 ucolor, const SDL_Color scolor):
	ucol(ucolor),
	scol(scolor)
{ }

Color::Color(const Color& col):
	ucol(col.ucol),
	scol(col.scol)
{ }

Color::Color(const SDL_Surface* const surface, const Uint8 red, const Uint8 green, const Uint8 blue) {
	assert(surface);
	ucol = SDL_MapRGB(surface->format, red, green, blue);
	scol.r = red;
	scol.g = green;
	scol.b = blue;
	scol.unused = 255;
}

Color::Color():
	ucol(0),
	scol()
{
	scol.r = 0;
	scol.g = 0;
	scol.b = 0;
	scol.unused = 0;
}

Color::~Color() { }

/*void Color::updateColor(const SDL_Surface* const surface)
{
	assert(surface);
	ucol = SDL_MapRGB(surface->format, scol.r, scol.g, scol.b);
}*/


const Color Color::mixColor(const SDL_Surface* const surface, const Color& other)  const
{
	return(Color(surface, 
		    (this->r() + other.r())/2,
			(this->g() + other.g())/2,
			(this->b() + other.b())/2));
}

const Color Color::mixColor(const SDL_Surface* const surface, const Color& other, const unsigned int gradient) const
{
#ifdef _SCC_DEBUG
	if(gradient > 100) {
		throw MyException("ERROR (Color::mixColor()): Parameter gradient is out of range.");
	}
#endif
	return(Color(surface, 
		(Uint8)((this->r()*gradient + other.r() * (100 - gradient))/100),
		(Uint8)((this->g()*gradient + other.g() * (100 - gradient))/100),
		(Uint8)((this->b()*gradient + other.b() * (100 - gradient))/100)));
}

const Color Color::changeAbsoluteBrightness(const SDL_Surface* const surface, const signed int brightness) const
{
#ifdef _SCC_DEBUG
	if(brightness > 255) {
		throw MyException("ERROR (Color::changeAbsoluteBrightness()): Parameter brightness is out of range.");
	}
#endif
	int r = this->r() + brightness;
	int g = this->g() + brightness;
	int b = this->b() + brightness;
	if(r > 255) { r = 255;}
	if(g > 255) { g = 255;}
	if(b > 255) { b = 255;}
	return Color(surface, (Uint8)r, (Uint8)g, (Uint8)b);
}

const Color Color::changeRelativeBrightness(const SDL_Surface* const surface, const unsigned int brightness_percent) const
{
	int r = this->r() * brightness_percent / 100; if(r>255) r = 255;
	int g = this->g() * brightness_percent / 100; if(g>255) g = 255;
	int b = this->b() * brightness_percent / 100; if(b>255) b = 255;
	return(Color(surface, (Uint8)r, (Uint8)g, (Uint8)b));
}