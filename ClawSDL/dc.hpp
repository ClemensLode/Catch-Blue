#ifndef _SDL_DC_HPP
#define _SDL_DC_HPP

#include "rect.hpp"
#include "pen.hpp"
#include "brush.hpp"
#include "font.hpp"

#include <sstream>

#define PRESSED_BRIGHTEN 50
#define PRESSED_DARKEN 130
#define PRESSED_NORMAL 90
#define NOT_PRESSED_DARKEN 60
#define NOT_PRESSED_BRIGHTEN 140

#define DC_MAX_CHANGED_RECTANGLES 200
 
#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif


enum DC_API eChooseDriverError
{
	DRIVER_SUCCESS,
	NO_VIDEO_DRIVERS_AVAILABLE,
	SDL_DRIVER_NOT_SUPPORTED
};

enum DC_API eBitDepth
{
	DEPTH_8BIT,
	DEPTH_16BIT,
	DEPTH_24BIT,
	DEPTH_32BIT
};

typedef struct DC_API tColorRGBA {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} tColorRGBA;

typedef struct DC_API tColorY {
	Uint8 y;
} tColorY;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_rmask 0xff000000
#define SDL_gmask 0x00ff0000
#define SDL_bmask 0x0000ff00
#define SDL_amask 0x00000000
#else
#define SDL_rmask 0x000000ff
#define SDL_gmask 0x0000ff00
#define SDL_bmask 0x00ff0000
#define SDL_amask 0x00000000
#endif



// singleton class
class DC_API DC
{
	public:
		DC(const char* chosen_driver);
		~DC();
		const bool initSDL(const Size& current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags);

		const std::string& getCurrentDriver() const;
		static const eChooseDriverError chooseDriver(std::string& chosen_driver, const std::string& selected_driver);
		
		void selectDriver(const std::string chosen_driver);

		static const char* getVersion();

		operator SDL_Surface*() const;
		SDL_Surface* operator->() const;
		SDL_Surface* getSurface() const;

		const bool isSurfacePuffered() const;

		const Size getSize() const;
		const Uint16 w() const;
		const Uint16 h() const;
		const bool valid() const;
		const Uint32 flags() const;
		const bool setResolution(const Size& current_resolution);
		const bool setBitDepth(const eBitDepth bit_depth);
		const bool setScreen(const Size& current_resolution, const eBitDepth bit_depth, const Uint32 nflags);
		const eBitDepth getBitDepth() const;
		const unsigned int getBits() const;
	
		const Uint16 pitch() const;
		void *pixels();
		void const* pixels() const;
		const struct private_hwdata *hwdata() const;

		const bool Lock() const;
		void Unlock() const;

		
		const Color doColor(const Uint8 r, const Uint8 g, const Uint8 b) const;

		const bool setColorKey(const Uint32 flag, const Color key) const;
		const bool setAlpha(const Uint32 flag, const Uint8 alpha) const;
		void updateScreen();
		
		void Blit(SDL_Surface* src, SDL_Rect& dstrect) const;
		
		void clearScreen();
		
		const bool SaveBMP(const std::string& file) const;

		const bool setFullScreen(const bool fullscreen = true);
		const bool isFullScreen() const;
		
		void setBrush(const Brush& brush);
		void setPen(const Pen& pen);
		void setTextForeground(const SDL_Color& text_color);
		void setFont(const Font& font);

		const Brush& getBrush() const;
		const Pen& getPen() const;
		const SDL_Color& getTextColor() const;
		const Font& getFont() const;



		const Size getTextExtent(const std::string& text) const;
		
		void DrawText(const std::string& text, const Point& p) const;
		
		void DrawBrightenedBitmap(const SDL_Surface* bitmap, const Point& p, const Uint8 brightness) const;

		void DrawBrightenedBitmap(const SDL_Surface* bitmap, const Point& p, const Rect& clip_rect, const Uint8 brightness) const;
	
		void DrawBitmap(const SDL_Surface* bitmap, const Point& p) const;
		
		void DrawBitmap(const SDL_Surface* bitmap, const Point& p, const Rect& clip_rect) const;

		void DrawEmptyRectangle(const Rect& rect) const;
		void DrawEmptyRectangle(const Point& p, const Size& s) const;
		
		void DrawRectangle(const Rect& rect) const; 
		void DrawRectangle(const Point& p, const Size& s) const;

		void DrawTabRectangle(const Rect& rect) const; 
		void DrawTabRectangle(const Point& p, const Size& s) const;


		void DrawSpline(const unsigned int c, const Point* p) const;
		void DrawSpline(const unsigned int c, const Point* p, const Point s) const;

