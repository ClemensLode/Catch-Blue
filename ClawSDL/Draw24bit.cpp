#include "dc.hpp"

// for 24bit:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_DRAW_PUTPIXEL_BPP_3_AUX \
	{ \
		p[0] = colorbyte2;					 \
		p[1] = colorbyte1;					 \
		p[2] = colorbyte0;					 \
	}
#else
#define SDL_DRAW_PUTPIXEL_BPP_3_AUX \
    { \
		p[0] = colorbyte0;					 \
		p[1] = colorbyte1;					 \
		p[2] = colorbyte2;					 \
	}
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX \
    { \
		p[0] = bright_colorbyte2;					 \
		p[1] = bright_colorbyte1;					 \
		p[2] = bright_colorbyte0;					 \
	}
#else
#define SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX \
    { \
		p[0] = bright_colorbyte0;					 \
		p[1] = bright_colorbyte1;					 \
		p[2] = bright_colorbyte2;					 \
	}
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX \
	{ \
		p[0] = dark_colorbyte2;					 \
		p[1] = dark_colorbyte1;					 \
		p[2] = dark_colorbyte0;					 \
	}
#else
#define SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX \
	{ \
		p[0] = dark_colorbyte0;					 \
		p[1] = dark_colorbyte1;					 \
		p[2] = dark_colorbyte2;					 \
	}
#endif

#define SDL_DO_DRAWING_X if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}
#define SDL_DO_BRIGHT_DRAWING_X if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}
#define SDL_DO_DARK_DRAWING_X if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}

#define SDL_DO_DRAWING_Y if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}
#define SDL_DO_BRIGHT_DRAWING_Y if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}
#define SDL_DO_DARK_DRAWING_Y if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}


void DC::Draw_VLine_24bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const
{
	Uint32 pen_col = (Uint32)(pen.getColor());
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 3;
	register Sint16 k = y1-y0+1;

	Lock();
	if(pen.getStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;k--;)
		{
			if(((k%5) != 2) && ((k%5) != 3))
			{
				SDL_DRAW_PUTPIXEL_BPP_3_AUX
			}
			p+=surface->pitch;
		}
	} else
	{
		SDL_DO_DRAWING_Y
	}
	Unlock();
}


void DC::Draw_HLine_24bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const
{
	Color col = pen.getColor();
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 3;
	register Sint16 k = x1-x0+1;

	Lock();
	
	SDL_DO_DRAWING_X
	
	Unlock();
}


void DC::Draw_Line_24bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const
{
	Sint16 dx = x1 - x0 + 1;
	Sint16 dy = y1 - y0;
	Sint16 sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	Sint16 pixx = 3;
	Sint16 pixy = surface->pitch;
	Uint8* p = ((Uint8 *) surface->pixels) + pixx * x0 + pixy * y0;
	pixy *= sy;
	if (dx < dy) {
		Sint16 swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}
// TODO?!
	Color col = pen.getColor();
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
	register Sint16 x = 0;
	register Sint16 y = 0;

	Lock();
	for (; x < dx; ++x, p += pixx) {
		
		SDL_DRAW_PUTPIXEL_BPP_3_AUX
			
		y += dy;
		if (y >= dx) {
			y -= dx;
			p += pixy;
		}
	}
	Unlock();
}

void DC::DrawEmptyRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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

	Color col = pen.getColor();
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = dy;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	Uint16 rightInc = 6;
	Sint16 d = 3 - (corner << 1);
	Sint16 diagonalInc = 10 - (corner<<2);	
	Uint16 radius = corner;
	Uint16 i = 0;
	
	register Uint8* p;
	while (i <= radius) 
	{
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX

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
	Unlock();
}


void DC::DrawFilledRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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

	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner << 2);
	Sint16 d = 3 - (corner << 1);
	Uint16 radius = corner;
	Uint16 i = 0;
	
	Lock();	
	while (i <= radius) 
	{
		register Uint8* p;
		register Sint16 k;

		p = (Uint8*)surface->pixels + (Ycenter - i) * surface->pitch + (Xcenter - radius) * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter - radius) * surface->pitch + (Xcenter - i) * 3;k = i;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Ycenter - radius) * surface->pitch + (X2center + i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter - i) * surface->pitch + (X2center + radius) * 3;k = radius;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center + radius) * surface->pitch + (X2center - i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center + i) * surface->pitch + (X2center + radius) * 3;k = radius;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center + radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center + i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
	
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


void DC::DrawEmptyEdgedRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	Lock();
	{
		register Uint8* p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3; k = dx+corner-1; SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3; k = dx; SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = corner+dy-1;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner<<2);
	Sint16 d = 3 - (corner<<1);
	Uint16 radius = corner;
	Uint16 i = 0;
	register Uint8* p;
	
	while (i <= radius) 
	{
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center+i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center+radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Y2center + radius)*surface->pitch + (X2center+i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center + i)*surface->pitch + (X2center+radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX

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
	
	Unlock();
}


