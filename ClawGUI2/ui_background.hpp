#ifndef _UI_BACKGROUND_HPP
#define _UI_BACKGROUND_HPP

#include "ui_object.hpp"

class UI_BackGround : public UI_Object
{
	public:
		UI_BackGround(const Size& size, SDL_Surface* background_bitmap = NULL);
		~UI_BackGround();

		void reloadOriginalSize();
		void process();
		void draw(DC* dc) const;
	private:
		SDL_Surface* backgroundBitmap;
};

#endif

