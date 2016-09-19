#include "dc.hpp"

#include "string.h"
#include "wchar.h"


// ---- DRAWING

void DC::DrawBrightenedBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y, const Rect& clip_rect, const Uint8 brightness) const
{
//	if(brightness == 100)
//	{
		SDL_SetAlpha((SDL_Surface*)bitmap, SDL_SRCALPHA, (Uint8)(brightness * 255 / 100));
		DrawBitmap(bitmap, x, y, clip_rect);
		SDL_SetAlpha((SDL_Surface*)bitmap, SDL_SRCALPHA, 255);
		return;
//	}
//	double brightness_percent = brightness / 100;
/*	SDL_Surface* neu = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 8, 0, 0, 0, 0);
	Uint32* pc = (Uint32*)(bitmap->pixels);
	Uint32* dst = (Uint32*)(neu->pixels);

	for(unsigned int i = bitmap->w * bitmap->h; i--;)
	{
		Uint8 r,g,b;
		SDL_GetRGB(*pc, bitmap->format, &r, &g, &b);
//		r = r/2;// * brightness_percent; if(r > 255) r = 255;
//		g = g/2;// * brightness_percent; if(g > 255) g = 255;
//		b = b/2;// * brightness_percent; if(b > 255) b = 255;
		*dst = SDL_MapRGB(bitmap->format, r, g, b);
		++pc;
		++dst;
	}
	DrawBitmap(neu, x, y, clip_rect);
	SDL_FreeSurface(neu);*/
		//TODO
}

void DC::DrawBrightenedBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y, const Uint8 brightness) const
{
//	if(brightness == 100)
//	{
		SDL_SetAlpha((SDL_Surface*)bitmap, SDL_SRCALPHA, (Uint8)(brightness * 255 / 100));
		DrawBitmap(bitmap, x, y);
		SDL_SetAlpha((SDL_Surface*)bitmap, SDL_SRCALPHA, 255);
		return;
//	}

//	double brightness_percent = brightness / 100;
/*	SDL_Surface* neu = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 8, 0, 0, 0, 0);
	Uint32* pc = (Uint32*)(bitmap->pixels);
	Uint32* dst = (Uint32*)(neu->pixels);

	for(unsigned int i = bitmap->w * bitmap->h; i--;)
	{
		Uint8 r,g,b;
		SDL_GetRGB(*pc, bitmap->format, &r, &g, &b);
//		r = r /2;//* brightness_percent; if(r > 255) r = 255;
//		g = g /2;//* brightness_percent; if(g > 255) g = 255;
//		b = b /2;//* brightness_percent; if(b > 255) b = 255;
//		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;
		*dst = SDL_MapRGB(bitmap->format, r, g, b);
		++pc;
		++dst;
	}
	DrawBitmap(neu, x, y);
	SDL_FreeSurface(neu);*/
}

void DC::DrawBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y) const
{
	if((x + bitmap->w >= getMaxX())||(y + bitmap->h >= getMaxY()) || (x < 0) || (y < 0))
	{
		DrawBitmap(bitmap, x, y, Rect(0, 0, getMaxX(), getMaxY()));
		return;
	}
	SDL_Rect drect = createRect(x, y, 0, 0);
	SDL_BlitSurface((SDL_Surface*)bitmap , NULL, surface, &drect);
}


// clipped bitmap
void DC::DrawBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y, const Rect& clip_rect) const
{
	SDL_Rect srect = createRect(0, 0, (Uint16)(bitmap->w), (Uint16)(bitmap->h));
	SDL_Rect drect = createRect(x, y, 0, 0);

	if((clip_rect.getLeft() > x + srect.w) || (clip_rect.getTop() > y + srect.h) || (clip_rect.getBottom() < y) || (clip_rect.getRight() < x)) {
		return;
	}
	if(clip_rect.getLeft() > x)
	{
		srect.x = clip_rect.getLeft() - x;
		srect.w -= srect.x;
		drect.x = clip_rect.getLeft();
	}
	if(clip_rect.getTop() > y)
	{
		srect.y = clip_rect.getTop() - y;
		srect.h -= srect.y;
		drect.y = clip_rect.getTop();
	}
	if(clip_rect.getRight() < x + srect.w) {
		srect.w = clip_rect.getRight() - x;
	}	
	if(clip_rect.getBottom() < y + srect.h) {
		srect.h = clip_rect.getBottom() - y;
	}
	
	SDL_BlitSurface((SDL_Surface*)bitmap , &srect, surface, &drect);
}

