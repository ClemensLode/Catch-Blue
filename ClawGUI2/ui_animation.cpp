#ifdef _SMPEG
#include "ui_animation.hpp"

// TODO problem: einzelne Resource? Was wenn mehrmals auf dem Schirm dasselbe Video gespielt wird?

UI_Animation::UI_Animation(SDL_Object* animation_parent, 
				const Rect& animation_rect,
				const Size& distance_bottom_right, 
				const Animation* my_animation,
				const Bitmap* end_bitmap,
				const Surface* surface,
			//	const eString animation_text,
				const bool animation_border,
				const ePositionMode animation_position_mode) :
	UI_Object(animation_parent, animation_rect,  distance_bottom_right, animation_position_mode, NO_AUTO_SIZE),
	animation(my_animation),
	animationSurface(NULL),
	endBitmap(NULL),
	currentSurface(NULL),
//	animationText(NULL),
	loop(false),
	border(animation_border),
	wasStarted(false)
{
	endBitmap = new UI_Bitmap(this, Rect(), Size(), end_bitmap);
//	animationText = new UI_StaticText(this, animation_text, Rect(Point(0,-10),Size()), Size(), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT);

	endBitmap->Hide();
//	animationText->Hide();
	setAnimationFile(my_animation);
	setDrawType(SOLID_OBJECT);
}

UI_Animation::~UI_Animation()
{
//      SMPEG_stop(movie);
//	  SMPEG_delete(movie);
//	  movie = NULL;

	delete endBitmap;
//	delete animationText;
//	lockAnimation(false);
}

//void UI_Animation::lockAnimation(const bool lock)
//{
//	SMPEG* temp = gui.lookUpAnimation(id);
//	assert(temp);
	//??? TODO

//}


void update(SDL_Surface *screen)
{
	// TODO observer erstellen...
// => process
}

const bool UI_Animation::setAnimationFile(const Animation* animation)
{
	assert(animation);
//	if(isLocked(id)) {
//		return(false);
//	}
//	lockAnimation(false);
	this->animation = animation;

//	lockAnimation(true);
	if(animationSurface != NULL) {
		SDL_FreeSurface(animationSurface);
		animationSurface = NULL;
	}
#ifdef USE_SMPEG
	setSize(Size((Uint16)(animation->getInfo().width), (Uint16)(animation->getInfo().height)));	
#endif
	return(true);
}

const bool UI_Animation::isPlaying() const
{
	bool status = false;
#ifdef USE_SMPEG
	status = SMPEG_status(*animation) == SMPEG_PLAYING;
#endif
	return status;
}

const bool UI_Animation::isDone() const
{
	return((!isPlaying()) && wasStarted);
}

void UI_Animation::play()
{
#ifdef USE_SMPEG
	if(!isPlaying())
	{
		if(!wasStarted)
		{
			wasStarted = true;
			SMPEG_play(*animation);
		}
	}
#endif
}

void UI_Animation::pause()
{
#ifdef USE_SMPEG
	if(isPlaying()) {
		SMPEG_pause(*animation);
	}
#endif
}

/*			  // Toggle filtering on/off
			  if ( bilinear_filtering ) {
			    SMPEG_Filter *filter = SMPEGfilter_null();
			    filter = SMPEG_filter( mpeg, filter );
			    filter->destroy(filter);
			    bilinear_filtering = 0;
			  } else {
			    SMPEG_Filter *filter = SMPEGfilter_bilinear();
			    filter = SMPEG_filter( mpeg, filter );
			    filter->destroy(filter);
			    bilinear_filtering = 1;
			  }*/

//			    SMPEG_skip(mpeg, 50);
//			  SMPEG_setvolume(mpeg, volume);
		  
/*case SDL_VIDEORESIZE: {
                        SDL_Surface *old_screen = screen;
                        SMPEG_pause(mpeg);
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, screen->format->BitsPerPixel, screen->flags);
			if ( old_screen != screen ) {
                            SMPEG_setdisplay(mpeg, screen, NULL, update);
                        }
                        SMPEG_scaleXY(mpeg, screen->w, screen->h);
                        SMPEG_pause(mpeg);
                    } break;
*/


void UI_Animation::reloadOriginalSize()
{
//	setOriginalSize(getBitmapSize());
	UI_Object::reloadOriginalSize();
//	adjustSize(CHILD_WAS_CHANGED); // TODO
}

