#include "dc.hpp"

void DC::Draw_VLine_32bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const
{
	Uint32 pen_col = (Uint32)(pen.getColor());

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0 << 2);
	register Sint16 i = y1-y0+1;
	
	Lock();
	
	if(pen.getStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;i--;)
		{
			if(((i%5) != 2)&&((i%5) != 3))
				*(Uint32*)p = pen_col;
			p+=surface->pitch;
		}
	} else
	{
		for(;i--; p+=surface->pitch)
			*(Uint32*)p = pen_col;
	}
	
	Unlock();
}


void DC::Draw_HLine_32bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const
{
	Uint32 pen_col = (Uint32)(pen.getColor());

	Lock();

# pragma warning( push )
# pragma warning( disable:4127 )
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
# pragma warning( pop )

#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0 << 2)), pen_col, x1-x0+1);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0 << 2)), pen_col, x1-x0+1);
#endif
	} else 
	{
		register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0 << 2);
		register Sint16 i = x1-x0+1;
		for(;i--;p+=4)
			*(Uint32*)p = pen_col;
	}

	Unlock();
}




// -------- ------------ --------
// -------- FILLED ROUND --------
// -------- ------------ --------





void DC::DrawFilledRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
{
// Ecke links oben
	Sint16 Xcenter = x+corner;
	Sint16 Ycenter = y+corner;
// Kantenlaengen
	Uint16 dx = width - (corner << 1);
	Uint16 dy = height - (corner << 1);
// Ecke rechts unten
	Sint16 X2center = Xcenter + dx - 1;
	Sint16 Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(brush.getColor());

// FILLED
	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; 
		r.y = Ycenter;
		r.w = width-1; 
		r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; 
		r.y = y+1;
		r.w = dx; 
		r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}
	
	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner << 2);
	Sint16 d = 3 - (corner << 1);
	Uint16 radius = corner;
	Uint16 i = 0;
	
	Lock();
	while (i <= radius) 
	{

// Nur Ecken:
#ifdef __linux__
	// links oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i +1)<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius + 1)<<2) ), col, radius);
	// rechts oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center<<2) ), col, radius);
	// links unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((Xcenter - i + 1)<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - radius + 1)<<2) ), col, radius);
	// rechts unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center<<2) ), col, radius);
#elif __WIN32__
		for(Uint16 k=i; k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+ (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-radius)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + ((Xcenter-i)<<2) )) = col;

		}
		for(Uint16 k=radius; k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + ((Xcenter-radius)<<2) )) = col;
		}
#endif
		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	Unlock();

}

// -------- ----------------- -------- 
// -------- EMPTY EDGED ROUND -------- 
// -------- ----------------- -------- 




void DC::DrawEmptyEdgedRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
{
// Ecke links oben
	Sint16 Xcenter = x+corner;
	Sint16 Ycenter = y+corner;
// Kantenlaengen
	Uint16 dx = width - (corner << 1);
	Uint16 dy = height - (corner << 1);
// Ecke rechts unten
	Sint16 X2center = Xcenter + dx - 1;
	Sint16 Y2center = Ycenter + dy - 1;
	Uint32 pen_col = (Uint32)(pen.getColor());

// ------ TOP AND LOWER HORIZONTAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<2);
		Lock();
# pragma warning( push )
# pragma warning( disable:4127 )
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
# pragma warning( pop )

#ifdef __linux__
			wmemset((wchar_t*)p0, pen_col, dx);
			wmemset((wchar_t*)p1, pen_col, width - corner);
#elif __WIN32__
			memset((wchar_t*)p0, pen_col, dx);
			memset((wchar_t*)p1, pen_col, width - corner);
#endif
		} else 
		{
			register Uint16 i = corner;
			for(;i--;p1+=4)
				*(Uint32*)p1 = pen_col;
			i = dx;
			for(;i--;p0+=4,p1+=4)
			{
				*(Uint32*)p0 = pen_col;
				*(Uint32*)p1 = pen_col;
			}
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register Uint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint32*)p0 = pen_col;
			*(Uint32*)p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*(Uint32*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner<<2);
	Sint16 d = 3 - (corner<<1);
	Uint16 radius = corner;
	Uint16 i = 0;	
	
	while (i <= radius) 
	{
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2) )) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}


// -------- ------------ --------
// -------- FILLED EDGED --------
// -------- ------------ --------




