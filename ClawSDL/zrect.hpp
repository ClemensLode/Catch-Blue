#ifndef _SDL_ZRECT_HPP
#define _SDL_ZRECT_HPP

#include "rect.hpp"
#include <list>

class ZRect {
	public:
		ZRect(Rect rect, signed int z, long unsigned int ID);
		~ZRect();
		static const bool isInFrontOf(const ZRect& r1, const ZRect& r2);
		static void eraseOverlappingRects(std::list<ZRect>& rect_list);
		static void eraseOverlappingRects(std::list<ZRect>& old_rect_list, std::list<ZRect>& new_rect_list);
		static void connectRects(std::list<ZRect>& rect_list);
		Rect rect;
		signed int z;
		long unsigned int ID;
};

#endif