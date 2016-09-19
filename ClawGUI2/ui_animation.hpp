// g++ -o plaympeg plaympeg.c -I/usr/include/SDL -lSDL -lsmpeg
#ifdef _SMPEG
#ifndef _UI_ANIMATION_HPP
#define _UI_ANIMATION_HPP

#include "ui_bitmap.hpp"
#include "ui_statictext.hpp"
#include <animation.hpp>
#include <surface.hpp>

class UI_Animation : public UI_Object
{
	public:
		UI_Animation(SDL_Object* animation_parent,
						const Rect& animation_rect,
                        const Size& distance_bottom_right,
						const Animation* my_animation,
						const Bitmap* end_bitmap,
						const Surface* surface,
					//	const eString animation_text,
						const bool animation_border = false,
						const ePositionMode animation_position_mode = DO_NOT_ADJUST);

        ~UI_Animation();

        void reloadOriginalSize();
		UI_Object* checkToolTip(const Point& mouse) const;
		const SDL_Object* checkHighlight(const Point& mouse);
        void process();
        void draw(DC* dc) const;
		const bool setAnimationFile(const Animation* animation);
		const bool isPlaying() const;
		const bool isDone() const;
		void play();
		void pause();

	private:
		const Animation* animation;
		SDL_Surface* animationSurface;	
		SDL_Surface* currentSurface;
		UI_Bitmap* endBitmap;
//		UI_StaticText* animationText;
		bool loop;
		bool border;
		bool wasStarted;	
};

#endif // _UI_ANIMATION_HPP

#endif