void DC::DrawFilledEdgedRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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

// Rechteck entlang der oberen und entlang der unteren Kante
	if( (corner > 0) && (dx > 0) )
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}

	Uint8 colorbyte0 = (Uint8) (brush_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((brush_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((brush_col >> 16) & 0xff);

	Uint16 rightInc = 6;
	Sint16 diagonalInc = 10 - (corner<<2);
	Sint16 d = 3 - (corner<<1);
	Uint16 radius = corner - 1;
	Uint16 i = 0;

	Lock();
	while (i <= radius) 
	{
		Uint8* p;
		Sint16 k;
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter+radius) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center-i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center-i) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+i) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X

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


void DC::DrawFilledEdgedBorderRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const
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
	if(!pressedRectangle)
		brush_col = (Uint32)(brush.getColor());
	else brush_col = (Uint32)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));

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

//	Uint32 pen_col = (Uint32)(*pen.getColor());

	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);
	
	Uint8 dark_colorbyte0 = (Uint8) (dark_pen_col & 0xff);
	Uint8 dark_colorbyte1 = (Uint8) ((dark_pen_col >> 8) & 0xff);
	Uint8 dark_colorbyte2 = (Uint8) ((dark_pen_col >> 16) & 0xff);
	
	Uint8 bright_colorbyte0 = (Uint8) (bright_pen_col & 0xff);
	Uint8 bright_colorbyte1 = (Uint8) ((bright_pen_col >> 8) & 0xff);
	Uint8 bright_colorbyte2 = (Uint8) ((bright_pen_col >> 16) & 0xff);

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_BRIGHT_DRAWING_X
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3;k = dx+corner;SDL_DO_DARK_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register Sint16 k;
		
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = height-corner-1;SDL_DO_BRIGHT_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DARK_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------


// Halbkreis

	colorbyte0 = (Uint8) (brush_col & 0xff);
	colorbyte1 = (Uint8) ((brush_col >> 8) & 0xff);
	colorbyte2 = (Uint8) ((brush_col >> 16) & 0xff);
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner << 2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius)
		{
			register Uint8 *p;
			register Sint16 k;
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (X2center) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+radius) * surface->pitch + (X2center) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (Xcenter-radius) * 3; k = radius; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (X2center) * 3; k = radius; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+i) * surface->pitch + (X2center) * 3; k = radius; SDL_DO_DRAWING_X

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

	colorbyte0 = (Uint8) (pen_col & 0xff);
	colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner << 2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		register Uint8 *p;
	
		Uint16 i = 0;
		while (i <= radius) 
		{
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX
			
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			
			p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX
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

void DC::DrawTab_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const
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
	Sint16 Y2center = Ycenter + dy - 1;
	Uint32 brush_col;
	if(pressedRectangle)
		brush_col = (Uint32)(brush.getColor());
	else brush_col = (Uint32)(brush.getColor().changeRelativeBrightness(this->getSurface(), NOT_PRESSED_DARKEN));

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

//	Uint32 pen_col = (Uint32)(*pen.getColor());

	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);
	
	Uint8 dark_colorbyte0 = (Uint8) (dark_pen_col & 0xff);
	Uint8 dark_colorbyte1 = (Uint8) ((dark_pen_col >> 8) & 0xff);
	Uint8 dark_colorbyte2 = (Uint8) ((dark_pen_col >> 16) & 0xff);
	
	Uint8 bright_colorbyte0 = (Uint8) (bright_pen_col & 0xff);
	Uint8 bright_colorbyte1 = (Uint8) ((bright_pen_col >> 8) & 0xff);
	Uint8 bright_colorbyte2 = (Uint8) ((bright_pen_col >> 16) & 0xff);

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_BRIGHT_DRAWING_X
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3;k = dx+corner;SDL_DO_DARK_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register Sint16 k;
		
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = height-corner-1;SDL_DO_BRIGHT_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DARK_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------


// Halbkreis

	colorbyte0 = (Uint8) (brush_col & 0xff);
	colorbyte1 = (Uint8) ((brush_col >> 8) & 0xff);
	colorbyte2 = (Uint8) ((brush_col >> 16) & 0xff);
	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner << 2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		Uint16 i = 0;
		
		while (i <= radius)
		{
			register Uint8 *p;
			register Sint16 k;
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (X2center) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+radius) * surface->pitch + (X2center) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (Xcenter-radius) * 3; k = radius; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (X2center) * 3; k = radius; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+i) * surface->pitch + (X2center) * 3; k = radius; SDL_DO_DRAWING_X

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

	colorbyte0 = (Uint8) (pen_col & 0xff);
	colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	{
		Uint16 rightInc = 6;
		Sint16 diagonalInc = 10 - (corner << 2);
		Sint16 d = 3 - (corner << 1);
		Uint16 radius = corner;
		register Uint8 *p;
	
		Uint16 i = 0;
		while (i <= radius) 
		{
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX
			
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			
			p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX
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
