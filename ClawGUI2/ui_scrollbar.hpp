#ifndef _UI_SCROLLBAR_HPP
#define _UI_SCROLLBAR_HPP

#include "ui_button.hpp"
#include "windowcolors.hpp"

#define MAX_SCROLL_VALUE 10000

class UI_Window;

class UI_ScrollBar : public UI_Object
{
	public:
		UI_ScrollBar(SDL_Object* scroll_parent, /*const Rect& clientrect?*/ 
			const Sint16 start_y, 
			const Uint16 max_height,
			const ButtonColors* up_button_colors,
			const ButtonColors* down_button_colors,
			const WindowColors* window_colors,
			const bool scroll_hideable = false);
		~UI_ScrollBar();

		void process(); // process messages, continue animation etc.
		void draw(DC* dc) const;

		void moveUp();
		void moveDown();
		void moveToTop();
		void moveToBottom();

		void setClientHeight(const Uint16 height);
		void setClientTargetHeight(const Uint16 height);

		void setStartY(const Sint16 start_y);
		
		void setFirstItemY(const Sint16 first_item_y);
		void setLastItemY(const Sint16 last_item_y);
		const Sint16 getFirstItemY() const;
		const Sint16 getLastItemY() const;

		void checkBoundsOfChildren(const Sint16 upper_bound, const Sint16 lower_bound);

		const SDL_Object* checkHighlight(const Point& mouse);
	protected:
	private:
		Sint16 firstItemY;
		Sint16 lastItemY;

		Sint16 startY;
		
		UI_Button* add;
		UI_Button* sub;

		const WindowColors* windowColors;

		bool hideable; // hides if total height is smaller maxheight
		Sint16 internalScrollY;
		Uint16 internalHeight;
		Sint16 currentScrollY;
		Sint16 targetScrollY;
		Uint16 barHeight;
		
		Uint16 clientHeight;
		Uint16 clientTargetHeight;
		Uint16 totalHeight;

		Uint16 maxHeight;

		UI_ScrollBar& operator=(const UI_ScrollBar& object);
		UI_ScrollBar(const UI_ScrollBar& object);
};

inline void UI_ScrollBar::setStartY(const Sint16 start_y) {
	assert(start_y <= MAX_SCROLL_VALUE);
	startY = start_y;
}

inline void UI_ScrollBar::setFirstItemY(const Sint16 first_item_y) 
{
	assert(first_item_y <= MAX_SCROLL_VALUE);
	firstItemY = first_item_y;
}

inline void UI_ScrollBar::setLastItemY(const Sint16 last_item_y) 
{
	assert(last_item_y <= MAX_SCROLL_VALUE);

	lastItemY = last_item_y;
}

inline const Sint16 UI_ScrollBar::getFirstItemY() const 
{
	assert(firstItemY <= MAX_SCROLL_VALUE);

	return(firstItemY);
}

inline const Sint16 UI_ScrollBar::getLastItemY() const 
{
	assert(lastItemY <= MAX_SCROLL_VALUE);

	return(lastItemY);
}

#endif // _UI_SCROLLBAR_HPP