void DC::DrawLine(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const
{
	if(pen.getStyle()==TRANSPARENT_PEN_STYLE) {
		return;
	}
	Sint16 xx1, xx2, yy1, yy2;
	if(x0>x1) {
		xx1=x1;xx2=x0;yy1=y1;yy2=y0;
	} else {
		xx1=x0;xx2=x1;yy1=y0;yy2=y1;
	}

	if((xx1<pen.getWidth()/2) || (yy1+1<pen.getWidth()/2) || (yy2<pen.getWidth()/2) ||
		(xx2>=getMaxX() - pen.getWidth()/2)||(yy1>=getMaxY()-pen.getWidth()/2)||(yy2>=getMaxY()-pen.getWidth()/2)) {
		return;
	}
		
	if(yy1==yy2)
	{
		if(xx1==xx2) {
			return;
		}
		if(pen.getWidth()>1)
		{
			SDL_Rect rc = createRect(xx1/*-pen.getWidth()/2*/, yy1/*-pen.getWidth()/2*/, xx2 - xx1/*+pen.getWidth()*/, pen.getWidth());
			SDL_FillRect(surface, &rc, (Uint32)(pen.getColor()) );
		} else {
			(*this.*Draw_HLine)(xx1, yy1, xx2);
		}
	} else
	if(xx1==xx2)
	{
		if(yy1>yy2) {
			{Sint16 swp=yy1;yy1=yy2;yy2=swp;}
		}
		if(pen.getWidth()>1)
		{
			SDL_Rect rc = createRect(xx1/*-pen.getWidth()/2*/, yy1/*-pen.getWidth()/2*/, pen.getWidth(), yy2-yy1/*+pen.getWidth()*/);
			SDL_FillRect(surface, &rc, (Uint32)(pen.getColor()) );
		} else {
			(*this.*Draw_VLine)(xx1, yy1, yy2);
		}
	} else
	{
		(*this.*Draw_Line)(xx1, yy1, xx2, yy2);
		if(pen.getWidth()==2)
		{
//			Color c = *pen.getColor();
//			const_cast<DC*>(this)->pen.setColor(Color(surface, (Uint8)(pen.getColor()->r()*0.5),  (Uint8)(pen.getColor()->g()*0.5), (Uint8)(pen.getColor()->b()*0.5)));
			(*this.*Draw_Line)(xx1, yy1+1, xx2, yy2+1);
			(*this.*Draw_Line)(xx1, yy1, xx2, yy2);
			(*this.*Draw_Line)(xx1, yy1-1, xx2, yy2-1);
//			const_cast<DC*>(this)->pen.setColor(c);
		}
	}
}

// TODO dp!
void DC::DrawSpline(const unsigned int c, const Point* p) const
{
	if((pen.getStyle() == TRANSPARENT_PEN_STYLE)||(c<2)) {
		return;
	}
	for(unsigned int i=c-1;i--;)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.getColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.getColor()));
//	}
	{
/*		if((i>0)&&(i<c-2))
		{
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else*/
		{
			DrawLine(dp.getX() + p[i].getX(),   dp.getY() + p[i].getY(), 
					 dp.getX() + p[i+1].getX(), dp.getY() + p[i+1].getY());
//			DrawLine(p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1);
		}
	}
}
	
void DC::DrawSpline(const unsigned int c, const Point* p, const Point s) const
{
	if((pen.getStyle() == TRANSPARENT_PEN_STYLE)||(c<2)) {
		return;
	}
	Point sp = s + dp;
	for(unsigned int i=c-1;i--;)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.getColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.getColor()));
