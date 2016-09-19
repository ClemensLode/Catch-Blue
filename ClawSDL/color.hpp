#ifndef _SDL_COLOR_HPP
#define _SDL_COLOR_HPP

#include <SDL.h>
#include <resource.hpp>

#define DC_API
/*#ifdef DC_EXPORTS
    #define DC_API __declspec(dllexport)
#else
    #define DC_API __declspec(dllimport)
#endif*/

class DC_API Color : public Resource
{
	public:
		Color(const Uint32 ucolor, const SDL_Color scolor);
		Color(const SDL_Surface* const surface, const Uint8 red, const Uint8 green, const Uint8 blue);
		Color(const Color& col);
		Color();
		~Color();

		operator SDL_Color() const;
		operator Uint32() const;
		
		const Uint8 r() const;
		const Uint8 g() const;
		const Uint8 b() const;

		const Color mixColor(const SDL_Surface* const surface, const Color& other) const;
		const Color mixColor(const SDL_Surface* const surface, const Color& other, const unsigned int gradient) const;
		const Color changeAbsoluteBrightness(const SDL_Surface* const surface, const signed int brightness) const;
		const Color changeRelativeBrightness(const SDL_Surface* const surface, const unsigned int brightness_percent) const;

//		void updateColor(const SDL_Surface* const surface);
	private:	
		Uint32 ucol;
		SDL_Color scol;
};

inline Color::operator SDL_Color() const {
	return(scol);
}

inline Color::operator Uint32() const {
	return(ucol);
}

inline const Uint8 Color::r() const {
	return(scol.r);
}

inline const Uint8 Color::g() const {
	return(scol.g);
}

inline const Uint8 Color::b() const {
	return(scol.b);
}

#endif // _SDL_COLOR_HPP