/*
void UI_Animation::setBrightness(const unsigned int bitmap_brightness) 
{
	if(brightness != bitmap_brightness)
	{
		brightness = bitmap_brightness;
		makePufferInvalid();
//		setNeedRedrawAllThatOverlaps(getRelativeRect());
//		toErrorLog(brightness);
	}
}*/
// hoechstens bei update aufrufen...
	

// Render button.  How it draws exactly depends on it's current state.
void UI_Animation::draw(DC* dc) const
{
	assert(dc);

            // Play the movie
   //         SMPEG_play( movie );
 
            // Draw the movie surface to the main screen at 0,0
      //      DrawIMG( movieSurface, 0, 0 );


//	if(clipRect != Rect())
//		dc->DrawBrightenedBitmap(gui.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap), Point(), clipRect, brightness);
//	else
//		dc->DrawBrightenedBitmap(gui.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap), Point(), brightness);
	UI_Object::draw(dc);
		
	// leeres / letztes Bild evtl

/*	if(border)
	{
		dc->setBrush(*gui.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*gui.lookUpPen(INNER_BORDER_PEN));
		dc->DrawEdgedRoundedRectangle(getRelativeRect(), 4);
		dc->setPen(*gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		dc->DrawEdgedRoundedRectangle(Rect(getRelativePosition() + Size(3,3), getSize() - Size(6,6)), 4);
	}*/

	if(animationSurface != NULL) {
		SDL_Rect dest = DC::createRect(getAbsoluteRect());
		SDL_BlitSurface(animationSurface, NULL, dc->getSurface(), &dest);
		SDL_UpdateRect(dc->getSurface(), dest.x, dest.y, dest.w, dest.h);
	}
	//TODO?
//		UI_Object::addNewRectToList(getAbsoluteRect(), getZ());

}

UI_Object* UI_Animation::checkToolTip(const Point& mouse) const {
	if( !isShown() || !getAbsoluteRect().isTopLeftCornerInside(mouse)) {
		return(NULL);
	}
	return((UI_Object*)this);
}

const SDL_Object* UI_Animation::checkHighlight(const Point& mouse)
{
	if(!isShown()) {
		return NULL;
	}
	return UI_Object::checkHighlight(mouse);
}

void UI_Animation::process()
{
	UI_Object::process();
	if(!isShown()) {
		return;
	}

	if(isPlaying()) {
//		animationText->Show();
		endBitmap->Hide();
	} else if(wasStarted && !isPlaying()) {
		endBitmap->Show();
	}

#ifdef USE_SMPEG
	if(animationSurface == NULL) {
		
	    const SDL_PixelFormat* pixel_format = currentSurface->format;

		SDL_Surface* temp = SDL_CreateRGBSurface( SDL_SWSURFACE, getWidth(), getHeight(), 32, pixel_format->Rmask, pixel_format->Gmask, pixel_format->Bmask, pixel_format->Amask );
		
		animationSurface = SDL_DisplayFormat( temp );
		SDL_FreeSurface(temp);
	
		SMPEG_setdisplay(animation->getSurface(), animationSurface, NULL, NULL);//update);
		SMPEG_scaleXY(animation->getSurface(), getWidth(), getHeight());
	//	SMPEG_setdisplayregion(animation, 50, 50, 100, 100);
/*
	      // Set the display region
      SMPEG_setdisplayregion( movie, 0, 0, moveInfo.width, moveInfo.height );
 
      // Loop forever
      SMPEG_loop( movie, -1 );
 
      SDL_ShowCursor(SDL_DISABLE);
*/

	/* Enable software bilinear filtering, if desired */
//	if ( bilinear_filtering ) {
//		SMPEG_Filter *filter;
//		filter = SMPEGfilter_bilinear();
///		filter = SMPEG_filter( mpeg, filter );
//		filter->destroy(filter);
//	}

        /* Set any special playback parameters */
//	if ( loop ) SMPEG_loop(mpeg, 1);

	/* Seek starting position */
//	if(seek) SMPEG_seek(mpeg, seek);

	/* Skip seconds to starting position */
//	if(skip) SMPEG_skip(mpeg, skip);
	}
#endif
}

#endif