//	}
	{
/*		if((i>0)&&(i<c-2))
		{
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else*/
		{
			DrawLine(sp.getX() + p[i].getX(),   sp.getY() + p[i].getY(), 
				     sp.getX() + p[i+1].getX(), sp.getY() + p[i+1].getY());
	//		DrawLine(s.x + p[i].x, s.y + p[i].y+1, s.x + p[i+1].x, s.y + p[i+1].y+1);
		}
	}
	
}
void DC::DrawText(const std::string& text, const Sint16 x, const Sint16 y) const 
{
	if(font.isShadow()) {
		font.DrawText(surface, toSDL_Color(0,0,0), text, x+font.getSize()/6, y+font.getSize()/6);
	}

	font.DrawText(surface, textColor, text, x, y);
	if(font.isUnderlined())
	{
		Size s = font.getTextExtent(text);
		SDL_Rect r = createRect(x, y+s.getHeight()*2/3, s.getWidth(), 1);
		SDL_FillRect(surface, &r, Color(surface, textColor.r, textColor.g, textColor.b));
	}

}



#if 0
void DC::DrawGridEdgedRoundedRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 int radius, std::list<Rect> notDrawRectList) const 
{
	return;
	if((width<2)||(height<2)) return;
	if((x>=getMaxX())||(x<0)) return;
	if((y>=getMaxY())||(y<0)) return;
	unsigned int ww = width;
	unsigned int hh = height;
	if(x+ww>=getMaxX()) ww = getMaxX() - 1 - x;
	if(y+hh>=getMaxY()) hh = getMaxY() - 1 - y;
	bool rounded = false;
	if((radius <= 1)||(ww<2*radius)||(hh<2*radius))
		rounded=true;

	signed int xx = x;
	signed int yy = y;
	unsigned int lastHeight = 0;

	std::list<Rect>::const_iterator i = notDrawRectList.begin();
	while(i!=notDrawRectList.end())
	{
		lastHeight=0;
		while((i!=notDrawRectList.end())&&(yy == i->getTop()))
		{
			DrawRectangle(xx, yy, i->getLeft() - xx, i->getHeight());
			lastHeight = i->getHeight();
			xx = i->getLeft() + i->getWidth();
			i++;
		}
		// rechter Rand
		DrawRectangle(xx, yy, x+ww - xx, lastHeight);
		// neue Zeile
		xx = x;
		yy += lastHeight;
		if((i!=notDrawRectList.end())&&(yy < i->getTop()))
		{
			DrawRectangle(xx, yy, ww, i->getTop() - yy);
			yy = i->getTop();
			xx = x;
		}
	}
	DrawRectangle(xx, yy, x+ww - xx, y + hh - yy);		
				
/*	if(rounded) {
		DrawRectangle(x,y,ww,hh);
		return;
	}
	
	if (brush.getStyle() != TRANSPARENT_BRUSH_STYLE)
		DrawFilledEdgedRound(x, y, ww, hh, radius);
	if (pen.getStyle() != TRANSPARENT_PEN_STYLE)
		DrawEmptyEdgedRound(x, y, ww, hh, radius);*/
}
#endif

void DC::DrawEdgedRoundedRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 radius) const
{
	if((width<2)||(height<2)||(x>=getMaxX())||(y>=getMaxY())||(x<0)||(y<0)) {
		return;
	}
	
	Uint16 mw = width;
	Uint16 mh = height;

	if(x + width >= getMaxX()) {
		mw = getMaxX() - x;
	}
	if(y + height >= getMaxY()) {
		mh = getMaxY() - y;
	}
	
	if((radius <= 1)||(mw<2*radius)||(mh<2*radius))	{
		DrawRectangle(x,y,mw,mh);
		return;
	}

	if(brush.getStyle() == TRANSPARENT_BRUSH_STYLE)
	{
		if(pen.getStyle() != TRANSPARENT_PEN_STYLE) {
			(*this.*DrawEmptyEdgedRound)(x, y, mw, mh, radius);
		}
	} else if(pen.getStyle() == TRANSPARENT_PEN_STYLE) {
		(*this.*DrawFilledEdgedRound)(x, y, mw, mh, radius);
	} else {
		(*this.*DrawFilledEdgedBorderRound)(x, y, mw, mh, radius); 
	}
}

