#include "dc.hpp"

void DC::Draw_VLine_8bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const
{
	Uint8 pen_col = (Uint8)(pen.getColor());
	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0;
	register Sint16 i = y1-y0+1;
	Lock();

	if(pen.getStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;i--;)
		{
			if(((i%5) != 2)&&((i%5) != 3))
				*p = pen_col;
			p+=surface->pitch;
		}
	} else
	{
		switch( i % 4 ) {					
			do{								 
				case 0: *p = pen_col; p+=surface->pitch;
				case 3: *p = pen_col; p+=surface->pitch;
				case 2: *p = pen_col; p+=surface->pitch;
				case 1: *p = pen_col; p+=surface->pitch;
			}while( (i-=4) > 0 );
		}
	}
	Unlock();
}

void DC::Draw_HLine_8bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const
{
	Uint32 col = (Uint32)(pen.getColor());
	register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + x0;
	memset(p, col, x1-x0+1);
}

void DC::Draw_Line_8bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const
{
	Sint16 dx = x1 - x0 + 1;
	Sint16 dy = y1 - y0;
	Sint16 sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	Sint16 pixx = 1;
	Sint16 pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * x0 + pixy * y0;
	pixy *= sy;
	if (dx < dy) {
		Sint16 swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}

	Uint8 col = (Uint8)(pen.getColor());
	register Sint16 x = 0;
	register Sint16 y = 0;
	
	Lock();
	for (; x < dx; ++x, pixel += pixx) {
		*pixel = col;
		y += dy;
		if (y >= dx) {
			y -= dx;
			pixel += pixy;
		}
	}
	Unlock();
}

void DC::DrawEmptyRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	Uint8 col = (Uint8)(pen.getColor());

	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels +	y	* surface->pitch + Xcenter;
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter;
		register Sint16 i = dx;
		switch( i % 4 ) { 
			do{			
				case 0:	 
					*p0 = col; ++p0;		 
					*p1 = col; ++p1;		
				case 3:								
					*p0 = col; ++p0;		
					*p1 = col; ++p1;	 
				case 2:						 
					*p0 = col; ++p0;	 
					*p1 = col; ++p1;
				case 1:						
					*p0 = col; ++p0;
					*p1 = col; ++p1;
			}while( (i-=4) > 0 );			
		}											 
	}
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1);
		register Sint16 i = dy;
		switch( i % 4 ) {							 
			do{										
				case 0:								 
					*p0 = col; p0+=surface->pitch;
					*p1 = col; p1+=surface->pitch;
				case 3:								 
					*p0 = col; p0+=surface->pitch;
					*p1 = col; p1+=surface->pitch;
				case 2:								 
					*p0 = col; p0+=surface->pitch;
					*p1 = col; p1+=surface->pitch;
				case 1:								 
					*p0 = col; p0+=surface->pitch;
					*p1 = col; p1+=surface->pitch;
			}while( (i-=4) > 0 );					 
		}
	}		

//	rounded corners 
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner<<2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;

		while (i <= radius) 
		{
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i))) = col;

			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius))) = col;

			*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius))) = col;

			*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter - i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius))) = col;

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

void DC::DrawFilledRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	Uint8 col = (Uint8)(brush.getColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
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
// TODO optimieren evtl 4 bytes zusammenfassen
// Nur Ecken:
		for(Uint16 k = i; k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k+ (Ycenter-radius)*surface->pitch + (Xcenter - i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-radius)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + (Xcenter-i))) = col;

		}
		for(Uint16 k = radius; k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + (Xcenter-radius))) = col;
		}
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
	}/*while*/
	Unlock();
}

void DC::DrawEmptyEdgedRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	Uint8 pen_col = (Uint8)(pen.getColor());

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		memset((Uint8*)surface->pixels + y * surface->pitch + Xcenter, pen_col, dx);
		memset((Uint8*)surface->pixels + (y+height-1) * surface->pitch + x, pen_col, dx+corner);
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;
		Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1);
		Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint8*)p0 = pen_col;
			*(Uint8*)p1 = pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch)
			*(Uint8*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner<<2);
	Sint16 d = 3 - (corner<<1);
	Uint16 radius = corner;
	Uint16 i = 0;	
	
	while (i <= radius) 
	{
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + Xcenter - corner)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + Xcenter - corner)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + X2center + i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + X2center + radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+corner)*surface->pitch + Xcenter - radius)) = pen_col; // new
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+corner)*surface->pitch + Xcenter - i)) = pen_col; // new

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

}/*Draw_Round*/

