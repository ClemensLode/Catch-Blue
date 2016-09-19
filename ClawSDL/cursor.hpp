#ifndef _SDL_CURSOR_HPP
#define _SDL_CURSOR_HPP

#include "resource.hpp"
#include <SDL.h>

class Cursor : public Resource
{
	public:
		Cursor(char* xpm_image[]);
		~Cursor();
		operator const SDL_Cursor*() const {return cursor;}
		operator SDL_Cursor*() {return cursor;}
		const SDL_Cursor* operator->() const {return cursor;}
		SDL_Cursor* operator->() {return cursor;}
	private:
		SDL_Cursor* cursor;
};

#endif