void DC::DrawRoundedRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 radius) const
{
	if((width<2)||(height<2)||(x+width>=getMaxX())||(y+height>=getMaxY())||(x<0)||(y<0)) {
		return;
	}
	
	if((radius <= 1)||(width<2*radius)||(height<2*radius))	{
		DrawRectangle(x,y, width, height);
		return;
	}

	if (brush.getStyle() != TRANSPARENT_BRUSH_STYLE) {
		(*this.*DrawFilledRound)(x, y, width, height, radius);
	}
	if (pen.getStyle() != TRANSPARENT_PEN_STYLE) {
		(*this.*DrawEmptyRound)(x, y, width, height, radius);
	}
}


void DC::DrawTabRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const
{
	if((width<2)||(height<2)||(x+width>=getMaxX())||(y+height>=getMaxY())||(x<0)||(y<0)) {
		return;
	}
	
	(*this.*DrawTab)(x, y, width, height);
}


void DC::DrawRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const
{
	if((width<2)||(height<2)||(width>getMaxX())||(height>getMaxY())||(x<0)||(y<0)) {
		return;
	}

	if(brush.getStyle() != TRANSPARENT_BRUSH_STYLE) {
		SDL_Rect rc = createRect(x+pen.getWidth(), y+pen.getWidth(), width-2*pen.getWidth(), height-2*pen.getWidth());
		SDL_FillRect(surface, &rc, (Uint32)(brush.getColor()) );
	}
	DrawEmptyRectangle(x, y, width, height);
}

void DC::DrawEmptyRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const
{
	if(pen.getStyle()==TRANSPARENT_PEN_STYLE)  {
		return;
	}
	if((width<2)||(height<2)||(width>getMaxX())||(height>getMaxY())||(x<0)||(y<0)) {
		return;
	}
	
	Uint32 dark_pen_col, bright_pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), 110));
		bright_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), 70));
	
	} else
	{
		dark_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), 80));
		bright_pen_col = (Uint32)(pen.getColor().changeRelativeBrightness(this->getSurface(), 120));
	}

	SDL_Rect rc = createRect(x-(pen.getWidth()>>1), y-(pen.getWidth()>>1), width, pen.getWidth());
	SDL_FillRect(surface, &rc, bright_pen_col);
	
	rc = createRect(x-(pen.getWidth()>>1), y+height-1-(pen.getWidth()>>1), width, pen.getWidth());
	SDL_FillRect(surface, &rc, dark_pen_col);
	
	rc = createRect(x-(pen.getWidth()>>1), y-(pen.getWidth()>>1), pen.getWidth(), height);
	SDL_FillRect(surface, &rc, bright_pen_col);
	
	rc = createRect(x+width-1-(pen.getWidth()>>1), y-(pen.getWidth()>>1), pen.getWidth(), height);
	SDL_FillRect(surface, &rc, dark_pen_col);
}

void DC::DrawVerticalLine(const Sint16 x0, const Sint16 y0, const Sint16 y1) const
{
	if((y1<0)||(y0>=getMaxY())||(x0<0)||(x0>=getMaxX())||(y1<y0))
	{
	//	std::ostringstream os;
	//	os.str("");
	//	os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
	//	toErrorLog(os.str());
		return;
	}
	Sint16 yy0;
	Sint16 yy1;
	if(y0 < 0) {
		yy0=0;
	} else {
		yy0=y0;
	}
	if(y1>=getMaxY()) {
		yy1=getMaxY()-1;
	} else {
		yy1=y1;
	}
	(*this.*Draw_VLine)(x0, yy0, yy1);
}

void DC::DrawHorizontalLine(const Sint16 x0, const Sint16 y0, const Sint16 x1) const
{
	if((x1<0)||(x0>=getMaxX())||(y0<0)||(y0>=getMaxY())||(x1<x0))
	{
//	  std::ostringstream os;
//	  os.str("");
//	  os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
//		toLog("Line out of range");//os.str());
		return;
	}
	Sint16 xx0;
	Sint16 xx1;
	if(x0 < 0) {
		xx0=0;
	} else {
		xx0=x0;
	}
	if(x1>=getMaxX()) {
		xx1=getMaxX()-1;
	} else {
		xx1=x1;
	}
	(*this.*Draw_HLine)(xx0, y0, xx1);
}