		void DrawLine(const Point& p1, const Point& p2) const;
		
		void DrawVerticalLine(const Point& p, const Uint16 height) const;
		void DrawHorizontalLine(const Point& p, const Uint16 width) const;

		
		void DrawRoundedRectangle(const Point& p, const Size& s, const Uint16 radius) const;
		void DrawRoundedRectangle(const Rect& rect, const Uint16 radius) const;
		void DrawEdgedRoundedRectangle(const Point& p, const Size& s, const Uint16 radius) const;
		void DrawEdgedRoundedRectangle(const Rect& rect, const Uint16 radius) const;
		
		static const SDL_Color toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b);
		static const SDL_Rect createRect(const Sint16 x, const Sint16 y, const Uint16 w, const Uint16 h);
		static const SDL_Rect createRect(const Rect& r);
    	static const std::list<std::string> getAvailableDrivers();
	


		void setPressedRectangle(const bool pressed = true);

		void addRectangle(const Rect& rect);

		void switchToSurface(SDL_Surface* temp_surface);
		void switchToOriginalSurface();

		void drawFromPoint(const Point& p);
		const Uint16 getMaxX() const;
		const Uint16 getMaxY() const;
	private:

		static const char* dcVersion;

		unsigned int changedRectangles;
		SDL_Rect changedRectangle[DC_MAX_CHANGED_RECTANGLES];
		Uint16 max_x;
		Uint16 max_y;

		std::string currentDriver;

		SDL_Surface* surface;
		SDL_Surface* oldSurface;
		void setSurface(SDL_Surface* sdl_surface) {
			if(surface) {
				SDL_FreeSurface(surface);
			}
			surface = sdl_surface;
		}

		bool pressedRectangle;


		Point dp;
		
		Brush brush;
		Pen pen;
		SDL_Color textColor;
		Font font;
		
		eBitDepth bitDepth;
		unsigned int bits;
		Size resolution;

		void (DC::*Draw_HLine)(const Sint16 x0, const Sint16 y0, const Sint16 x1) const;
		void Draw_HLine_8bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const;
		void Draw_HLine_16bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const;
		void Draw_HLine_24bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const;
		void Draw_HLine_32bit(const Sint16 x0, const Sint16 y0, const Sint16 x1) const;


		void (DC::*Draw_VLine)(const Sint16 x0, const Sint16 y0, const Sint16 y1) const;
		void Draw_VLine_8bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const;
		void Draw_VLine_16bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const;
		void Draw_VLine_24bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const;
		void Draw_VLine_32bit(const Sint16 x0, const Sint16 y0, const Sint16 y1) const;


		void (DC::*Draw_Line)(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const;
		void Draw_Line_8bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const;
		void Draw_Line_16bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const;
		void Draw_Line_24bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const;
		void Draw_Line_32bit(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const;
		
		void (DC::*DrawFilledRound)(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledRound_16bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
	
		void (DC::*DrawTab)(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawTab_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawTab_16bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawTab_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawTab_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		
		void (DC::*DrawFilledEdgedRound)(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedRound_16bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		
		void (DC::*DrawEmptyEdgedRound)(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyEdgedRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyEdgedRound_16bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyEdgedRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyEdgedRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		
		void (DC::*DrawEmptyRound)(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyRound_16bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawEmptyRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		
		void (DC::*DrawFilledEdgedBorderRound)(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedBorderRound_8bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedBorderRound_16bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedBorderRound_24bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;
		void DrawFilledEdgedBorderRound_32bit(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 corner) const;

		void DrawBrightenedBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y, const Uint8 brightness) const;
		void DrawText(const std::string& text, const Sint16 x, const Sint16 y) const;
		void DrawBrightenedBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y, const Rect& clip_rect, const Uint8 brightness) const;
		void DrawBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y) const;
		void DrawBitmap(const SDL_Surface* bitmap, const Sint16 x, const Sint16 y, const Rect& clip_rect) const;
		void DrawEmptyRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawTabRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height) const;
		void DrawLine(const Sint16 x0, const Sint16 y0, const Sint16 x1, const Sint16 y1) const;
		void DrawVerticalLine(const Sint16 x0, const Sint16 y0, const Sint16 y1) const;
		void DrawHorizontalLine(const Sint16 x0, const Sint16 y0, const Sint16 x1) const;
		void DrawRoundedRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 radius) const;
		void DrawEdgedRoundedRectangle(const Sint16 x, const Sint16 y, const Uint16 width, const Uint16 height, const Uint16 radius) const;

		DC(const DC& other);
		DC &operator=(const DC& other);
};


inline const bool DC::isFullScreen() const {
	assert(surface);
	return ((surface->flags) & SDL_FULLSCREEN) == SDL_FULLSCREEN;
}


inline const char* DC::getVersion() {
	return dcVersion;
}

inline void DC::drawFromPoint(const Point& p) {
	dp = p;
}

inline void DC::setPressedRectangle(const bool pressed) {
	pressedRectangle = pressed;
}

inline DC& DC::operator=(const DC& other) {
	//if(*this != other) {
		setSurface(other.surface);
	//}
	return *this;
}

inline DC::operator SDL_Surface*() const { 
	return surface;
}
		
inline SDL_Surface* DC::operator->() const {
	return surface;
}
		
inline SDL_Surface* DC::getSurface() const {
	return surface;
}
		
inline const bool DC::valid() const {
	return (surface != NULL);
}
		
inline const Uint32 DC::flags() const {
	assert(surface);
	return getSurface()->flags;
}

inline const Size DC::getSize() const {
	return Size(w(), h());
}
		
inline const Uint16 DC::w() const {
	assert(surface);
	return (Uint16)(getSurface()->w);
}
		
inline const Uint16 DC::h() const {
	assert(surface);
	return (Uint16)(getSurface()->h);
}
		
inline const Uint16 DC::pitch() const {
	assert(surface);
	return surface->pitch;
}
		
inline void* DC::pixels() { 
	assert(surface);
	return surface->pixels; 
}
		
inline const void* DC::pixels() const {
	assert(surface);
	return surface->pixels;
}
		
inline const struct private_hwdata* DC::hwdata() const {
	assert(surface);
	return surface->hwdata;
}
		
inline const bool DC::SaveBMP(const std::string& file) const {
	if(file.size() == 0) {
		return false;// TODO SaveBMP(file.c_str()); 
	}
	else {
		return true;
	}
}

inline void DC::DrawLine(const Point& p1, const Point& p2) const {
	Point tp1 = dp + p1;
	Point tp2 = dp + p2;
	DrawLine(tp1.getX(), tp1.getY(), tp2.getX(), tp2.getY());
}

inline void DC::DrawText(const std::string& text, const Point& p) const {
	if(text.empty()) {
		return;
	}
	Point tp = dp + p;
	DrawText(text, tp.getX(), tp.getY());
}

inline void DC::DrawRoundedRectangle(const Point& p, const Size& s, const Uint16 radius) const {
	Point tp = dp + p;
	DrawRoundedRectangle(tp.getX(), tp.getY(), s.getWidth(), s.getHeight(), radius);
}
			
inline void DC::DrawRoundedRectangle(const Rect& rect, const Uint16 radius) const {
	Point tp = dp + rect.getTopLeft();
	DrawRoundedRectangle(tp.getX(), tp.getY(), rect.getWidth(), rect.getHeight(), radius);
}

inline void DC::DrawEdgedRoundedRectangle(const Point& p, const Size& s, const Uint16 radius) const {
	Point tp = dp + p;
	DrawEdgedRoundedRectangle(tp.getX(), tp.getY(), s.getWidth(), s.getHeight(), radius);
}
		
inline void DC::DrawEdgedRoundedRectangle(const Rect& rect, const Uint16 radius) const {
	Point tp = dp + rect.getTopLeft();
	DrawEdgedRoundedRectangle(tp.getX(), tp.getY(), rect.getWidth(), rect.getHeight(), radius);
}

inline void DC::setTextForeground(const SDL_Color& text_color) {
	textColor = text_color;
}

inline const Font& DC::getFont() const {
	return(font);
}

inline const Brush& DC::getBrush() const {
	return(brush);
}

inline const Pen& DC::getPen() const {
	return(pen);
}

inline const SDL_Color& DC::getTextColor() const {
	return(textColor);
}


inline const std::string& DC::getCurrentDriver() const {
	return currentDriver;
}

inline const bool DC::isSurfacePuffered() const {
	return(oldSurface != NULL);
}
		
inline void DC::DrawBrightenedBitmap(const SDL_Surface* bitmap, const Point& p, const Uint8 brightness) const {
	Point tp = dp + p;
	DrawBrightenedBitmap(bitmap, tp.getX(), tp.getY(), brightness);
}

inline void DC::DrawBrightenedBitmap(const SDL_Surface* bitmap, const Point& p, const Rect& clip_rect, const Uint8 brightness) const {
	Point tp = dp + p;
	DrawBrightenedBitmap(bitmap, tp.getX(), tp.getY(), clip_rect, brightness);
}

inline void DC::DrawBitmap(const SDL_Surface* bitmap, const Point& p) const {
	Point tp = dp + p;
	DrawBitmap(bitmap, tp.getX(), tp.getY());
}

inline void DC::DrawBitmap(const SDL_Surface* bitmap, const Point& p, const Rect& clip_rect) const {
	Point tp = dp + p;
	DrawBitmap(bitmap, tp.getX(), tp.getY(), clip_rect);
}	

inline void DC::DrawEmptyRectangle(const Rect& rect) const	{
	Point tp = dp + rect.getTopLeft();
	DrawEmptyRectangle(tp.getX(), tp.getY(), rect.getWidth(), rect.getHeight());
}
		
inline void DC::DrawEmptyRectangle(const Point& p, const Size& s) const {
	Point tp = dp + p;
	DrawEmptyRectangle(tp.getX(), tp.getY(), s.getWidth(), s.getHeight());
}
		
inline void DC::DrawRectangle(const Rect& rect) const { 
	Point tp = dp + rect.getTopLeft();
	DrawRectangle(tp.getX(), tp.getY(), rect.getWidth(), rect.getHeight());
}
		
inline void DC::DrawRectangle(const Point& p, const Size& s) const { 
	Point tp = dp + p;
	DrawRectangle(tp.getX(), tp.getY(), s.getWidth(), s.getHeight());
}

inline void DC::DrawTabRectangle(const Rect& rect) const { 
	Point tp = dp + rect.getTopLeft();
	DrawTabRectangle(tp.getX(), tp.getY(), rect.getWidth(), rect.getHeight());
}
		
inline void DC::DrawTabRectangle(const Point& p, const Size& s) const { 
	Point tp = dp + p;
	DrawTabRectangle(tp.getX(), tp.getY(), s.getWidth(), s.getHeight());
}

inline const bool DC::setColorKey(const Uint32 flag, const Color key) const {
	assert(surface);
	return SDL_SetColorKey(surface, flag, key) == 0;
}

inline const bool DC::setAlpha(const Uint32 flag, const Uint8 alpha) const {
	assert(surface);
	return SDL_SetAlpha(surface, flag, alpha) == 0;
}

inline void DC::setBrush(const Brush& dc_brush) {
	brush = dc_brush;
}

inline void DC::setPen(const Pen& dc_pen) {
	pen = dc_pen;
}
inline const Color DC::doColor(const Uint8 r, const Uint8 g, const Uint8 b) const {
	return Color(surface, r, g, b);
}

inline const SDL_Color DC::toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b) {
	SDL_Color c;
	c.r = r; c.g = g; c.b = b;
	c.unused = 255;
	return c;	
}



inline void DC::clearScreen() {
	assert(surface);
	SDL_Rect rc = createRect(0, 0, max_x, max_y);
	SDL_FillRect(surface, &rc, 1);
}

inline void DC::Blit(SDL_Surface* src, SDL_Rect& dstrect) const {
	assert(surface);
	SDL_BlitSurface(src, NULL, surface, &dstrect);
}

inline void DC::DrawVerticalLine(const Point& p, const Uint16 height) const {
	Point tp = dp + p;
	DrawVerticalLine(tp.getX(), tp.getY(), tp.getY() + height);
}

inline void DC::DrawHorizontalLine(const Point& p, const Uint16 width) const {
	Point tp = dp + p;
	DrawHorizontalLine(tp.getX(), tp.getY(), tp.getX() + width);
}

inline const SDL_Rect DC::createRect(const Sint16 x, const Sint16 y, const Uint16 w, const Uint16 h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

inline const SDL_Rect DC::createRect(const Rect& r) {
	SDL_Rect rect;
	rect.x = r.getLeft();
	rect.y = r.getTop();
	rect.w = r.getWidth();
	rect.h = r.getHeight();
	return rect;
}

inline const bool DC::setBitDepth(const eBitDepth bit_depth) {
	assert(surface);
	return setScreen(resolution, bit_depth, surface->flags);
}

inline const bool DC::setResolution(const Size& current_resolution) {
	assert(surface);
	return setScreen(current_resolution, bitDepth, surface->flags);
}

inline const Size DC::getTextExtent(const std::string& text) const {
	return(font.getTextExtent(text));
}

inline const Uint16 DC::getMaxX() const {
	return max_x;
}

inline const Uint16 DC::getMaxY() const {
	return max_y;
}

inline const unsigned int DC::getBits() const {
	return(bits);
}

inline const eBitDepth DC::getBitDepth() const {
	return(bitDepth);
}

#endif // _SDL_DC_HPP