void DC::DrawFilledEdgedRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
{
// Ecke links oben
	Sint16 Xcenter = x+corner;
	Sint16 Ycenter = y+corner;
// Kantenlaengen
	Uint16 dx = width - (corner << 1);
	Uint16 dy = height - (corner << 1);
// Ecke rechts unten
	Sint16 X2center = Xcenter + dx - 1;
	Sint16 Y2center = Ycenter + dy - 1;
	Uint32 brush_col;
	if(!pressedRectangle) {
		brush_col = (Uint32)(brush.getColor());
	}
	else {
		brush_col = (Uint32)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
	}


	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; 
		r.y = Ycenter;
		r.w = width-1; 
		r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; 
		r.y = y+1;
		r.w = dx; 
		r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; 
		r.y = Y2center;
		r.w = width-corner-1; 
		r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}
	
	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner << 2);
	Sint16 d = 3 - (corner << 2);
	Uint16 radius = corner - 1;
	Uint16 i = 0;

	Lock();
// Halbkreis
	while (i <= radius) 
	{
// Nur Ecken:
		for(Uint16 k=i; k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((k + X2center)<<2))) = brush_col;
		}
		for(Uint16 k=radius; k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((k + X2center)<<2))) = brush_col;
		}

		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else 
		{
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	Unlock();
}

// -------- ------------------- --------
// -------- FILLED EDGED BORDER --------
// -------- ------------------- --------






void DC::DrawFilledEdgedBorderRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
{
// Ecke links oben
	Sint16 Xcenter = x+corner;
	Sint16 Ycenter = y+corner;
// Kantenlaengen
	Uint16 dx = width - (corner << 1);
	Uint16 dy = height - (corner << 1);
// Ecke rechts unten
	Sint16 X2center = Xcenter + dx - 1;
	Sint16 Y2center = Ycenter + dy - 1;

	Uint32 brush_col;
	if(!pressedRectangle) {
		brush_col = (Uint32)(brush.getColor());
	}
	else {
		brush_col = (Uint32)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
	}

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------
	Uint32 dark_pen_col, bright_pen_col, pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), PRESSED_DARKEN));
		bright_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), PRESSED_BRIGHTEN));
		pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), PRESSED_NORMAL));
	
	} else
	{
		dark_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
		bright_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_BRIGHTEN));
		pen_col = (Uint32)(pen.getColor());
	}
// ------ TOP AND LOWER HORIZONTAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<2);
		Lock();
# pragma warning( push )
# pragma warning( disable:4127 )
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
# pragma warning( pop )

#ifdef __linux__
			wmemset((wchar_t*)p0, bright_pen_col, dx);
			wmemset((wchar_t*)p1, dark_pen_col, width - corner);
#elif __WIN32__
			memset((wchar_t*)p0, bright_pen_col, dx);
			memset((wchar_t*)p1, dark_pen_col, width - corner);
#endif
		} else 
		{
			register Uint16 i = corner;
			for(; i--; p1+=4)
				*(Uint32*)p1 = dark_pen_col;
			i = dx;
			for(; i--; p0+=4,p1+=4)
			{
				*(Uint32*)p0 = bright_pen_col;
				*(Uint32*)p1 = dark_pen_col;
			}
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register Uint16 i = dy;

		for(; i--; p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint32*)p0 = bright_pen_col;
			*(Uint32*)p1 = dark_pen_col;
		}

		i = corner;
		for(; i--; p0+=surface->pitch) {
			*(Uint32*)p0 = bright_pen_col;
		}
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner << 2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{
			for(Uint16 k=i; k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
			}
			for(Uint16 k=radius; k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
			}
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner<<2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{	
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2))) = bright_pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2))) = bright_pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2))) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<2))) = dark_pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<2))) = dark_pen_col;

			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	Unlock();

}




// -------- --- --------
// -------- TAB --------
// -------- --- --------







void DC::DrawTab_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const
{
	Uint16 corner = 4;
// Ecke links oben
	Sint16 Xcenter = x+corner;
	Sint16 Ycenter = y+corner;
// Kantenlaengen
	Uint16 dx = width - (corner << 1);
	Uint16 dy = height - (corner << 1);
// Ecke rechts unten
	Sint16 X2center = Xcenter + dx - 1;
//	Sint16 Y2center = Ycenter + dy - 1;

	Uint32 brush_col;
	if(pressedRectangle) {
		brush_col = (Uint32)(brush.getColor());
	}
	else {
		brush_col = (Uint32)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
	}

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy + corner;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------
	Uint32 dark_pen_col, bright_pen_col, pen_col;
/*	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), PRESSED_DARKEN));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), PRESSED_BRIGHTEN));
		pen_col = (Uint32)(changeRelativeBrightness(*pen.getColor(), PRESSED_NORMAL));
	
	} else*/
	{
		dark_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
		bright_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_BRIGHTEN));
		pen_col = (Uint32)(pen.getColor());
	}
