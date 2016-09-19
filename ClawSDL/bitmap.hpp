#ifndef _SDL_BITMAP_HPP
#define _SDL_BITMAP_HPP

#include <resource.hpp>
#include <misc.hpp>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#define DC_API
/*#ifdef DC_EXPORTS
    #define DC_API __declspec(dllexport)
#else
    #define DC_API __declspec(dllimport)
#endif*/

class DC_API Bitmap : public Resource
{
	public:
		Bitmap(const std::string& file);
		~Bitmap();
		operator SDL_Surface*() const;
		SDL_Surface* operator->() const;
	private:
		SDL_Surface* bitmap;
		std::string name;
	
/*		bool scale;
		bool used;
		bool solid;*/
};



inline Bitmap::operator SDL_Surface*() const { 
	return bitmap;
}
		
inline SDL_Surface* Bitmap::operator->() const {
	return bitmap;
}

#endif