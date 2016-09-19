#ifndef _SDL_SURFACE_HPP
#define _SDL_SURFACE_HPP

#include <SDL.h>
#include <resource.hpp>

class Surface : public Resource
{
	public:
		Surface(const SDL_Surface* surface);
		Surface();
		~Surface();
		operator const SDL_Surface*() const {return surface;}
	private:
		const SDL_Surface* surface;
};



#endif