void DC::DrawFilledEdgedRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	Uint8 brush_col;
	if(!pressedRectangle)
		brush_col = (Uint8)(brush.getColor());
	else brush_col = (Uint8)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
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

	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner<<2);
	Sint16 d = 3 - (corner<<1);
	Uint16 radius = corner - 1;
	Uint16 i = 0;
	Lock();
// Halbkreis
	while (i <= radius) 
	{
		for(Uint16 k = i; k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-radius)*surface->pitch + (Xcenter - i))) = brush_col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-radius)*surface->pitch + X2center)) = brush_col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Y2center+radius)*surface->pitch + X2center)) = brush_col;
		}
		for(Uint16 k = radius; k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-i)*surface->pitch + (Xcenter - radius))) = brush_col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-i)*surface->pitch + X2center)) = brush_col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Y2center+i)*surface->pitch + X2center)) = brush_col;
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


void DC::DrawFilledEdgedBorderRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	Uint8 brush_col;
	if(!pressedRectangle) {
		brush_col = (Uint8)(brush.getColor());
	}
	else {
		brush_col = (Uint8)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
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


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint8 dark_pen_col, bright_pen_col, pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), PRESSED_DARKEN));
		bright_pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), PRESSED_BRIGHTEN));
		pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), PRESSED_NORMAL));
	
	} else
	{
		dark_pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
		bright_pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_BRIGHTEN));
		pen_col = (Uint8)(pen.getColor());
	}
	
	Lock();
	{
		memset((Uint8*)surface->pixels + y * surface->pitch + Xcenter, bright_pen_col, dx);
		memset((Uint8*)surface->pixels + (y+height-1) * surface->pitch + x, dark_pen_col, dx+corner);
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x+width-1;
		register Uint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*p0 = bright_pen_col;
			*p1 = dark_pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch) {
			*p0 = bright_pen_col;	
		}
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner<<2);
		Sint16 d = 3 - (corner<<1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{
			for(Uint16 k = i; k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + Xcenter - i)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + X2center)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + k + X2center)) = brush_col;
			}
			for(Uint16 k = radius; k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + Xcenter - radius)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + X2center)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + k + X2center)) = brush_col;
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
		Sint16 d = 3 - (corner<<1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = bright_pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = bright_pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + X2center + i)) = dark_pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + X2center + radius)) = dark_pen_col;

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


void DC::DrawTab_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const
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
	Uint8 brush_col;
	if(pressedRectangle)
		brush_col = (Uint8)(brush.getColor());
	else brush_col = (Uint8)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; 
		r.y = Ycenter;
		r.w = width-1; 
		r.h = dy + corner;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; 
		r.y = y+1;
		r.w = dx; 
		r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint8 dark_pen_col, bright_pen_col, pen_col;
/*	if(pressedRectangle)
	{
		dark_pen_col = (Uint8)(changeRelativeBrightness(*pen.getColor(), PRESSED_DARKEN));
		bright_pen_col = (Uint8)(changeRelativeBrightness(*pen.getColor(), PRESSED_BRIGHTEN));
		pen_col = (Uint8)(changeRelativeBrightness(*pen.getColor(), PRESSED_NORMAL));
	
	} else*/
	{
		dark_pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));
		bright_pen_col = (Uint8)(pen.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_BRIGHTEN));
		pen_col = (Uint8)(pen.getColor());
	}
	
	Lock();
	{
		memset((Uint8*)surface->pixels + y * surface->pitch + Xcenter, bright_pen_col, dx);
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x+width-1;
		register Uint16 i = dy + corner;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*p0 = bright_pen_col;
			*p1 = dark_pen_col;
		}
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner<<2);
		Sint16 d = 3 - (corner<<1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{
			for(Uint16 k = i; k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + Xcenter - i)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + X2center)) = brush_col;
			}
			for(Uint16 k = radius; k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + Xcenter - radius)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + X2center)) = brush_col;
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
		Sint16 d = 3 - (corner<<1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius) 
		{
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = bright_pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = bright_pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

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