// ------ TOP AND LOWER HORIZONTAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		Lock();
# pragma warning( push )
# pragma warning( disable:4127 )
		if (sizeof(wchar_t) == sizeof(Uint32)) { 
# pragma warning( pop )

#ifdef __linux__
			wmemset((wchar_t*)p0, bright_pen_col, dx);
#elif __WIN32__
			memset((wchar_t*)p0, bright_pen_col, dx);
#endif
		} else 
		{
			register Uint16 i = dx;
			for(;i--;p0+=4)
				*(Uint32*)p0 = bright_pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register Uint16 i = dy + corner;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint32*)p0 = bright_pen_col;
			*(Uint32*)p1 = dark_pen_col;
		}
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner << 2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{
			for(Uint16 k=i; k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
			}
			for(Uint16 k=radius; k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
			}
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner<<2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{	
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2))) = bright_pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2))) = bright_pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2))) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	Unlock();

}




void DC::Draw_Line_32bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const
{
	Sint16 dx = x1 - x0 + 1;
	Sint16 dy = y1 - y0;
	Sint16 sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	Sint16 pixx = 4;
	Sint16 pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * x0 + pixy * y0;
	pixy *= sy;
	if (dx < dy) {
		Sint16 swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}

	Uint32 col = pen.getColor();
	register Sint16 x = 0;
	register Sint16 y = 0;

	Lock();
	for (; x < dx; ++x, pixel += pixx) {
		*(Uint32 *) pixel = (Uint32)col;
		y += dy;
		if (y >= dx) {
			y -= dx;
			pixel += pixy;
		}
	}
	Unlock();
}




// -------- ----------- --------
// -------- EMPTY ROUND --------
// -------- ----------- --------





void DC::DrawEmptyRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
{
// Ecke links oben
	Sint16 Xcenter = x+corner;
	Sint16 Ycenter = y+corner;
// Kantenlaengen
	Uint16 dx = width - (corner << 1);
	Uint16 dy = height - (corner << 1);
// Ecke rechts unten
	Sint16 X2center = Xcenter + dx - 1;
	Sint16 Y2center = Ycenter + dy - 1;
	Uint32 pen_col = pen.getColor();

	{
	 	register Uint8* p0 = (Uint8*)surface->pixels +	y	* surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (Xcenter<<2);
		Lock();
# pragma warning( push )
# pragma warning( disable:4127 )
		if (sizeof(wchar_t) == sizeof(Uint32))
# pragma warning( pop )

		{
#ifdef __linux__
			wmemset((wchar_t*)p0, pen_col, dx);
			wmemset((wchar_t*)p1, pen_col, dx);
#elif __WIN32__
			memset((wchar_t*)p0, pen_col, dx);
			memset((wchar_t*)p1, pen_col, dx);
#endif
		} else 
		{
			register Sint16 i = dx;				
			switch( i % 4 ) { 
				do{			
					case 0:	 
						*(Uint32*)p0 = pen_col; p0+=4;		 
						*(Uint32*)p1 = pen_col; p1+=4;		
					case 3:								
						*(Uint32*)p0 = pen_col; p0+=4;		
						*(Uint32*)p1 = pen_col; p1+=4;	 
					case 2:							 
						*(Uint32*)p0 = pen_col; p0+=4;	 
						*(Uint32*)p1 = pen_col; p1+=4;
					case 1:							
						*(Uint32*)p0 = pen_col; p0+=4;
						*(Uint32*)p1 = pen_col; p1+=4;
				}while( (i-=4) > 0 );			
			}
		}
	}
	{ 
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register Sint16 i = dy;
		switch( i % 4 ) {							 
			do{										
				case 0:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
				case 3:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
				case 2:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
				case 1:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
			}while( (i-=4) > 0 );					 
		}
	}	
	{
		Uint16 rightInc = 6;
		Sint16 d = 3 - (corner << 1);
		Sint16 diagonalInc = 10 - (corner<<2);	
		Uint16 radius = corner;
		Uint16 i = 0;	
		
		while (i <= radius) 
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2) )) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((Xcenter - i)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;

			if (d >= 0) {
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else {
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}		
	Unlock();

}
